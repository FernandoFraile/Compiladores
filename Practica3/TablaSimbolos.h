

#ifndef PRACTICA1_TABLASIMBOLOS_H
#define PRACTICA1_TABLASIMBOLOS_H
#include <stdbool.h>
#include "tabla_hash_recolocacion.h"
#include "definiciones.h"





//Funcion para buscar un elemento en la tabla. Devuelve 0 si no está, y el valor asignado si está.
short buscarTabla(CompLexico *lex);
//Funcion para insertar un elemento en la tabla
void insertarTabla(CompLexico *lex);
//Funcion para inicializar la tabla de simbolos
void inicializarTablaSimbolos(); //Devuelve un EXIT_FAILURE en caso de error, EXIT_SUCCESS en caso de exito
//Funcion para imprimir lo que hay en la tabla de simbolos
void verTabla();
//Funcion para liberar la memoria de la tabla de simbolos
void liberarTablaSimbolos();
//Funcion para imprimir las variables del workspace
void imprimirVariables();
//Funcion para modificar un elemento de la tabla de simbolos
void modificarElementoTabla(CompLexico lex);
//Funcion para eliminar las variables del workspace
void eliminarVariables();
//Funcion para eliminar una variable
void eliminarVar(char *var);

int funcionLibDinamica(char *funcion); //Funcion para buscar una funcion en las librerias cargadas
                                        //Si existe la función, se carga en la tabla de Símbolos
                                        //Devuelve 1 si existe la función, 0 si no existe
/*Esta forma de hacerlo es costosa computacionalmente si la función no está en la tabla de símbolos, ya que hay que reccorer
 * dicha tabla. La ventaja es que el usuario puede indicarle la función, sin la necesidad de indicarle la librería en la cual
 * está la función.*/


void eliminarLibreria(char *lib); //Funcion para cerrar una libreria dinamicamente
                                //Se usa cuando se cierra la calculadora

void eliminarTodasLibrerias(); //Funcionn para eliminar todas las librerías cargadas dinamicamente
                                //Se utiliza para cuando se acaba la aplicación, con el fin de liberar memoria


#endif //PRACTICA1_TABLASIMBOLOS_H
