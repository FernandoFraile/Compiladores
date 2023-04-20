
%{
#include <stdio.h>
#include "definiciones.h"
#include "TablaSimbolos.h"
#include "errores.h"
#include "bison.tab.h"

#define MAX_LEXEMA 64

CompLexico lex; /*Componente lexico*/
int lineas=0;   /*Para contar las lineas*/

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
%option header-file="lex.yy.h"


/*Condiciones de entrada*/
%x comentarioBloque
%x comentarioLinea


LETRA [a-zA-Z_]
OPERADORES [\.;\(\)\[\],><\-*\/=\+]
ESPACIOBLANCO [ \t]+
NUEVALINEA \n
REAL [-]?[0-9|_]+(\.)?[0-9]+([eE][+-][0-9]*)?
ENTERO [-]?[0-9|_]+
SENO "sin"
COS "cos"
TAN "tan"
LOG "log"
EXP "exp"
SQRT "sqrt"
VARIABLE {LETRA}({LETRA}|{REAL}|{ENTERO})*
NUMERO {REAL}|{ENTERO}
FICHERO ({LETRA}|{NUMERO})+\.txt
LIBRERIA ({LETRA}|{NUMERO})+\.so(\.({LETRA}|{NUMERO}))*





%%


<INITIAL>"/*" BEGIN(comentarioBloque);
<INITIAL>"//" BEGIN(comentarioLinea);


<comentarioBloque>[^*\n]* /*come todo lo que no sea  '*' */
<comentarioBloque>"*"+[^*/\n]* /* come ’*’s no seguidos por ’/’ */
<comentarioBloque>\n lineas++;
<comentarioBloque><<EOF>> {errorLexicoComentario(lineas,"bloque");} BEGIN(INITIAL);  /*Si llega a EOF es un error*/
<comentarioBloque>"*"+"/" BEGIN(INITIAL);

<comentarioLinea>[^\n]* /*come todo lo que no sea \n */
<comentarioLinea>\n {lineas++;}  BEGIN(INITIAL);


{ESPACIOBLANCO} /*No hace nada*/
{NUEVALINEA} lineas++; return (int) *yytext;
{OPERADORES} { return (int) *yytext; }

{NUMERO} {  if(comprobar_tamanho()){
                          yylval.variable=atof(yytext);
                          return NUMERO;
                           } }
{SENO} {
          yylval.cadena= strdup(yytext);
          return SIN;
        }
{COS} {
          yylval.cadena= strdup(yytext);
          return COS;
        }
{TAN} {
          yylval.cadena= strdup(yytext);
          return TAN;
        }
{LOG} {
          yylval.cadena= strdup(yytext);
          return LOG;
        }
{SQRT} {
          yylval.cadena= strdup(yytext);
          return SQRT;
        }
{EXP} {
          yylval.cadena= strdup(yytext);
          return EXP;
        }

{VARIABLE} { if(comprobar_tamanho()){

                          yylval.cadena= strdup(yytext);
                          lex.clave=yylval.cadena;
                          if(buscarTabla(&lex)){

                              return lex.valor;
                          }
                          else{

                                return VAR;
                          }

                    }
        }
{FICHERO} { if(comprobar_tamanho()){
                          yylval.cadena= strdup(yytext);
                          return FICHERO;
                    }}
{LIBRERIA} { if(comprobar_tamanho()){
                          yylval.cadena= strdup(yytext);
                          return LIBRERIA;
                    }}






. errorLexema(lineas,yytext); /*Si no es ninguno de los anteriores es un error*/


%%


/*Para liberar la memoria del lexema*/
void liberarLexema(){
    lex.valor=0;
    if(lex.clave!=NULL){
        free(lex.clave);
        lex.clave=NULL;
    }

}


