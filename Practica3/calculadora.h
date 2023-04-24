
#ifndef PRACTICA3_CALCULADORA_H
#define PRACTICA3_CALCULADORA_H

#include "TablaSimbolos.h"

/*Archivo cabecera de la calculadora implementada*/


void iniciar(char *argv); //Funcion para inicializar la calculadora

void cargarFichero(char *argv); //Funcion para cargar el fichero

void ayuda(); //Funcion para mostrar la ayuda

void salir(); //Funcion para salir de la calculadora

void workSpace(); //Funcion para mostrar el workspace de las variables

void eliminarWorkspace(); //Funcion para eliminar el workspace de las variables

void eliminarVariable(char *var); //Funcion para eliminar una variable del workspace

void importarLibreria(char *lib); //Funcion para importar una libreria dinamicamente

void cerrarLibreria(char *lib); //Funcion para cerrar una libreria que se ha abierto dinamicamente







#endif //PRACTICA3_CALCULADORA_H
