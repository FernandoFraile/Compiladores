CC=gcc -Wall
#carpeta de las cabeceras (SI ESTÁN EN LA ACTUAL, PONEMOS .)
HEADER_FILES_DIR = .
#opciones de compilación, indica dónde están los archivos .h
INCLUDES = -I $(HEADER_FILES_DIR)
#nombre del ejecutable o archivo de salida
OUTPUT = ejecutable
#ficheros .h. Si hay varios, se enumeran con su path completo
LIB_HEADERS = $(HEADER_FILES_DIR)/analizadorSintactico $(HEADER_FILES_DIR)/analizadorLexico.h $(HEADER_FILES_DIR)/tabla_hash_recolocacion.h $(HEADER_FILES_DIR)/definiciones.h $(HEADER_FILES_DIR)/errores.h $(HEADER_FILES_DIR)/sistemaEntrada.h $(HEADER_FILES_DIR)/TablaSimbolos.h
#fuentes
SRCS = main.c analizadorLexico.c analizadorSintactico.c errores.c sistemaEntrada.c TablaSimbolos.c tabla_hash_recolocacion.c
#ficheros .o: todos los .o con un análogo .c en SRCS
OBJS = $(SRCS:.c=.o)
#regla 1 (dependencia de los .o)
#si no hay librerías adicionales, no existe la variable $(LIBS),
#por lo que se elimina $(LIBS) de la regla siguiente
$(OUTPUT): $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS)
#regla 2 (genera los .o cuando es necesario, dependencia de los .c y .h)
#$@ es el nombre del fichero que se genera con la regla (.o)
#$< es el nombre del primer prerrequisito (el archivo .c cuyo .o se está generando)
%.o: %.c $(LIB_HEADERS)
	$(CC) -c -o $@ $< $(INCLUDES)
#regla 3 que borra el ejecutable (prerrequisito: clean)
cleanall: clean
	rm -f $(OUTPUT)
#regla 4 que borra los ficheros .o y los de backup (terminan en ~)
clean:
	rm -f *.o *~
