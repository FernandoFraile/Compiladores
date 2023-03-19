
#include "TablaSimbolos.h"

//Funcion que lee el siguiente caracter del sistema de entrada
char siguienteCaracter();
//Funcion para inicializar el doble centinela utilizado para el sistema de entrada. Devuelve el tamaño maximo del lexema
int inicializarCentinela(char *nombreFichero);
//Funcion que acepta el lexema
// IMPORTANTE: INVOCAR SOLO SI NO HAY ERRORES
void aceptarLexema(CompLexico *lex);
//Funcion que devuelve el valor de la linea del fichero y la palabra. Utilizada para cuando hay un error
int obtenerLineaYPalabra(char *palabra);
//Funcion para que el analizador lexico le indique al sistema de entrada que el lexema ha sido reconocido, y devuelve un caracter
//Cuando el lexema no acaba en el ultimo caracter leido, si no en el anterior
void retroceder(); //Valor a true si se ha producido un error lexico
//Funcion para que el analizador lexico le indique al sistema de entrada que se ha producido un error lexico
void HayError();

//Funcion que va a invocar el analizador lexico cuando quiere saltar un caracter, como un espacio, \n etc. Devuelve el siguiente
char saltarCaracter();

//Funcion para aceptar los comentarios
void aceptarComentario();

//Funcion para acabar el sistema de entrada
void finalizarSistemaEntrada();
