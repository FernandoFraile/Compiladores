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
CompLexico lex; //Para devolver el lexema


//-------Funciones privadas------------


//Funcion para imprimir el lexema y linea en caso de que haya un error que supere el tamaño

void _liberarCompLexico(){
    if(lex.clave!=NULL){
        free(lex.clave);
        lex.clave=NULL;

    }
    lex.valor=0;
}

//Funcion para aceptar un lexema
//Se utiliza para aceptar los lexemas que, por ejemplo, tienen codigo ASCII asociado
void _aceptar(int valor){
    aceptarLexema(&lex);
    lex.valor=(short) valor;
    aceptado=true;

}

//Funcion para imprimir un error de tamaño de lexema, y gestionarlo
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
        caracteresLeidos++; //Se van sumando caracteres para comprobar si exceden el tamanho maximo
        if(caracteresLeidos>maxlexema && aux==false){     //Se comprueba si el lexema excede el tamaño maximo
            //Si ya se habia ejecutado la funcion de error, no se vuelve a ejecutar
            aux=true;
            _errorTamaho();
        }



    } while (isalpha(c) || isalnum(c) || c=='_');
    retroceder(); //Se retrocede porque el reconocimiento del lexema acaba en el caracter siguiente
    if(aux) HayError(); //Si hay un error de tamaño hay que indicarselo al sistema de entrada

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
        //Se lee hasta que se encuentra un */
        // /* /* */ */ No se acepta, se le mandara dos componentes lexicos, * y /
        // /* /* */  Se acepta

        switch (estado) {
            case 0:
                if (c == '*') {
                    estado = 1;
                }
                break;
            case 1:
                if(c=='/'){
                    correcto=true;
                }
                else{
                    estado=0;
                }
                break;

        }

        if(c==EOF){ //Segun la especificacion, si no se cierra antes del EOF esta mal formado
            error=true;
            errorLexicoComentario(obtenerLineaYPalabra(NULL),"bloque");
            retroceder();
            HayError(); //Se indica que hay un error al sistema de entrada

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
            retroceder();
            HayError(); //Se indica que hay un error al sistema de entrada
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
                    estado = 2; //Puede ser otro comentario anidado
                }
                if(c==EOF){
                    error=true;
                    errorLexicoComentario(obtenerLineaYPalabra(NULL),"de linea");
                    retroceder();
                    HayError(); //Se indica que hay un error al sistema de entrada
                }
                break;
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
                    anidados++; //Se encuentra otro comentario anidado
                }
                estado=0;



                break;
            default:
                errorSistema("Error interno del analizador lexico\n");
                break;

        }
        if(c==EOF){ //Segun la especificacion, si no se cierra antes del EOF esta mal formado
            error=true;
            errorLexicoComentario(obtenerLineaYPalabra(NULL),"anidado");
            retroceder();
            HayError(); //Se indica que hay un error al sistema de entrada
        }

    }while(correcto==false && error==false);

}




void _automataComentarios(char c){
    //Se comrpueba que tipo de comentario es
    switch (c) {
        case '*':
            _automataComentariosBloque();
            aceptarComentario();
            break;
        case '/':
            _automataComentariosLinea();
            aceptarComentario();
            break;
        case '+':
            _automataComentariosAnidados();
            aceptarComentario();
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
    int estado=0; //Estados del automata. El estado de error es para cuando se supera el tamaño maximo

    //Se lee hasta que se encuentre un " no escapado

    do{
        c=siguienteCaracter();
        caracteresLeidos++; //Se aumenta el numero de caracteres leidos
        if(caracteresLeidos>maxlexema && aux==false) {     //Se comprueba si el lexema excede el tamaño maximo
            aux = true;
            _errorTamaho();
        }
        if(caracteresLeidos>maxlexema){
            //Una vez se llega a este punto, se lee hasta que se encuentre un "
            switch (estado) {
                case 0:
                    if(c=='"'){
                        aceptado=true;
                    }
                    if(c=='\\'){
                        estado=1; //Puede ser que se escape un caracter de ", por lo que se va a un estado 1
                    }

                    break;
                case 1:
                    estado=0; //Se vuelve al estado inicial. Este estado es solo para gestionar el " escapado
                    break;
                default:
                    errorSistema("Error interno del analizador lexico\n");
                    break;

            }

        }
        if(c==EOF){ //Si se llega a un EOF esta mal formado
            error=true;
            errorLexicoString(obtenerLineaYPalabra(NULL),"String mal formado (EOF)");
            retroceder();
            HayError(); //Se indica que hay un error al sistema de entrada
        }
        if(aux==false){ //Si no ha habido errores de tamaño, se sigue el automata para posible aceptacion del lexema
            switch (estado) {
                case 0:
                    if(c=='"'){
                        aceptado=true;
                        lex.valor=STRING;
                    }
                    if(c=='\\'){
                        estado=1; //Puede ser que se escape un caracter de ", por lo que se va a un estado 1
                    }
                    break;
                case 1:
                    estado=0; //Se vuelve al estado inicial. Este estado es solo para gestionar el " escapado
                    break;
                default:
                    errorSistema("Error interno del analizador lexico\n");
                    break;


            }
        }


    }while(aceptado==false && error==false);
    //En este caso no se retrocede, porque el lexema acaba en " , y no es necesario leerlo otra vez
    if(aux) HayError(); //Se indica si hay un error al sistema de entrada
    if(aux==false){ //Si no ha habido errores de tamaño, se acepta el lexema
        aceptarLexema(&lex);
        aceptado=true;
        lex.valor=STRING;

    }

}

//IMPORTANTE: Si es un numero mal formado, se le mandara al analizador sintactico de todas formas y este se encargara de dar el error

void _automataNumeros(char c){

    bool retr=true; //Para saber si se tiene que retroceder o no, ya que hay algunos numeros que no acaban en caracter siguiente
    bool aux=false; //Para no hacer la cromprobacion de si los caracteres son mayores que el tamanho permitido mas de una vez
    //Se van a diferenciar los casos segun el primer caracter que llegue
    int estado =0; //Estados del automata.

    do{


        switch (estado) {
            case 0:
                switch (c) {
                    case '0':
                        estado=2; //Puede ser un numero entero,flotante,binario
                        break;

                    default:
                        estado=1; //Si no es un cero, se sigue la comprobacion para ver si es un flotante o un entero
                        break;
                }
                break;
            case 1:
                switch (c) {
                    case '.':
                        estado=4; //Se comprueba si es un numero flotante
                        break;
                    case 'E':
                    case 'e':
                        estado=6; //Se comprueba si es un numero flotante
                        break;
                    case 'F':
                    case 'f':
                    case 'i':
                    case 'L':
                        //Si se indica una f, F, i o L al final, se acepta el lexema, siendo flotante
                        aceptado=true;
                        retr=false; //No se retrocede, porque el numero acaba en el caracter leido, no el siguiente
                        lex.valor=FLOATING;
                        break;
                    case 'u':
                    case 'U':

                        //Si se indica una u o U al final, se acepta el lexema, siendo entero
                        aceptado=true;
                        retr=false; //No se retrocede, porque el numero acaba en el caracter leido, no el siguiente
                        lex.valor=INTEGER;
                        break;
                    default:
                        if(!(isdigit(c) || c=='_')){ //Si es un numero o un guion bajo, se sigue leyendo
                            aceptado=true; //Si no es ninguno de los casos anteriores, se acepta el lexema
                            lex.valor=INTEGER;
                        }
                        break;
                }
                break;
            case 2:
                switch (c) {
                    case 'B':
                    case 'b': //Si es un numero binario
                        estado=3;
                        break;
                    //Hay que incluir los casos del estado 1, ya que puede ser un 0f, por ejemplo
                    case '.':
                        estado=4; //Se comprueba si es un numero flotante
                        break;
                    case 'E':
                    case 'e':
                        estado=6; //Se comprueba si es un numero flotante
                        break;
                    case 'F':
                    case 'f':
                    case 'i':
                    case 'L':
                        //Si se indica una f,F,i,L al final, se acepta el lexema, siendo flotante
                        aceptado=true;
                        retr=false; //No se retrocede, porque el numero acaba en el caracter leido, no el siguiente
                        lex.valor=FLOATING;
                        break;
                    case 'u':
                    case 'U':

                        //Si se indica una u o U al final, se acepta el lexema, siendo entero
                        aceptado=true;
                        retr=false;     //No se retrocede, porque el numero acaba en el caracter leido, no el siguiente
                        lex.valor=INTEGER;
                        break;

                    default:
                        if(!(isdigit(c) || c=='_')){ //Si no es ningun caso de los anteriores, y se acaba, es un entero
                            aceptado=true;
                            lex.valor=INTEGER;
                        }
                        else{
                            estado=1;  //Puede ser un entero o flotante en otro caso, por lo que se va al estado 1

                        }

                        break;


                }
                break;
            case 3://Se comprueba si es un numero binario
                //Ahora se comprueba si los caracteres leidos son binarios o no
                if(!(c=='0' || c=='1')){
                    aceptado=true; //Se acepta cuando ya no es un 1 o un 0
                    lex.valor=INTEGER;
                }


                break;
            case 4: //Se comprueba si es un numero flotante
                //En este punto, solo puede haber un numero, ya que se ha leido un .
                if(!(isdigit(c) || c=='_')){//Si no es ese caso, se acepta el lexema, y el analizador sintactico ya se encargará de gestionar el error
                    aceptado=true;
                    lex.valor=FLOATING;
                }
                else{
                    estado=5;
                }



                break;
            case 5: //Estado intermedio porque puede combinarse un . con un e
                if(c=='e' || c=='E'){
                    estado=6;

                }
                else if(!(isdigit(c) || c=='_')){
                    aceptado=true;
                    lex.valor=FLOATING;
                }
                else{
                    estado=5;

                }
                break;

            case 6: //Se comprueba si es un numero flotante
                //En este punto se ha leido un e o un E, por lo que se comprueba si a continuación hay un + o un -
                if(c=='+' || c=='-'){
                    estado=7;
                }
                else{ //Si no, se acepta el lexema
                    aceptado=true;
                    lex.valor=FLOATING;
                }
            break;
            case 7: //Se comprueba si es un numero flotante
                //Se siguen leyendo caracteres hasta que se acabe el numero flotante
                if(!(isdigit(c) || c=='_')){
                    aceptado=true;
                    lex.valor=FLOATING;
                }
                break;
            default:
                errorSistema("Erro interno en el analizador lexico\n");
                break;

        }

        if(aceptado==false){ //Se comprueba debido a la construccion del bucle, para evitar leer un caracter de mas cuando
                                //ya se ha aceptado la cadena
            c=siguienteCaracter();
            caracteresLeidos++;
        }
        if(caracteresLeidos>maxlexema && aux==false){     //Se comprueba si el lexema excede el tamaño maximo

            aux=true;
            _errorTamaho();
        }


    }while(aceptado==false);

        if(retr) {
            retroceder();  //Se retrocede si es necesario
            if(aux) HayError();  //Si se ha producido un error, se indica
        }
        aceptarLexema(&lex); //Se acepta el lexema



}

//Funcion privada para realizar el automata
//Se implementa en dos pasos, una privada y la pública, para que si el lexema está a NULL, no se devuelva al analizador
//sintáctico, y por lo tanto solo le lleguen lexemas correctos
void _siguiente_componente_lexico(){
    char c; //Caracter para leer
    int estado=0; //Estado inicial del automata

    //Antes de empezar, hay que liberar la estrucutura
    _liberarCompLexico();
    aceptado=false; //Se inicializa a false, hasta que se reconozca un lexema y se acepte el componente lexico

    do{
        c=siguienteCaracter(); //Se lee el siguiente caracter
        

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
                else if(c==EOF){
                    lex.valor=EOF; //Se acepta el lexema
                    //Este caso es solo para reconocer el EOF
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
                        case '<':
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
                            _automataStrings(); //Se invoca al automata de strings
                            break;
                        case '+':
                            estado=3;
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
                        _automataComentarios(c); //Se invoca al automata de comentarios
                        estado=0;
                        break;
                    default:
                        retroceder(); //Se retrocede porque no es un comentario, para devolver lo siguiente
                        _aceptar('/'); //Se acepta la division
                }
                break;
            case 2: //  '='
                //Puede ser un operador de asignacion o un operador de comparacion

                if(c=='='){
                    _aceptar(IGUALIGUAL);
                    estado=0;
                }
                else{
                    retroceder(); //Se retrocede porque no es un IGUALIGUAL
                    _aceptar('='); //Se acepta el igual
                    estado=0;
                }


                break;

            case 3: //  '+'
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
                        retroceder(); //Se retrocede porque es un + normal
                        _aceptar('+'); //Se acepta el mas
                        estado=0;
                }
            default:

                break;
        }


    } while (c!=EOF && aceptado==false );

    //Recordamos que esta es una funcion privada para gestionar cuando el Componente Lexico es NULL


}



//----------Fin de funciones privadas--------

void  inicializarAnalizadorLexico(char *nombreFichero){
    aceptado=false;
    //Se inicializan los valores del lexema(No se reserva memoria para el campo que almacenará el lexema porque esto lo hara
    // el sistema de entrada, segun el tamaño de dicho lexema)
    lex.valor=0;
    lex.clave=NULL;
    maxlexema=inicializarCentinela(nombreFichero);
}


CompLexico siguiente_componente_lexico(){
   do{
       _siguiente_componente_lexico(); //se devuelve el componente lexico, mientras no sea NULL o EOF
   } while (lex.clave==NULL && lex.valor!=EOF);
   return lex;
}

void finalizarAnalizadorLexico(){
    _liberarCompLexico(); //Se libera la estructura
    finalizarSistemaEntrada(); //Se finaliza el sistema de entrada

}



