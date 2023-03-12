
#include <stdio.h>
#include "analizadorSintactico.h"
#include "analizadorLexico.h"

void analizadorSintactico() {
    //Funcion que va a innvocar a siguiente.componente.lexico y va a imprimir el resultado
    lexema lex;
    while((lex = siguiente_componente_lexico()).valor != EOF) {
        if(lex.clave !=NULL) {
            printf("<%hd,%s>\n", lex.valor, lex.clave);
        }
    }
}