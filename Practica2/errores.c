#include <stdlib.h>
#include "stdio.h"

#include "errores.h"

void errorSistema(char *error){
    printf("Error: %s",error);
    exit(EXIT_FAILURE);
}

void errorLexicoTamanho(int linea,char *palabra){
    printf("Error: el lexema %s supera el tamaño máximo en la linea %d\n",palabra,linea);
}

void errorLexicoComentario(int linea,char *tipo){
    printf("Error: el comentario %s no esta bien formado en la linea %d\n",tipo,linea);
}

void errorLexicoString(int linea,char *frase){
    printf("Error en la linea %d: %s\n",linea,frase);
}
