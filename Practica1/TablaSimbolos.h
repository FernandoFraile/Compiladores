

#ifndef PRACTICA1_TABLASIMBOLOS_H
#define PRACTICA1_TABLASIMBOLOS_H
#include <stdbool.h>
#include "tabla_hash_recolocacion.h"
#include "definiciones.h"




//Funcion para buscar un elemento en la tabla. Devuelve 0 si no está, y el valor asignado si está.
short buscarTabla(lexema *lex);
//Funcion para insertar un elemento en la tabla
void insertarTabla(lexema *lex);
//Funcion para inicializar la tabla de simbolos
void inicializarTablaSimbolos(); //Devuelve un EXIT_FAILURE en caso de error, EXIT_SUCCESS en caso de exito
//Funcion para imprimir lo que hay en la tabla de simbolos
void verTabla();
//Funcion para liberar la memoria de la tabla de simbolos
void liberarTablaSimbolos();

#endif //PRACTICA1_TABLASIMBOLOS_H
