
/*Implementacion de la calculadora*/
#include  <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>

#include "calculadora.h"
#include "errores.h"
#include "lex.yy.h"
#include "bison.tab.h"
#include "TablaSimbolos.h"



char **libDinamicas = NULL;

void cargarFichero(char *argv){


    yyin = fopen(argv, "r");

    if (yyin == NULL) {
        errorSistema(strerror(errno));
        yyin = stdin;
        yyparse();
    } else {
        //Hay que indicarle a el analizador sintactico que se está ejecutando un script
        ejecutarFichero(true);
        /*
        int a=yylex();
        while(a>0) {
            printf("Lexema: %d, %s\n", a, yytext);
            a = yylex();
        }*/

        while (!feof(yyin)) {
            yyparse();
        }
        fclose(yyin);
        yyrestart(stdin);
        ejecutarFichero(false);
        yyparse();
    }


}

void iniciar( char *argv){

    if(argv != NULL){

        //Se carga el fichero indicado, si no se indica se ejecuta en modo interactivo
        cargarFichero(argv);
    }
    else{
        yyparse();
    }

}

void ayuda(){
    printf("----------------------------------------\n"
                  "|       AYUDA DE LA CALCULADORA        |\n"
           "Uso: ./ejecutable <nombre_archivo> . <nombre_archivo es opcional>\n"
           "Si no se indica el nombre del archivo se ejecuta en modo interactivo\n"
           "Realiza operaciones aritmeticas y asignaciones de variables como: "
           "+,-,^,/,*,sin,cos,tan,log,exp,sqrt\n"
           "\n"
           "COMANDOS DE LA CALCULADORA:\n"
           "-ayuda: Muestra la ayuda de la calculadora\n"
           "-salir: Sale de la calculadora\n"
           "-cargarFichero: Carga el fichero indicado. Una vez a acaba de leerlo vuelve a modo interactivo\n"
           "-workSpace: Muestra el workspace de la calculadora\n"
           "-eliminarWorkspace: Elimina el workspace de la calculadora\n");


}

void salir(){
    printf("Saliendo de la calculadora...\n");
    //Hay que invocar a todas las funciones de liberar la memoria

    //LIBERAR LO DE FLFEX Y BISON
    yylex_destroy();
    liberarTablaSimbolos();

    exit(EXIT_SUCCESS);
}

void workSpace(){
    printf("----------------------------------------\n"
                  "|       WORKSPACE DE LA CALCULADORA     |\n");
    imprimirVariables();

}

void eliminarWorkspace(){
    printf("----------------------------------------\n"
                  "|       ELIMINANDO WORKSPACE DE LA CALCULADORA     |\n");
    eliminarVariables();
}


void importarLibreria(char *lib){
    char *nombre;
    CompLexico LEX;
    printf("Importando libreria %s\n", lib);
    //Se utiliza la función dlopen disponible en dlfcn.h, para cargar liberías dinámicamente.
    void* libreria = dlopen(lib, RTLD_LAZY);
    if (libreria == NULL) {
        printf("Error cargando la librería: %s\n", dlerror());
        errorSistema("Error cargando la librería");
    }
    else{
        nombre = strtok(lib, "."); //Se obtiene el nombre de la librería
        LEX.valor = LIBRERIA;
        LEX.clave = nombre;
        LEX.libreria = libreria;
        insertarTabla(&LEX); //Se inserta la librería en la tabla de símbolos.
        /*
        if(libDinamicas==NULL){
            libDinamicas = (char**) malloc(sizeof(char*));
            if(libDinamicas==funcionLibDinamicaNULL){
                errorSistema("Error al reservar memoria");
            }
            libDinamicas[0] = lib;
        }*/
        printf("Libreria %s importada correctamente\n", lib);

    }

}



