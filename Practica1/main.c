#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "analizadorLexico.h"
#include "analizadorSintactico.h"
#include "TablaSimbolos.h"
#include "sistemaEntrada.h"



int main(int argc, char **argv) {

    //Se inicializa la tabla de símbolos
    inicializarTablaSimbolos(); //Se inicializa la tabla de símbolos
    verTabla();

    //Se inicializa el sistema de entrada
    inicializarAnalizadorLexico(); //Se inicializa el sistema de entrada

    //Se invoca al analizador sintáctico
    analizadorSintactico();



    return EXIT_SUCCESS;
}
