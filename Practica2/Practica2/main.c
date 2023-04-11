#include <stdlib.h>


#include "analizadorSintactico.h"
#include "TablaSimbolos.h"
#include "lexx.yy.h"



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



    //Se invoca al analizador sintáctico
    analizadorSintactico(filename);

    //Se ve como ha quedado la tabla de simbolos
    verTabla();

    //SE libera la memoria
    liberarLexema();//Se libera el lexema cuando se acaba de usar
    yylex_destroy();

    //Se liberan los recursos
    liberarTablaSimbolos();


    return EXIT_SUCCESS;
}
