
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "errores.h"
#include "TablaSimbolos.h"

TablaHash Tabla; //Tabla de símbolos implementada con una tabla Hash
CompLexico estructura;  //Estructura requerida para insertar en la tabla Hash
//short contador; //Contador para saber que valor asignar al siguiente elemento que entre en la tabla de simbolos
#define TAMTAB  79 //Tamanho de las lineas del fichero
#define TAMLEX 30 //Tamanho del lexema

void inicializarTablaSimbolos(){
    FILE *fp;
    char *aux; //Para leer la macro definido
    CompLexico LEX;
    short argumentos; //Variable para almacenar el numero de argumentos leidos en sscanf


    char *linea=NULL; //Para leer las lineas del fichero


    //Hay que inicializar el componente lexico y la linea para leer el fichero
    LEX.clave = (char*) malloc ((64)*sizeof(char));
    if(LEX.clave == NULL){
        errorSistema(strerror(errno));
    }
    linea = (char*) malloc ((TAMLEX)*sizeof(char));
    if(linea == NULL){
        errorSistema(strerror(errno));
    }

    aux = (char*) malloc ((TAMLEX)*sizeof(char));
    if(aux == NULL){
        errorSistema(strerror(errno));
    }
    //Se crea la estrucutra para la tabla de simbolos
    Tabla= NULL; //Se inicializa a NULL
    Tabla = (TablaHash) malloc (TAMTAB*sizeof(CompLexico));
    if(Tabla == NULL){
        errorSistema(strerror(errno));
    }


    InicializarTablaHash(Tabla); //Se inicializa la tabla de simbolos

    //Hay que leer el archivo de definiciones. Primero realizo distintas comprobaciones
    fp = fopen("definiciones.h", "r");

    if (fp == NULL) {
        errorSistema(strerror(errno));
    }
    if(feof(fp)){
        errorSistema("Error: el archivo de definiciones esta vacio");
    }


    //Se empieza a leer el archivo
    while (fgets(linea, TAMLEX, fp) != NULL) {
        argumentos=sscanf(linea,"#define %s %hd //%s",aux,&LEX.valor,LEX.clave);
        if(argumentos==3){
            InsertarHash(Tabla,LEX);


        }
    }


    fclose(fp);

    if(aux!=NULL){
        free(aux);
        free(LEX.clave);
    }
    if(linea!=NULL){
        free(linea);
    }


}

short buscarTabla(CompLexico *lex){
    //Se busca en la tabla de simbolos
    return (short)MiembroHash(Tabla,lex->clave);
}

void insertarTabla(CompLexico *lex){
    //Se inserta en la tabla de simbolos
    //Primero se incrementa el contador
    InsertarHash(Tabla,*lex);
}

void verTabla(){
    printf("------TABLA SIMBOLOS--------\n");
    imprimirTabla(Tabla);
}

void liberarTablaSimbolos(){

    BorrarTabla(Tabla);
    if(Tabla!=NULL){
        free(Tabla);
    }
}

