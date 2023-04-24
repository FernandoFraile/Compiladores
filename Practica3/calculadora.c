
/*Implementacion de la calculadora*/
#include  <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>

#include "calculadora.h"
#include "errores.h"
#include "lex.yy.h"
#include "bison.tab.h"
#include "TablaSimbolos.h"


bool flagFichero = false; //Variable utilizada para saber si se ha introducido un fichero al ejecutar el programa
                            //Su explicacón está comentada en la función cargarFichero

void cargarFichero(char *argv){


    yyin = fopen(argv, "r");

    if (yyin == NULL) {
        errorSistema(strerror(errno));
        yyin = stdin;
    } else {
        //Hay que indicarle a el analizador sintactico que se está ejecutando un script
        printf("--Se va a ejecutar el fichero: %s\n", argv) ;
        ejecutarFichero(true);

        while (!feof(yyin)) {
            yyparse();
        }
        fclose(yyin);
        if(argv!=NULL && flagFichero==false){
            //Se comprueba si el flagFichero es false, para evitar que se libere la memoria cuando se ha introducido
            //como argumento del ejecutable. De otro modo, se realizaría un free de un puntero que no se ha reservado
            free(argv);
        }
        yyrestart(stdin);
        ejecutarFichero(false);
        flagFichero = false;
        printf(">>");
        yyparse();
    }


}

void iniciar( char *argv){
    printf("------------------------------------------\n"
                  "|      BIENVENIDO A LA CALCULADORA        |\n"
                  "------------------------------------------\n");

    if(argv != NULL){

        //Se carga el fichero indicado, si no se indica se ejecuta en modo interactivo
        flagFichero = true;
        cargarFichero(argv);
    }
    else{
        printf(">>");

        yyparse(); //Se invoca a yyparse, con stdin como entrada predeterminada
    }

}

void ayuda(){
    printf("----------------------------------------\n"
                  "|       AYUDA DE LA CALCULADORA        |\n"
                  "----------------------------------------\n"

    "Uso: ./ejecutable <nombre_archivo> . <nombre_archivo es opcional>\n"
           "Si no se indica el nombre del archivo se ejecuta en modo interactivo\n"
           "El archivo puede contener comentarios de una linea (//) o de bloque (/**/)\n\n"
           "Realiza operaciones aritmeticas y asignaciones de variables como: "
           "+,-,^,/,*,sin,cos,tan,log,exp,sqrt\n"
           "La calculadora mostrará el resultado de las operaciones cuando se ejecute un fichero, o se indique un "
           "';' al final de cada instruccion, cuando se ejecute el modo interactivo\n"
           "\n\n"
           "COMANDOS DE LA CALCULADORA:\n"
           "-ayuda: Muestra la ayuda de la calculadora\n"
           "-salir: Sale de la calculadora\n"
           "-cargarFichero(fichero): Carga el fichero indicado. Una vez a acaba de leerlo vuelve a modo interactivo\n"
           "-workSpace: Muestra el workspace de la calculadora\n"
           "-eliminarWorkspace: Elimina el workspace de la calculadora\n"
           "-eliminarVariable(variable): Elimina la variable indicada\n"
           "-import(libreria): Importa una libreria dinamicamente. Se debe indicar el nombre de la libreria\n"
           "\tLa librería matemática ya está incluida estáticamente para poder realizar las funcione mínimas indicadas"
           "previamente. \nPara ejecutar librerías creadas, hay que situar el arhcivo .so en la carpeta en la que busca dlopen (se puede "
           "ver ejecutando el comando ldd ejecutable, viendo donde están las bibliotecas compartidas que dependen del ejecutable)"
           ", o se puede indicar en la variable LD_LIBRARY_PATH el directorio donde se encuentran las librerías dinámicas. También se puede indicar"
           "directamente en el comando la ruta completa, import(/ruta_completa_al_archivo/libreria.so)\n"
           "-cerrarLibreria(libreria): Cierra la librería indicada. Se debe indicar el nombre de la librería, sin extensiones \n");


}

void salir(){
    printf("Saliendo de la calculadora...\n");



    //Hay que invocar a todas las funciones de liberar la memoria
    eliminarTodasLibrerias();
    yylex_destroy();
    liberarTablaSimbolos();

    exit(EXIT_SUCCESS);
}

void workSpace(){
    printf("----------------------------------------\n"
                  "|       WORKSPACE DE LA CALCULADORA     |\n"
                  "----------------------------------------\n");

    imprimirVariables();

}

void eliminarWorkspace(){
    printf("----------------------------------------------------\n"
                  "|       ELIMINANDO WORKSPACE DE LA CALCULADORA     |\n"
                  "----------------------------------------------------\n");


    eliminarVariables();
}


void eliminarVariable(char *var){
    printf("Eliminando variable %s ...\n", var);
    eliminarVar(var);
}


void importarLibreria(char *lib){
    char *nombre = strdup(lib);
    CompLexico LEX;
    //Se comprueba que la librería no esté ya cargada

    strtok(lib, "."); //Se obtiene el nombre de la librería

    LEX.clave = lib;
    if(buscarTabla(&LEX)){
        printf("La libreria %s ya está cargada\n", lib);
    }
    else{
        //Se utiliza la función dlopen disponible en dlfcn.h, para cargar liberías dinámicamente.
        void* libreria = dlopen(nombre, RTLD_LAZY);
        if (libreria == NULL) {
            errorSistema(dlerror());
        }
        else{
            printf("Importando libreria %s\n", lib);

            //Se ha cargado la librería correctamente, por lo que se va a insertar en la tabla de símbolos
            LEX.valor = LIBRERIA;
            LEX.clave = lib;
            LEX.libreria = libreria;
            insertarTabla(&LEX); //Se inserta la librería en la tabla de símbolos.
            printf("Libreria %s importada correctamente\n", lib);

        }
    }
    free(nombre);



}


void cerrarLibreria(char *lib){
    printf("Cerrando libreria:  %s\n", lib);
    //Se va a cerrar la librería, por lo que se va a eliminar de la tabla de símbolos
    //Se elimina la librería de la tabla de símbolos
    eliminarLibreria(lib);

}


