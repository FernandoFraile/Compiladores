#include <stdlib.h>


#include "analizadorLexico.h"
#include "analizadorSintactico.h"
#include "TablaSimbolos.h"
#include "sistemaEntrada.h"



int main(int argc, char **argv) {

    //Se inicializa la tabla de símbolos
    inicializarTablaSimbolos(); //Se inicializa la tabla de símbolos
    verTabla(); //Se muestra la tabla

    //Se inicializa el analizador lexico, qye ya inicializa el sistema de entrada
    inicializarAnalizadorLexico("regression.d"); //Se inicializa el sistema de entrada

    //Se invoca al analizador sintáctico
    analizadorSintactico();

    //Se ve como ha quedado la tabla de simbolos
    verTabla();

    //Se liberan los recursos
    liberarTablaSimbolos();
    finalizarAnalizadorLexico();


    return EXIT_SUCCESS;
}
