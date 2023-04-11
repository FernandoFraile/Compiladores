#include <stdlib.h>


#include "TablaSimbolos.h"
#include "lex.yy.h"
#include "errores.h"
#include "bison.tab.h"
#include "calculadora.h"



int main(int argc, char **argv) {


    //Se inicializa la tabla de símbolos
    inicializarTablaSimbolos(); //Se inicializa la tabla de símbolos
    verTabla(); //Se muestra la tabla

    if (argc == 2) {
        iniciar(argv[1]);
    } else if(argc==1) {
        iniciar(NULL);
    }else{
        printf("Uso: ./ejecutable <nombre_archivo> . <nombre_archivo es opcional>\n");
        return EXIT_FAILURE;
    }


    //SE libera la memoria
    /*
    liberarLexema();//Se libera el lexema cuando se acaba de usar
    liberarAnalizadorLexico(); */

    //Se liberan los recursos
    liberarTablaSimbolos();


    return EXIT_SUCCESS;
}
