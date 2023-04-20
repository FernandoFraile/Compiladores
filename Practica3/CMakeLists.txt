cmake_minimum_required(VERSION 3.24)
project(Practica3 C)

set(CMAKE_C_STANDARD 11)

add_executable(Practica3 main.c tabla_hash_recolocacion.c TablaSimbolos.c bison.tab.c errores.c lex.yy.c tabla_hash_recolocacion.c calculadora.c miLibreria.c)

target_link_libraries(Practica3 PUBLIC m)
