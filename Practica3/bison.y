/* Calculadora de notaci´on infija--calc */
%{
#include <math.h>


#include "lex.yy.h"
#include "errores.h"
#include "TablaSimbolos.h"


bool fichero = true;
CompLexico Complex;


void yyerror (char *s);

%}
%union {
    double variable;
    char *cadena;
}

%token <variable> NUMERO;
%token <cadena> VAR CONSTANTE COMANDO;
%type <variable> exp igualdad;

/* Declaraciones de BISON */
%left '+' '-'
%left '*' '/'
%left NEG /* negaci´on--menos unario */
%right '^' /* exponenciaci´on */



%%
input: /* cadena vacia */
    | input line
;
line: '\n'
    | exp ';' '\n' { if(fichero) {printf("%lf \n", $1);}}
    | exp '\n' { if(fichero) {printf("%lf \n", $1);}}
    | igualdad ';' '\n' { if(fichero) {printf("%lf \n", $1);}}
    | igualdad '\n' { if(fichero) {printf("%lf \n", $1);}}
    | comando ';' '\n'
    | comando '\n'





;
exp: NUMERO
    | VAR {
            Complex.clave = $1;
            buscarTabla(&Complex);
            if(Complex.valor>0){
                $$ = Complex.variable;
            }else{
                errorSistema("Variable no inicializada\n");
            }

          }
    | CONSTANTE {
            Complex.clave = $1;
            buscarTabla(&Complex);
            $$ = Complex.variable;

          }

    | exp '+' exp { $$ = $1 + $3;}
    | exp '-' exp { $$ = $1 - $3; }
    | exp '*' exp { $$ = $1 * $3; }
    | exp '/' exp { $$ = $1 / $3; }
    | exp '^' exp { $$ = pow($1, $3); }
;

igualdad: VAR '=' exp {
                        Complex.clave = strdup($1);
                        Complex.variable = $3;
                        Complex.valor = VAR;
                        insertarTabla(&Complex);
                        $$ = $3;
                        free(Complex.clave);
                    }
;

comando: COMANDO {
            printf("EEEE\n");
            Complex.clave = strdup($1);
            buscarTabla(&Complex);
            printf("%s\n",Complex.clave);

          }






%%

void yyerror(char *s) {
    errorSistema("Error de sintaxis\n");
}

void ejecutarFichero(bool opcion){
    fichero = opcion;
}





