
/*Implementacion de la calculadora*/
#include  <stdio.h>
#include <stdbool.h>


#include "calculadora.h"
#include "errores.h"
#include "lex.yy.h"
#include "bison.tab.h"



//Función para cargar el fichero
void cargarFichero(char *argv){


    yyin = fopen(argv, "r");

    if (yyin == NULL) {
        errorSistema(strerror(errno));
        yyin = stdin;
    } else {
        //Hay que indicarle a el analizador sintactico que se está ejecutando un script
    }


}

void iniciar( char *argv){

    if(argv != NULL){
        //Se carga el fichero indicado
        cargarFichero(argv);
        //ejecutarFichero(true);
    }
    //Se comienza el analisis sintactico
/*
    int a=yylex()       ;
    while(a>0){
        printf("Lexema: %s\n",yytext);
        a=yylex();
    }
*/
        yyparse();


}

void ayuda(){
    printf("------------------------------------------------------------\n"
           "|  AYUDA DE LA CALCULADORA IMPLEMENTADA EN C                      |\n"
           "Uso: ./ejecutable <nombre_archivo> . <nombre_archivo es opcional>\n");
}

