
#include "TablaSimbolos.h"

//Funcion que lee el siguiente caracter del sistema de entrada
//Hay que mandarle el tamaño del lexema ,
char siguienteCaracter();
//Funcion para inicializar el doble centinela utilizado para el sistema de entrada. Devuelve el tamaño maximo del lexema
int inicializarCentinela();
//Funcion que acepta el lexema (SOLO SI NO HAY ERRORES)
void aceptarLexema(lexema *lex);
//Funcion que devuelve el valor de la linea del fichero y la palabra. Utilizada para cuando hay un error
int obtenerLineaYPalabra(char *palabra);
//Funcion para que el analizador lexico le indique al sistema de entrada que el lexema ha sido reconocido, y devuelve un caracter
void retroceder(bool err); //Valor a true si se ha producido un error lexico
//Se hace asi porque por ejemplo si se excede el tamaño del lexema, el analizador lexico recorrera todo el lexema excedido
//por lo que el puntero a inicio debe ponerse una vez haya acabo de recorrerlo, en la posicion siguiente. Lo mismo ocurre
//cuando hay un error de lexemas mal formados.

//Funcion que va a invocar el analizador lexico cuando quiere saltar un caracter, como un espacio, \n etc. Devuelve el siguiente
char saltarCaracter();
