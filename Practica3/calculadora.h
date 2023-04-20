
#ifndef PRACTICA3_CALCULADORA_H
#define PRACTICA3_CALCULADORA_H

#include "TablaSimbolos.h"

/*Archivo cabecera de la calculadora implementada*/

/*
extern char **libDinamicas; //Variable para almacenar las librerias dinamicas
//Se guardan en una cadena de caracteres para poder buscarlas posteriormente.
//El proposito de esto es que la Tabla de simbolos es una tabla hash, por lo que para buscarlas
//es mejor tener un array auxiliar, que recorrer toda la tabla hash hasta encontrarlas.
//Ademas, asi permitiría invocar a una función de la librería sin tener que indicar el nombre de la librería,
//comprobando si la funcion existe en todas las librerias cargadas.
*/
void iniciar(char *argv); //Funcion para inicializar la calculadora

void cargarFichero(char *argv); //Funcion para cargar el fichero

void ayuda(); //Funcion para mostrar la ayuda

void salir(); //Funcion para salir de la calculadora

void workSpace(); //Funcion para mostrar el workspace de las variables

void eliminarWorkspace(); //Funcion para eliminar el workspace de las variables

void importarLibreria(char *lib); //Funcion para importar una libreria dinamicamente







#endif //PRACTICA3_CALCULADORA_H
