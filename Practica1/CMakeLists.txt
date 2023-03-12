cmake_minimum_required(VERSION 3.24)
project(Practica1 C)

set(CMAKE_C_STANDARD 11)

add_executable(Practica1 main.c analizadorLexico.c tabla_hash_recolocacion.c TablaSimbolos.c errores.c errores.h analizadorSintactico.c analizadorSintactico.h sistemaEntrada.c sistemaEntrada.h)
