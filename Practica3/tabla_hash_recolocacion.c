#include "tabla_hash_recolocacion.h"


/* TABLA HASH CON RECOLOCACION */

void InicializarTablaHash(TablaHash t) {
    int i;

    for (i = 0; i < Tam; i++){
        t[i].clave = (char *) malloc(sizeof(char));
        t[i].clave[0]=VACIO;
        t[i].valor=0;
        t[i].variable=0;
        t[i].funcion=NULL;
        t[i].funcion=NULL;
    }

}


/******* FUNCIONES HASH *******/

/* FUNCION HASH 1 */
/*
int Hash(char *cad) {
    int valor;
    unsigned char *c;

    for (c = cad, valor = 0; *c; c++)
        valor += (int) *c;

    return (valor % Tam);
}
*/

/* FUNCION HASH 2 */
/*
int Hash (char *cad){
    int i,suma=0;
    for (i=strlen(cad)-1;i>=0;i--){
        suma=(suma*256+cad[i])%Tam;
    }
    return suma;
}*/

/* FUNCION HASH 3: Probad al menos dos valores para la constante K */

int Hash (char *cad){
    int i,suma=0;
    int K=13;
    for (i=strlen(cad)-1;i>=0;i--){
        suma=(suma*K+cad[i])%Tam;
    }
    return suma;
}





/* RECOLOCACION LINEAL: depende del valor de la constante a*/

/* Función que localiza la posición del elemento cuando buscamos*/
int _PosicionBuscar(TablaHash t, char *cad) {
    // Devuelve el sitio donde está la clave cad, o donde debería estar.
    // No tiene en cuenta los borrados para avanzar.

    int ini, i, aux, a=1;
    // Hay que dar valor a la variable de recolocación a

    ini = Hash(cad);

    for (i = 0; i < Tam; i++) {
        aux = (ini + a*i) % Tam;
        if (t[aux].clave[0] == VACIO)
            return aux;
        if (!strcmp(t[aux].clave, cad))
            return aux;

    }
    return ini;
}

//Función que localiza la posicion para insertar un elemento
int _PosicionInsertar(TablaHash t, char *cad) {
    // Devuelve el sitio donde podriamos poner el elemento de clave cad

    int ini, aux, i, a=1;
    // Hay que dar un valor a la variable de recolocación a

    ini = Hash(cad);

    for (i = 0; i < Tam; i++) {
        aux = (ini + a*i) % Tam;
        if (t[aux].clave[0] == VACIO || t[aux].clave[0] == BORRADO){
            return aux;
        }
        if (!strcmp(t[aux].clave, cad)){
            return aux;
        }

    }
    return ini;
}



/* RECOLOCACION CUADRATICA */
/*
int _PosicionBuscar(TablaHash t, char *cad) {
    // Devuelve el sitio donde esta cad  o donde deberia  estar.
    // No tiene en cuenta los borrados.

    int ini, i, aux;

    ini = Hash(cad);

    for (i = 0; i < Tam; i++) {
        aux = (ini + i*i) % Tam;
        if (t[aux].clave[0] == VACIO)
            return aux;
        if (!strcmp(t[aux].clave, cad))
            return aux;

    }
    return ini;
}

int _PosicionInsertar(TablaHash t, char *cad) {
   //Devuelve el sitio donde podriamos poner cad (recolocacion cuadratica)
    int ini, i, aux;

    ini = Hash(cad);
    for (i = 0; i < Tam; i++) {
        aux = (ini + i*i) % Tam;
        if (t[aux].clave[0] == VACIO || t[aux].clave[0] == BORRADO)
            return aux;
        if (!strcmp(t[aux].clave, cad))
            return aux;

    }
    return ini;
}

*/
/* Funcion que indica si un elemento está o no en la tabla */
/* Devuelve el valor si el elemento de clave cad está en la tabla, 0 si no está */
int MiembroHash(TablaHash t, char *cad) {


    int pos = _PosicionBuscar(t,cad);
    if (t[pos].clave[0] == VACIO)
        return 0;
    else
        return (t[pos].valor);

}

/* BUSCA UN ELEMENTO CON LA CLAVE INDICADA EN LA TABLA HASH, Y LO DEVUELVE,
 * ADEMAS DE INDICAR CON 1 QUE EXISTE EL ELEMENTO, Y CON 0 QUE NO ESTA EN LA TABLA */
int Busqueda(TablaHash t, char *clavebuscar,  CompLexico *e) {

    int pos = _PosicionBuscar(t,clavebuscar);

    if (t[pos].clave[0] == VACIO)
        return 0;
    else {
        if (strcmp(t[pos].clave, clavebuscar) == 0) {
            *e = t[pos];
            return 1;
        } else return 0;
    }
}

/* Función que inserta un elemento en la tabla */
void InsertarHash(TablaHash t,CompLexico e) {
    int pos;
    Hash(e.clave);

    pos = _PosicionInsertar(t,e.clave);


    if (t[pos].clave[0] == VACIO || t[pos].clave[0] == BORRADO) {
        t[pos].clave = (char *) realloc(t[pos].clave,(strlen(e.clave)+1)*sizeof(char));

        strcpy(t[pos].clave, e.clave);
        t[pos].valor = e.valor;
        t[pos].variable = e.variable;
        t[pos].funcion = e.funcion;
        t[pos].libreria = e.libreria;
    }
    else if (strcmp(t[pos].clave,e.clave)){
        printf("No se encuentra posicion libre.\n");
    }
}


/* Función que elimina un elemento de la tabla */
void BorrarHash(TablaHash t, char *cad) {
    int pos = _PosicionBuscar(t,cad);

    if (t[pos].clave[0] != VACIO && t[pos].clave[0] != BORRADO) {
        if (!strcmp(t[pos].clave, cad)) {
            t[pos].clave[0]= BORRADO;
            t[pos].clave[1]= VACIO;
            t[pos].valor = 0;
            t[pos].variable = 0;
        }
    }
}

//Funcion que borra todos los elementos de la tabla
void BorrarTabla(TablaHash t){
    for (int i = 0; i < Tam; i++){
        t[i].clave[0]=VACIO;
        free(t[i].clave);
    }
}

void imprimirTabla(TablaHash t){
    for (int i = 0; i < Tam; i++) {
        CompLexico actual = t[i];
        printf("%hd \t %s \t %lf\n", actual.valor,actual.clave,actual.variable);
    }
}

int ModificarHash(TablaHash t, char *cad, CompLexico e){
    int pos = _PosicionBuscar(t,cad);

    if (t[pos].clave[0] == VACIO)
        return 0;
    else {
        if (strcmp(t[pos].clave, cad) == 0) {
            t[pos].variable= e.variable;
            return 1;
        } else return 0;
    }
}






