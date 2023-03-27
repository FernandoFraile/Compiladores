
%{
#include "definiciones.h"
#include "TablaSimbolos.h"
#include "errores.h"

#define MAX_LEXEMA 64

CompLexico lex; /*Componente lexico*/
int lineas=0;   /*Para contar las lineas*/
int anidados=0; /*Para aceptar los comentarios anidados*/

/*Funcion interna para comprobar el tamaño del lexema*/
int comprobar_tamanho(){
    if(yyleng>MAX_LEXEMA){
        errorLexicoTamanho(lineas,yytext);
        return 0;
    }
    return 1;
}


%}

/*No se van a usar las siguientes opciones*/
%option noinput nounput
%option noyywrap

/*Condiciones de entrada*/
%x comentarioBloque
%x comentarioLinea
%x comentarioAnidado
%x HayAnidados
%x ComprobacionStrings


DIGITO [0-9]
IDENTIFICADOR   [_[:alpha:]][_[:alnum:]]*
TOKEN  [.;(){}\[\],,><\-*\/=+]
MASDOBLE \+\+
MASIGUALOP \+=
IGUALDOBLE ==
ESPACIOBLANCO [ \t]+
NUEVALINEA \n
CADENA \"((\\.|[^\"])*)\"
REAL [+-]?[0-9|_]+(\.)?[0-9]+([eE][+-][0-9]*)?
ENTERO [+-]?[0-9|_]+
BINARIO (0b[01]*)



%%

\" BEGIN(ComprobacionStrings);

<INITIAL>"/*" BEGIN(comentarioBloque);
<INITIAL>"//" BEGIN(comentarioLinea);
<INITIAL>"/+" {anidados++;BEGIN(comentarioAnidado);}


<ComprobacionStrings>\n lineas++;
<ComprobacionStrings><<EOF>> {errorLexicoString(lineas,"String mal formado (EOF)");} BEGIN(INITIAL);  /*Si llega a EOF es un error*/


<comentarioBloque>[^*\n]* /*come todo lo que no sea  '*' */
<comentarioBloque>"*"+[^*/\n]* /* come ’*’s no seguidos por ’/’ */
<comentarioBloque>\n lineas++;
<comentarioBloque><<EOF>> {errorLexicoComentario(lineas,"bloque");} BEGIN(INITIAL);  /*Si llega a EOF es un error*/
<comentarioBloque>"*"+"/" BEGIN(INITIAL);

<comentarioLinea>[^\n]* /*come todo lo que no sea \n */
<comentarioLinea>\n {lineas++;}  BEGIN(INITIAL);

<comentarioAnidado>[^+\n(\/)]* /*come todo lo que no sea  '+' o un '/+' */
<comentarioAnidado>"+"+[^+/\n]* /* come ’+’s no seguidos por ’/’ */
<comentarioAnidado>"\/"+[^/+\n]* /* come ’/’s no seguidos por ’+’ */
<comentarioAnidado>"+"+"/" {anidados--;if(anidados==0) BEGIN(INITIAL);}
<comentarioAnidado>\n lineas++;
<comentarioAnidado><<EOF>> {errorLexicoComentario(lineas,"anidado");} BEGIN(INITIAL);  /*Si llega a EOF es un error*/
<comentarioAnidado>"/"+"+" {anidados++;} /*Si hay un comentario anidado*/



{IDENTIFICADOR}  {  if(comprobar_tamanho()) return ID;}
{TOKEN} {  if(comprobar_tamanho()) return((int) *yytext); }
{MASDOBLE} {  if(comprobar_tamanho()) return MASMAS; }
{MASIGUALOP} {  if(comprobar_tamanho()) return MASIGUAL; }
{IGUALDOBLE} {  if(comprobar_tamanho()) return IGUALIGUAL; }
{ESPACIOBLANCO} /*No hace nada*/
{NUEVALINEA} lineas++;
{CADENA}  {  if(comprobar_tamanho()) return STRING; }
{REAL} {  if(comprobar_tamanho()) return FLOATING; }
{ENTERO} {  if(comprobar_tamanho()) return INTEGER; }
{BINARIO} {  if(comprobar_tamanho()) return INTEGER; }


%%


/*Para liberar la memoria del lexema*/
void liberarLexema(){
    lex.valor=0;
    if(lex.clave!=NULL){
        free(lex.clave);
        lex.clave=NULL;
    }

}
/*Funcion que va a devolver un componente lexico*/
CompLexico siguiente_componente_lexico(){

   liberarLexema();
   lex.valor= (short) yylex();
   lex.clave= strdup(yytext);
   //Se inserta en la tabla de simbolos si es un identificador
   if(lex.valor==ID){
       if((buscarTabla(&lex))==0){
           insertarTabla(&lex);
       }
   }
   return lex;
}

/*Para obtener las lineas leidas*/
int obtenerLineas(){
    return lineas;
}


