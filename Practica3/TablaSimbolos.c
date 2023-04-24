
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dlfcn.h>
#include "errores.h"
#include "TablaSimbolos.h"
#include "bison.tab.h"
#include "calculadora.h"


TablaHash Tabla; //Tabla de símbolos implementada con una tabla Hash
//short contador; //Contador para saber que valor asignar al siguiente elemento que entre en la tabla de simbolos
#define TAMTAB  79 //Tamanho de las lineas del fichero
#define TAMLEX 30 //Tamanho del lexema


void _liberarMemoria(CompLexico *lex){ //Funcion auxiliar para liberar la memoria del campo clave de la estructura
    free(lex->clave);
    lex->clave = NULL;

}

void inicializarTablaSimbolos(){
    FILE *fp;
    char *aux; //Para leer la macro definido
    CompLexico LEX;




    //Hay que inicializar el componente lexico y la linea para leer el fichero



    aux = (char*) malloc ((TAMLEX)*sizeof(char));
    if(aux == NULL){
        errorSistema(strerror(errno));
    }
    //Se crea la estrucutra para la tabla de simbolos
    Tabla= NULL; //Se inicializa a NULL
    Tabla = (TablaHash) malloc (TAMTAB*sizeof(CompLexico));
    if(Tabla == NULL){
        errorSistema(strerror(errno));
    }

    InicializarTablaHash(Tabla); //Se inicializa la tabla de simbolos

    //Hay que leer el archivo de definiciones. Primero realizo distintas comprobaciones
    fp = fopen("definiciones.h", "r");

    if (fp == NULL) {
        errorSistema(strerror(errno));
    }
    if(feof(fp)){
        errorSistema("Error: el archivo de definiciones esta vacio");
    }




    fclose(fp);


    //Se añaden tambien las constantes pi y e
    LEX.clave = strdup("pi");
    LEX.valor = CONSTANTE;
    LEX.variable = 3.14159265358979323846;
    InsertarHash(Tabla,LEX);
    _liberarMemoria(&LEX);

    LEX.clave = strdup("Pi");
    LEX.variable = 3.14159265358979323846;
    InsertarHash(Tabla,LEX);
    _liberarMemoria(&LEX);

    LEX.clave = strdup("e");
    LEX.variable = 2.71828182845904523536;
    InsertarHash(Tabla,LEX);
    _liberarMemoria(&LEX);


    //Se añaden los comandos disponibles en la calculadora
    LEX.clave = strdup("ayuda");
    LEX.valor = COMANDO;
    LEX.variable = 0;
    LEX.funcion = ayuda; //Se guarda la direccion de la funcion
    InsertarHash(Tabla,LEX);
    _liberarMemoria(&LEX);


    LEX.clave = strdup("cargarFichero");
    LEX.funcion = cargarFichero; //Se guarda la direccion de la funcion
    InsertarHash(Tabla,LEX);
    _liberarMemoria(&LEX);


    LEX.clave = strdup("salir");
    LEX.funcion = salir; //Se guarda la direccion de la funcion
    InsertarHash(Tabla,LEX);
    _liberarMemoria(&LEX);


    LEX.clave = strdup("workSpace");
    LEX.funcion = workSpace; //Se guarda la direccion de la funcion
    InsertarHash(Tabla,LEX);
    _liberarMemoria(&LEX);


    LEX.clave = strdup("eliminarWorkspace");
    LEX.funcion = eliminarWorkspace; //Se guarda la direccion de la funcion
    InsertarHash(Tabla,LEX);
    _liberarMemoria(&LEX);



    LEX.clave = strdup("import");
    LEX.funcion =  importarLibreria; //Se guarda la direccion de la funcion
    InsertarHash(Tabla,LEX);
    _liberarMemoria(&LEX);

    LEX.clave = strdup("cerrarLibreria");
    LEX.funcion =  cerrarLibreria; //Se guarda la direccion de la funcion
    InsertarHash(Tabla,LEX);
    _liberarMemoria(&LEX);

    LEX.clave = strdup("eliminarVariable");
    LEX.valor = ELIM; //No es lo mismo que COMANDO, ya que se va a tratar distinto en bison. Se diferencia este caso para evitar errores.
    LEX.funcion = eliminarVariable; //Se guarda la direccion de la funcion
    InsertarHash(Tabla,LEX);
    _liberarMemoria(&LEX);



    if(aux!=NULL){
        free(aux);
    }


}

short buscarTabla(CompLexico *lex){
    //Se busca en la tabla de simbolos
    short aux=(short)MiembroHash(Tabla,lex->clave);
    if(aux){
        Busqueda(Tabla,lex->clave,lex);
    }
    return aux;
}

void insertarTabla(CompLexico *lex){
    //Se inserta en la tabla de simbolos
    //Primero se incrementa el contador
    InsertarHash(Tabla,*lex);
}

void verTabla(){
    printf("------TABLA SIMBOLOS--------\n");
    imprimirTabla(Tabla);
}

void liberarTablaSimbolos(){

    BorrarTabla(Tabla);
    if(Tabla!=NULL){
        free(Tabla);
    }
}



void imprimirVariables(){
    for(int i=0;i<TAMTAB;i++) {
        CompLexico aux = Tabla[i];
        if(aux.valor==VAR){
            printf("%s = %f \n",aux.clave,aux.variable);
        }

    }

}

void modificarElementoTabla(CompLexico lex){
    ModificarHash(Tabla,lex.clave,lex);
}

void eliminarVariables(){
    char *clave; //Para guardar la clave de la variable
    for(int i=0;i<TAMTAB;i++) {
        CompLexico aux = Tabla[i];
        if(aux.valor==VAR){
            clave = strdup(aux.clave);
            BorrarHash(Tabla,clave); //Se elimina de la tabla Hash
            free(clave);
        }

    }
}

void eliminarVar(char *var){
    BorrarHash(Tabla,var);
}


int funcionLibDinamica(char *funcion){
    //Se recorre la tabla de símbolos buscando si existe la función en las librerías cargadas.
    //Al principio es más costoso computacionalmente, pero luego como se incluye la función en la tabla, el usuario
    //puede acceder directamente a ella.
    CompLexico introducirFunc; //Para introducir la función en la tabla de símboloss
    void (*fptr)(void ); //Para guardar la direccion de la funcion

    bool flag = false; //Para controlar el bluce
    int i=0;
    while(i<TAMTAB && flag==false) {
        CompLexico aux = Tabla[i];
        if(aux.valor==LIBRERIA){
            *(void **)(&fptr) = dlsym(aux.libreria,funcion); //Para guardar la direccion de la funcion
            introducirFunc.clave = funcion;
            introducirFunc.valor = FUNCION;
            if(fptr!=NULL){
                introducirFunc.funcion = fptr;
                flag = true;

            }
            else{
                errorSistema("No se ha encontrado la funcion en ninguna libreria cargada dinamicamente\n");
            }

            //Primero se comprueba si ya estaba
            if(buscarTabla(&introducirFunc)==0){
                //Si no está se inserta en la tabla de símbolos.
                InsertarHash(Tabla,introducirFunc);
            }

        }
        i++;

    }
    if(flag==true){
        return 1; //Se ha encontrado la función
    }
    else{
        return 0; //No se ha encontrado la función
    }




}

//Funcion privada para obtener el componente lexico que tiene la librería
CompLexico _obtenerLibreria(char *lib){
    CompLexico aux;
    aux.libreria = NULL;
    aux.clave = lib;
    buscarTabla(&aux);
    return aux;
}

void eliminarFuncionesLibreria(char *lib){
    //Se recorre la tabla de Símbolos
    CompLexico Complibreria; //Para guardar la librería
    void (*fptr)(void ); //Para guardar la dirección de la función

    for(int i=0; i<TAMTAB; i++){
        CompLexico aux = Tabla[i];
        if(aux.valor==FUNCION){
            //Se comprueba si la función pertenece a la librería
            //Para ello primero se obtiene la librería que se encuentra en la tabla de símbolos
            Complibreria = _obtenerLibreria(lib);
            //Se ve si la función está en la librería
            *(void **)(&fptr) = dlsym(Complibreria.libreria,aux.clave);
            if(fptr!=NULL){
                //Se elimina la función de la tabla de símbolos
                BorrarHash(Tabla,aux.clave);
            }


        }
    }


}

void eliminarLibreria(char *lib){

    CompLexico Complibreria = _obtenerLibreria(lib); //Se obtiene el componente lexico que contiene a la libreria
    if(Complibreria.libreria==NULL){
        errorSistema("La libreria no se ha cargado previamente\n");
    }
    else{
        eliminarFuncionesLibreria(lib);
        if(    dlclose(Complibreria.libreria)){
            errorSistema(dlerror());
        }
        BorrarHash(Tabla,lib);
    }


}

void eliminarTodasLibrerias(){
    //Se recorre la tabla de símbolos
    for(int i=0; i<TAMTAB; i++){
        CompLexico aux = Tabla[i];
        if(aux.valor==LIBRERIA){
            //Se elimina la librería
            eliminarLibreria(aux.clave);
        }
    }
    
}


