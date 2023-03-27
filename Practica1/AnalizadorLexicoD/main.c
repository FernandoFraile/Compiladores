#include <stdlib.h>


#include "analizadorLexico.h"
#include "analizadorSintactico.h"
#include "TablaSimbolos.h"
#include "sistemaEntrada.h"



int main(int argc, char **argv) {

    char *filename = NULL;
    if (argc == 2) {
        filename = argv[1];
    } else {
        printf("Uso: ./ejecutable <nombre_archivo>\n");
        return EXIT_FAILURE;
    }

    //Se inicializa la tabla de símbolos
    inicializarTablaSimbolos(); //Se inicializa la tabla de símbolos
    verTabla(); //Se muestra la tabla

    //Se inicializa el analizador lexico, qye ya inicializa el sistema de entrada
    inicializarAnalizadorLexico(filename); //Se inicializa el sistema de entrada

    //Se invoca al analizador sintáctico
    analizadorSintactico();

    //Se ve como ha quedado la tabla de simbolos
    verTabla();

    //Se liberan los recursos
    liberarTablaSimbolos();
    finalizarAnalizadorLexico();


    return EXIT_SUCCESS;
}
