
#include <stdio.h>
#include "analizadorSintactico.h"
#include "analizadorLexico.h"

void analizadorSintactico() {
    //Funcion que va a innvocar a siguiente_componente_lexico y va a imprimir el resultado
    CompLexico lex;
    printf("-----------Comienza la etapa de analisis lexico--------------\n");
    while((lex = siguiente_componente_lexico()).valor != EOF) {
            printf("<%hd,%s>\n", lex.valor, lex.clave);

    }
    printf("-----------Fin de la etapa de analisis lexico--------------\n");

}