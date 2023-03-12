#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>


#include "analizadorLexico.h"
#include "sistemaEntrada.h"
#include "TablaSimbolos.h"
#include "errores.h"

bool aceptado; //Para saber si el lexema es aceptado o no
int maxlexema; //Tamaño maximo del lexema
int caracteresLeidos; //Para comprobar si exceden el tamanho maximo permitido
lexema lex; //Para devolver el lexema


//-------Funciones privadas------------


//Funcion para imprimir el lexema y linea en caso de que haya un error que supere el tamaño

void _liberarLexema(){
    if(lex.clave!=NULL){
        free(lex.clave);
        lex.clave=NULL;

    }
    lex.valor=0;
}

void _aceptar(int valor){
    aceptarLexema(&lex);
    lex.valor=(short) valor;
    aceptado=true;

}

void _errorTamaho(){
    char *palabra = (char *) malloc(sizeof(char)*(maxlexema+1)); //+1 para meter el /0
    if(palabra==NULL){
        errorSistema(strerror(errno));
    }
    errorLexicoTamanho(obtenerLineaYPalabra(palabra),palabra);
    free(palabra);
}


//Automata que reconoce identificadores y palabras reservadas
void _automataAlphanumerico(char c){
    bool aux=false; //Para no hacer la cromprobacion de si los caracteres son mayores que el tamanho permitido mas de una vez
    do{
        c=siguienteCaracter();
        caracteresLeidos++;
        if(caracteresLeidos>maxlexema && aux==false){     //Se comprueba si el lexema excede el tamaño maximo

            aux=true;
            _errorTamaho();
        }



    } while (isalpha(c) || isalnum(c) || c=='_');
    retroceder(aux);
    if(aux==false){ //Si no ha habido errores de tamaño, se acepta el lexema
        aceptarLexema(&lex);
        aceptado=true;
        //Se ve si esta en la tabla de simbolos, y si no se inroduce
        if((lex.valor=buscarTabla(&lex))==0){
            lex.valor=ID;
            insertarTabla(&lex);
        }

    }



};



void _automataComentariosBloque(){
    char c;
    bool correcto=false; //Para saber si se ha cerrado bien el comentario
    bool error=false; //Para saber si se ha producido un error
    int estado=0;
    do{

        c=siguienteCaracter();
        //Se lee hasta que se cierre bien un bloque

        if(c=='*'){
            c=siguienteCaracter();
            if(c=='/'){
                correcto=true;
            }
        }
        if(c==EOF){
            error=true;
            errorLexicoComentario(obtenerLineaYPalabra(NULL),"de bloque");
            retroceder(false);
        }
    } while (correcto==false && error==false);
}

void _automataComentariosLinea(){
    char c;
    bool correcto=false;
    bool error=false;
    do{
        c=siguienteCaracter();
        //Si se llega a un EOF esta mal formado
        if(c=='\n'){
            correcto=true;
        }
        if(c==EOF){
            error=true;
            errorLexicoComentario(obtenerLineaYPalabra(NULL),"de linea");
            retroceder(false);
        }

    }while(correcto==false && error==false ); //Se lee hasta que se llegue a un salto de linea
}

void _automataComentariosAnidados(){
    char c;
    bool correcto=false; //Par ver si el comentario es correcto
    bool error=false; //Para ver si se ha producido un error
    int estado=0; //Estados del automata
    int anidados=1; //Para saber cuantos comentarios tienen que anidarse
    do{
        c=siguienteCaracter();
        switch (estado) {
            case 0:
                if (c == '+') {
                    estado = 1;
                }
                if (c == '/') {
                    estado = 2;
                }
                if(c==EOF){
                    error=true;
                    errorLexicoComentario(obtenerLineaYPalabra(NULL),"de linea");
                    retroceder(false);
                }                break;
            case 1:
                if (c == '/') {
                    if(anidados==1){ //En este caso solo quedaba uno por cerrar
                        correcto = true;
                    }
                    anidados--; //Se ha cerrado un comentario anidado
                    estado=0;
                }
                else{
                    estado = 0; //Si solo era un +, se vuelve al estado inicial
                }
                break;
            case 2:
                if (c == '+') {
                    anidados++; //Se encuentra otro comentario aniado
                }
                estado=0;



                break;

        }

    }while(correcto==false && error==false);

}


void _automataComentarios(char c){
    //Se comrpueba que tipo de comentario es
    switch (c) {
        case '*':
            _automataComentariosBloque();
            break;
        case '/':
            _automataComentariosLinea();
            break;
        case '+':
            _automataComentariosAnidados();
            break;
        default:
            errorSistema("Error interno del analizador lexico\n");
            break;

    }

}

void _automataStrings(){
    char c;
    bool aux=false; //Para no hacer la cromprobacion de si los caracteres son mayores que el tamanho permitido mas de una vez
    bool error=false; //Para saber si se ha producido un error
    //Se lee hasta que se encuentre un "
    do{
        c=siguienteCaracter();
        caracteresLeidos++;
        if(caracteresLeidos>maxlexema && aux==false) {     //Se comprueba si el lexema excede el tamaño maximo
            aux = true;
            _errorTamaho();
        }
        if(c==EOF){ //Si se llega a un EOF esta mal formado
            error=true;
            errorLexicoString(obtenerLineaYPalabra(NULL),"String mal formado (EOF)");
            retroceder(false);
        }

    }while(c!='\"');
    //En este caso no se retrocede, porque el lexema acaba en " , y no es necesario leerlo otra vez
    if(aux==false){ //Si no ha habido errores de tamaño, se acepta el lexema
        aceptarLexema(&lex);
        aceptado=true;
        lex.valor=STRING;

    }

}

//IMPORTANTE: Si es un numero mal formado, se le mandara al analizador sintactico de todas formas y este se encargara de dar el error

void _automataNumeros(char c){

    bool aux=false; //Para no hacer la cromprobacion de si los caracteres son mayores que el tamanho permitido mas de una vez
    //Se van a diferenciar los casos segun el primer caracter que llegue
    int estado;
    if(c=='0'){
        estado=0; //Puede ser un numero entero,flotante,binario
    }
    else{
        estado=1;
    }
    do{
        c=siguienteCaracter();
        caracteresLeidos++;

        switch (estado) {
            case 0:
                switch (c) {
                    case 'B':
                    case 'b': //Si es un numero binario
                        estado=1;
                        break;
                    case '.': //Si es un numero flotante
                        estado=2;
                        break;
                    case 'e':

                        break;
                    default:
                        if(!isdigit(c)){ //Si no es uno de los casos descritos, es un entero
                            aceptado=true;
                            lex.valor=INTEGER;

                        }
                        //Puede ser un entero o flotante en otro caso, por lo que sigue en el estado cero
                        break;


                }
                break;
            case 1: //Si es un numero binario
                //Ahora se comprueba si los caracteres leidos son binarios o no
                if(c!='0' && c!='1'){
                    aceptado=true; //Se acepta cuando ya no es un 1 o un 0
                    lex.valor=INTEGER;
                }

                break;
            case 2:  //Si es un flotante
                

                break;

        }

    }while(aceptado==false);

        retroceder(false);  //Si hay error, hay que decirselo a retroceder
        aceptarLexema(&lex);

    }

}

//----------Fin de funciones privadas--------

void  inicializarAnalizadorLexico(){
    aceptado=false;
    //Se inicializan los valores del lexema(No se reserva memoria para el campo que almacenará el string porque esto lo hara
    // el sistema de entrada, segun el tamaño de dicho string)
    lex.valor=0;
    lex.clave=NULL;
    maxlexema=inicializarCentinela();
}


lexema siguiente_componente_lexico(){
    char c; //Caracter para leer
    int estado=0; //Estado inicial del automata

    //Antes de empezar, hay que liberar el componente lexico
    _liberarLexema();
    aceptado=false; //Se inicializa a false

    do{
        c=siguienteCaracter();
        while((c==' ' || c=='\t' || c=='\n' || c=='\r' || c=='\v' || c=='\f') && estado==0){
                //Es necesaria la comprobacion del estado, ya que el espacio puede determinar si estamos ante un tipo de
                //lexema u otro, por lo que si se salta, se pierde esa informacion .
                //Solo si es un estado incial del reconocimiento de automatas se saltará, interpretando entonces que es
                //un lexema de espacio en blanco, pero que no se le manda al analizador sintactico

              //Si sucede esto, se pasa al siguiente caracter. No es un error, ya que cuando un automata acaba
              // en un espacio en blanco, ya lo ha reconocido, y lo que ha hecho es volver atras
              c=saltarCaracter();

        }
        caracteresLeidos=1; //Se inicializa a 0 el numero de caracteres leidos
        switch (estado) {
            case 0: //Estado inicial del automata

                if(isalpha(c) || c=='_'){ //Automata que reconoce alphanumericos
                    _automataAlphanumerico(c);
                }
                else if(isdigit(c)){ //Si es un numero
                    _automataNumeros(c);
                }

                else{
                    switch (c) {
                        case ';':
                        case '.':
                        case '(':
                        case ')':
                        case '[':
                        case ']':
                        case ',':
                        case '{':
                        case '>':
                        case '-':
                        case '*':
                        case '}':
                            _aceptar(c); //Se acepta el lexema
                            break;
                        //Aqui empiezan casos en los que pueden ser distintos lexemas segun lo que haya a la derecha
                        case '/':
                            estado=1;
                            break;
                        case '=':
                            estado=2;
                            break;
                        case '"':
                            estado=3;
                            break;
                        case '+':
                            estado=4;
                            break;



                        default:
                            errorSistema("Error en el analizador lexico: caracter no reconocido\n");
                            break;
                    }
                }
                break;
            case 1: //  '/'
                //Puede ser un comentario o una division
                switch (c) {
                    case '*':
                    case '/':
                    case '+':
                        _automataComentarios(c);
                        estado=0;
                        break;
                    default:
                        retroceder(false); //Se retrocede porque no es un comentario, para devolver lo siguiente
                        _aceptar('/'); //Se acepta la division
                }
                break;
            case 2: //  '='
                //Puede ser un operador de asignacion o un operador de comparacion
                switch (c) {
                    case '=':
                        //Para saber el valor asignado se busca en la tabla de simbolos

                        _aceptar(IGUALIGUAL);
                        estado=0;
                        break;
                    default:
                        retroceder(false); //Se retrocede porque no es un IGUALIGUAL
                        _aceptar('='); //Se acepta el igual
                        estado=0;
                }
                break;
            case 3: //  '"'
                _automataStrings();
                estado=0;
                break;
            case 4: //  '+'
                switch (c) {
                    case '+':
                        _aceptar(MASMAS);
                        estado=0;
                        break;
                    case '=':
                        _aceptar(MASIGUAL);
                        estado=0;
                        break;

                    default:
                        retroceder(false); //Se retrocede porque es un + normal
                        _aceptar('+'); //Se acepta el mas
                        estado=0;
                }
            default:
                break;
        }


    } while (c!=EOF && aceptado==false );

    return lex;
}



