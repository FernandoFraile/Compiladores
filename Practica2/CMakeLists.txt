cmake_minimum_required(VERSION 3.24)
project(Practica2 C)

set(CMAKE_C_STANDARD 11)

add_executable(Practica2 main.c lex.yy.c tabla_hash_recolocacion.c
        TablaSimbolos.c errores.c errores.h analizadorSintactico.c analizadorSintactico.h)
