
#include <stdio.h>
#include <errno.h>
#include "analizadorSintactico.h"
#include "TablaSimbolos.h"
#include "lexx.yy.h"
#include "errores.h"

void analizadorSintactico(char *filename) {
    //Funcion que va a innvocar a siguiente_componente_lexico y va a imprimir el resultado
    //Se abre el archivo de entrada

    //Se inicializa la entrada del analizador léxico
    FILE* archivo = fopen(filename, "r");
    if (archivo == NULL) {
            errorSistema(strerror(errno));
    }

    yyin = archivo;

    CompLexico lex;
    printf("-----------Comienza la etapa de analisis lexico--------------\n");
    while((lex=siguiente_componente_lexico()).valor) {
        printf("< %d , %s >\n", lex.valor, lex.clave);
    }
    liberarLexema();//Se libera el lexema cuando se acaba de usar
    yylex_destroy();
    printf("-----------Fin de la etapa de analisis lexico--------------\n");
    //Se cierra el archivo
    if(archivo!=NULL){
        fclose(archivo);
    }




}