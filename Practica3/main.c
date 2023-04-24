#include <stdlib.h>


#include "TablaSimbolos.h"
#include "errores.h"
#include "calculadora.h"



int main(int argc, char **argv) {


    //Se inicializa la tabla de símbolos
    inicializarTablaSimbolos(); //Se inicializa la tabla de símbolos

    //Se comprueba si se ha pasado un argumento
    //Si se ha pasado un argumento se ejecuta el fichero indicado
    if (argc == 2) {
        iniciar(argv[1]); //Función dispnible en calculadora.c, que
    } else if(argc==1) {
        iniciar(NULL);
    }else{
        printf("Uso: ./ejecutable <nombre_archivo> . <nombre_archivo es opcional>\n");
        return EXIT_FAILURE;
    }




    return EXIT_SUCCESS;
}
