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
%token <cadena> VAR CONSTANTE COMANDO FICHERO LIBRERIA FUNCION ELIM;
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
    | input line { if(fichero==false){ printf(">>");} }
;
line: '\n'
    | lib '\n'
    | lib ';' '\n' { if(err==false && fichero==false){ printf("%lf \n", $1);} err = false;}
    | exp ';' '\n' { if(err==false){ printf(">>%lf \n", $1);} err = false;}
    | exp '\n'
    | igualdad ';' '\n' { if(err==false && fichero==false){ printf("%lf \n", $1);} err = false;}
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
                        if(isnan($3)){
                            errorSistema("No se puede asignar un valor no numerico\n");
                        }
                        else{
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
                        }

                        free($1);
                    }
            | VAR '=' lib{

                        if(isnan($3)){
                            errorSistema("No se puede asignar un valor no numerico\n");
                        }
                        else{
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
                        }
                        free($1);
            }
            | CONSTANTE '=' exp {
                errorSistema("No se puede modificar una constante\n");
                err = true;
            }


;

comando: COMANDO {
            Complex.clave = $1;
            buscarTabla(&Complex);
            if((strcmp(Complex.clave, "import") == 0) || (strcmp(Complex.clave, "cargarFichero") == 0)
                ||  (strcmp(Complex.clave, "cerrarLibreria") == 0)){
                errorSistema("Debe indicarse un argumento para la funcion\n");
                free($1);

            }
            else{
                FuncPtr func=Complex.funcion;
                free($1);

                func();
            }




          }

        | COMANDO '(' ')' {
              Complex.clave = $1;
              if( buscarTabla(&Complex)){

                if( (strcmp(Complex.clave, "import") == 0) || (strcmp(Complex.clave, "cargarFichero") == 0)
                     ||  (strcmp(Complex.clave, "cerrarLibreria") == 0)){
                        errorSistema("Debe indicarse un argumento para la funcion\n");
                        free($1);

                    }
                else {
                    FuncPtr func=Complex.funcion;

                    if(func!=NULL){
                        free($1);
                        func();
                    }
                }
              }else{
                  free($1);

                 errorSistema("Error interno\n");

              }


            }

        | COMANDO '(' FICHERO ')' {
                      Complex.clave = $1;
                      buscarTabla(&Complex);
                      FuncPtr func=Complex.funcion;
                      free($1);
                      func($3);

        }

        | COMANDO '(' LIBRERIA ')' {
                      Complex.clave = $1;
                      buscarTabla(&Complex);
                      FuncPtr func=Complex.funcion;
                      free($1);
                      func($3);
                      free($3);

                      }
        | COMANDO '(' VAR ')'{
                  Complex.clave = $1;
                  if(buscarTabla(&Complex)){
                        FuncPtr func=Complex.funcion;
                        free($1);
                        func($3);
                        free($3);

                  }
        }
        | COMANDO '(' CONSTANTE ')'{
            free($1);
            free($3);
            errorSistema("No se puede invocar a la funcion pedida con una constante\n");

        }

        | ELIM '('  ')' {
                errorSistema("Debe indicarse una variable para eliminar\n");
        }
        | ELIM '(' VAR ')' {
              Complex.clave = $3;
              CompLexico aux;
              if(buscarTabla(&Complex)){

                    aux.clave = $1;
                    buscarTabla(&aux);
                    FuncPtr func=aux.funcion;
                    free($1);
                    func($3);
                    free($3);

              }
              else{
                  free($3);
                  free($1);
                  errorSistema("No se ha encontrado la variable indicada\n");

              }
        }

;

lib: VAR '(' exp ')' {

                if(funcionLibDinamica($1)){
                    Complex.clave = $1;
                    buscarTabla(&Complex);
                    free($1);
                    double (*func)(double) = (double (*)(double))Complex.funcion;
                    double result = (*func)((double)$3);
                    $$ = result;

                }
                else{
                    free($1);

                     errorSistema("No se ha encontrado la funcion indicada\n");

                }
    }
    | VAR '('  ')' {
            if(funcionLibDinamica($1)){
                Complex.clave = $1;
                buscarTabla(&Complex);
                free($1);
                FuncPtr func=Complex.funcion;
                func();

            }
             else{
                 free($1);

                 errorSistema("No se ha encontrado la funcion indicada\n");

            }
    }
    | FUNCION '(' exp ')' {
            Complex.clave = $1;
            if(buscarTabla(&Complex)){
                    free($1);
                    double (*func)(double) = (double (*)(double))Complex.funcion;
                    double result = (*func)((double)$3);
                    $$ = result;
            }

    }

    | FUNCION '(' ')' {
                Complex.clave = $1;
                if( buscarTabla(&Complex)){
                    free($1);
                    FuncPtr func=Complex.funcion;
                    func();
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








