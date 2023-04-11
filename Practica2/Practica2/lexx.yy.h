//
// Created by fernando on 23/03/23.
//

#ifndef PRACTICA2_LEXX_YY_H
#define PRACTICA2_LEXX_YY_H

extern FILE *yyin;

int yylex(void);
CompLexico siguiente_componente_lexico();
void liberarLexema();
int obtenerLineas();
void yylex_destroy();


#endif //PRACTICA2_LEXX_YY_H
