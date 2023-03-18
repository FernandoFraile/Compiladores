#include <unistd.h>
#include <stdbool.h>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "sistemaEntrada.h"
#include "errores.h"
#include "errno.h"

#define MAX_LEXEMA 64
char centinela[2*(MAX_LEXEMA+1)]; //Doble centinela de tamaño 2*MAX_LEXEMA+1
int inicio,siguiente; //Punteros al inicio y al siguiente caracter
FILE *fp; //Archivo que se va a leer
int linea; //Para saber en que linea se encuentra el error lexico
bool cargar; //Variable para saber si tiene que cargar el sigueinte bloque. Se pone a false cuando se retrocede el puntero
                //siguiente porque se acepta un lexema


char cargarBloque(){

    int n; //Bytes
    //OJO: Si el tamanho del lexema excede el tamaño de bloque, se comprobará en el analizador léxico

    //Primero se comprueba si el fichero esta abierto correctamente
    if(fp==NULL){
        errorSistema(strerror(errno));
    }
    //Se lee con fread
    //Dependiendo de donde esté el puntero siguiente, se carga el bloque A o B del centinela
    if(siguiente==0 || siguiente==2*MAX_LEXEMA+1){ //Cargar Bloque A
        //La primera comprobacion es por si es la primera llamada a la funcion, y la segunda por si se ha llegado al final del bloque B
        n=fread((char *) centinela, sizeof(char), MAX_LEXEMA, fp);
        if(feof(fp)){ //Cuando se llega al final del archivo
            centinela[n]=EOF;
        }
        if(ferror(fp)){
            errorSistema(strerror(errno));
        }


    }
    else if(siguiente==MAX_LEXEMA){ //Cargar Bloque B
        n=fread((char *) centinela+MAX_LEXEMA+1, sizeof(char), MAX_LEXEMA, fp);
        if(feof(fp)){ //Cuando se llega al final del archivo
            centinela[n+MAX_LEXEMA]=EOF;
        }
        if(ferror(fp)){
            errorSistema(strerror(errno));
        }
    }
    else{ //Se gestiona el error de que el puntero no esté en ningun EOF
        errorSistema("Error: el puntero siguiente no apunta a ningun final de bloque del centinela");
    }

}


char siguienteCaracter(){

    //Para devolver el caracter se ve donde esta el puntero siguiente

    if(centinela[siguiente]==EOF){ //Se ve si se ha llegado al final de bloque
        if(siguiente==MAX_LEXEMA){ //Se ha llegado al final del bloque A
            if(cargar==true){
                cargarBloque();

            }
            else{
                cargar=true;
            }
            siguiente++; //Se mueve el puntero siguiente al siguiente primer caracter del bloque B
        }
        else if(siguiente==2*MAX_LEXEMA+1){ //Se ha llegado al final del bloque b
            //Se ha llegado al final del bloque B
            if(cargar==true){
                cargarBloque();

            }
            else{
                cargar=true;
            }
            siguiente=0; //Se vuelve a la posicion inicial del bloque A
        }
        else{
            return centinela[siguiente];
        }
    }
    if(centinela[siguiente]=='\n'){ //Pequeña comprobacion para que se imprima la linea en la que está el error lexico
        linea++;
    }
    char aux=centinela[siguiente];
    //Se mueve la posicion de siguiente al siguiente caracter del bloque
    siguiente++;
    return aux; //Se devuelve el caracter que tocaba

}
void _lexemaReconocido() {
    //Se mueve el puntero inicio al siguiente caracter del lexema

    inicio = siguiente;
    //La comprobacion de si el ini
    if(inicio==MAX_LEXEMA){
        inicio++;
    }
    if(inicio==2*MAX_LEXEMA+1){
        inicio=0;
    }

}

char saltarCaracter(){
    _lexemaReconocido();
    return siguienteCaracter();
}



void retroceder(bool err){
    //Se comprueba donde está el puntero siguiente
    if(siguiente==0 || siguiente==MAX_LEXEMA){  //En esta situacion esta justo antes de un EOF
        cargar=false; //Se pone a false para que no se cargue el siguiente bloque 2 veces, cuando se invoque a siguiente cacracter
        cargarBloque();
    }
    siguiente--; //Se retrocede el puntero siguiente
    if(err==true){ //Gestion de casos de error
        inicio=siguiente; //Se pone el puntero inicio al mismo sitio que el puntero siguiente
    }

}

void aceptarComentario(){
    _lexemaReconocido(); //Solo cambia el puntero de inicio
}

void aceptarLexema(lexema *lex){

    //Se reserva memoria
    //Hay 2 casos distintos: los punteros estan en distintos bloques de memoria y están el el mismo
    //Recordemos que el caso de que el lexema
    if((inicio<MAX_LEXEMA && siguiente<=MAX_LEXEMA) || (inicio>MAX_LEXEMA && siguiente>MAX_LEXEMA)){ //Mismo bloque
        lex->clave = (char *) malloc(sizeof(char)*(siguiente-inicio+1)); //Se suma uno para añadir un caracter fin de cadena '\0'
        strncpy(lex->clave,centinela+inicio,sizeof(char)*(siguiente-inicio));
        strncpy(lex->clave+(siguiente-inicio),"\0",1);

    }
    else{ //Si estan en distinto bloque
        //Recordemos otra vez que no va a pasar nunca que el lexema sea mayor que el tamanho del bloque
        if(inicio<MAX_LEXEMA){ //Empieza en el bloque A
            lex->clave = (char *) malloc(sizeof(char)*(siguiente-inicio)); //El +1 ya viene dado por la posicion del EOF
            strncpy(lex->clave,centinela+inicio,sizeof(char)*(MAX_LEXEMA-inicio)); //Se copia la parte del primer bloque
            strncpy(lex->clave+(MAX_LEXEMA-inicio),centinela+MAX_LEXEMA+1,sizeof(char)*(siguiente-MAX_LEXEMA-1)); //Se copia la parte del segundo bloque
            strncpy(lex->clave+(siguiente-inicio-1),"\0",1);
        }
        else{ //Empieza en el bloque B
            //Calculamos cuantas posiciones hay que cada bloque
            //2*MAX_LEXEMA+1-inicio: tamaho que hay en el segundo bloque
            //siguiente: tamanho que hay en el primer bloque
            lex->clave = (char *) malloc(sizeof(char)*(2*MAX_LEXEMA+1-inicio+siguiente+1));
            strncpy(lex->clave,centinela+inicio,sizeof(char)*(2*MAX_LEXEMA+1-inicio)); //Se copia la parte del segundo bloque
            strncpy(lex->clave+(2*MAX_LEXEMA+1-inicio),centinela,sizeof(char)*(siguiente)); //Se copia la parte del primer bloque
            strncpy(lex->clave+(2*MAX_LEXEMA+1-inicio+siguiente),"\0",1);


        }

    }


    _lexemaReconocido();

}

int obtenerLineaYPalabra(char *palabra){
    //Se copia el lexema en la variable palabra
    int auxiliar; //se va a utilizar para calcular cuanto hay que copiar del lexema si estan en distintos bloques
    if(palabra!=NULL){
        //Hay que diferenciar si el inicio esta en el primer bloque o en el segundo
        if((inicio<MAX_LEXEMA && siguiente<MAX_LEXEMA) || (inicio>MAX_LEXEMA && siguiente>MAX_LEXEMA)){ //Mismo bloque
            strncpy(palabra,centinela+inicio,sizeof(char)*(siguiente-inicio-1));
            strncpy(palabra+(siguiente-inicio-1),"\0",1);

        }
        else{ //Si estan en distinto bloque
            if(inicio<MAX_LEXEMA){ //Empieza en el bloque A
                if(MAX_LEXEMA-inicio<MAX_LEXEMA){
                    strncpy(palabra,centinela+inicio,sizeof(char)*(MAX_LEXEMA-inicio)); //Se copia la parte del primer bloque
                    strncpy(palabra+MAX_LEXEMA-inicio, centinela+MAX_LEXEMA+1,sizeof(char)*(MAX_LEXEMA-(MAX_LEXEMA-inicio))); //Se copia la parte del segundo bloque
                    strncpy(palabra+MAX_LEXEMA,"\0",1);

                }
                else{
                    strncpy(palabra,centinela+inicio,sizeof(char)*(MAX_LEXEMA)); //Se copia la parte del segundo bloque
                    strncpy(palabra+MAX_LEXEMA,"\0",1);
                }
            }
            else{ //Empieza en el bloque B
                if(2*MAX_LEXEMA+1-inicio<MAX_LEXEMA){
                    strncpy(palabra,centinela+inicio,sizeof(char)*(2*MAX_LEXEMA+1-inicio)); //Se copia la parte del segundo bloque
                    strncpy(palabra+2*MAX_LEXEMA+1-inicio,centinela,sizeof(char)*(MAX_LEXEMA-(2*MAX_LEXEMA+1-inicio))); //Se copia la parte del segundo bloque
                    strncpy(palabra+MAX_LEXEMA,"\0",1);

                }
                else{
                    strncpy(palabra,centinela+inicio,sizeof(char)*(MAX_LEXEMA)); //Se copia la parte del segundo bloque
                    strncpy(palabra+MAX_LEXEMA,"\0",1);

                }


            }

        }

    }
    return linea;
}



int inicializarCentinela(char *nombreFichero){

    centinela[MAX_LEXEMA] = EOF; //Se pone el EOF en la ultima posicion del primer bloque
    centinela[2*MAX_LEXEMA+1] = EOF;   //Se pone el EOF en la ultima posicion del segundo bloque
    /* Esquema del centinela:
     * ||.....|EOF||....|EOF|
     * Bloque A   Bloque B
     * */

    inicio = 0;
    siguiente = 0;
    linea=1; //Se inicializa la linea a 1
    cargar=true; //Se inicializa a true para que se cargue los bloques

    //Se lee el fichero de entrada, abriendolo en modo lectura y leyendolo con fread para mantener el puntero
    fp=fopen(nombreFichero,"r");
    if(fp==NULL){
        errorSistema(strerror(errno));
    }
    if(feof(fp)){
        errorSistema("Error: el archivo de entrada esta vacio");
    }
    //Ahora se llena el primer bloque
    cargarBloque();
    return MAX_LEXEMA;

}


void finalizarSistemaEntrada(){
    //Se cierra el fichero
    if(fp!=NULL){
        fclose(fp);

    }



}

