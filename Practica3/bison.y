/* Calculadora de notaci´on infija--calc */
%{
#include <math.h>


#include "lex.yy.h"
#include "errores.h"
#include "TablaSimbolos.h"
#include "calculadora.h"


bool fichero = false;
CompLexico Complex;
void (*funciones[])() = { };
bool err = false;


void yyerror (char *s);


%}
%union {
    double variable;
    char *cadena;
}

%token <variable> NUMERO;
%token <cadena> VAR CONSTANTE COMANDO FICHERO LIBRERIA FUNCION;
%token <cadena> SIN COS TAN LOG SQRT EXP;
%type <variable> exp igualdad lib;

%type <cadena> comando;







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
    | lib '\n'
    | lib ';' '\n'
    | exp ';' '\n' { if(!err) printf("%lf \n", $1);}
    | exp '\n' { if(!err) printf("%lf \n", $1);}
    | igualdad ';' '\n'
    | igualdad '\n'
    | comando ';' '\n'
    | comando '\n'
    | error '\n' {yyerrok; }



;
exp: NUMERO
    | VAR {
            Complex.clave = $1;

            if(buscarTabla(&Complex)){
                $$ = Complex.variable;
            }else{
                errorSistema("Variable no inicializada\n");
                err = true;
            }
            free($1);

          }
    | CONSTANTE {
            Complex.clave = $1;
            buscarTabla(&Complex);
            $$ = Complex.variable;
            free($1);

          }

    | exp '+' exp { $$ = $1 + $3; }
    | exp '-' exp { $$ = $1 - $3; }
    | exp '*' exp { $$ = $1 * $3;  }
    | exp '/' exp { $$ = $1 / $3; }
    | exp '^' exp { $$ = pow($1, $3); }
    | '-' exp %prec NEG { $$ = -$2; }
    | SIN '(' exp ')' { $$ = sin($3);
                             free($1);
                        }
    | COS '(' exp ')' { $$ = cos($3); free($1);}
    | TAN '(' exp ')' { $$ = tan($3); free($1); }
    | LOG '(' exp ')' { $$ = log($3);  free($1);}
    | SQRT '(' exp ')' { $$ = sqrt($3);  free($1);}
    | EXP '(' exp ')' { $$ = exp($3);  free($1);}
    | '(' exp ')' { $$ = $2; }

;



igualdad: VAR '=' exp {
                        Complex.clave = $1;
                        Complex.variable = $3;
                        Complex.valor = VAR;
                        if(buscarTabla(&Complex)){
                            Complex.variable = $3;
                            modificarElementoTabla(Complex);
                        }else{
                            insertarTabla(&Complex);

                        }

                        $$ = $3;
                        if(fichero) printf("%s = %lf\n", $1, $3);
                        free($1);
                    }

;

comando: COMANDO {
            Complex.clave = $1;
            buscarTabla(&Complex);
            FuncPtr func=Complex.funcion;
            free($1);
            func();

          }

        | COMANDO '(' ')' {
              Complex.clave = $1;
              buscarTabla(&Complex);
              FuncPtr func=Complex.funcion;
              free($1);

              func();

            }

        | COMANDO '(' FICHERO ')' {
                      Complex.clave = $1;
                      buscarTabla(&Complex);
                      FuncPtr func=Complex.funcion;
                      free($1);
                      func($3);
                      free($3);



        }

        | COMANDO '(' LIBRERIA ')' {
                      Complex.clave = $1;
                      buscarTabla(&Complex);
                      FuncPtr func=Complex.funcion;
                      free($1);
                      func($3);
                      free($3);

                      }
        | COMANDO '(' exp ')'{
                 Complex.clave = $1;
                  if(buscarTabla(&Complex)){
                        FuncPtr func=Complex.funcion;
                        free($1);
                        func($3);

                  }
        }

;

lib: VAR '(' exp ')' {

                if(funcionLibDinamica($1)){
                    buscarTabla(&Complex);
                    FuncPtr func=Complex.funcion;
                    free($1);
                    printf("%f\n",$3);
                    printf("%f\n",func($3));

                }
    }




;









%%

void yyerror(char *s) {
    errorSistema("Error de sintaxis\n");
}

void ejecutarFichero(bool opcion){
    fichero = opcion;
}






