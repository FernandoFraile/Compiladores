
//Gestion de errores producidos en todo el proceso, como error al abrir archivos, liberar o reservar memoria...
void errorSistema(char *error);

//Gestion de errores lexicos por tamanho de lexema
void errorLexicoTamanho(int linea,char *palabra);

//Gestion de errores por comentarios mal formados

void errorLexicoComentario(int linea,char *tipo);

//Error para lexemas no reconocidos
void errorLexema(int linea,char *lexema);


