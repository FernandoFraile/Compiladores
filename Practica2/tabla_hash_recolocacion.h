/* 
 * File:   tablas_hash_recolocacion.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Tam 79
/*DEFINIR NUMERO de entradas en la tabla.*/
#define VACIO '\0'
#define BORRADO ' '

typedef struct{

    short valor;
    char *clave; //La secucencia de caracteres correspondiente al lexema

}CompLexico;

typedef CompLexico *TablaHash;

//Funcion para inicializar la tabla
void InicializarTablaHash(TablaHash t);
//Funcion Hash para obtener la posicion
int Hash(char *cad);
//Funcion para ver si ya esta el elemento. Si esta devuelve su valor, si no devuelve 0.
int MiembroHash(TablaHash t, char *cad) ;
//Función que busca un elemento con la clave HASH indicada
int Busqueda(TablaHash t, char *cad, CompLexico *e);
//FUncion para insertar un elemento en la tabla

//OJO, CAMBIAR LO DE COLICINES, LO HAGO PARA VER LA EFICIENCIA DE LA TABLA HASH
void InsertarHash(TablaHash t,  CompLexico e);
//Funcion para borrar un elemento de la tabla
void BorrarHash(TablaHash t, char *cad);

//Funcion para borrar toda la tabla
void BorrarTabla(TablaHash t);


//Funcion para imprimir la tabla
void imprimirTabla(TablaHash t);
