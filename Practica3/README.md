"Uso: ./ejecutable <nombre_archivo> . <nombre_archivo es opcional>\n"
           "Si no se indica el nombre del archivo se ejecuta en modo interactivo\n"
           "El archivo puede contener comentarios de una linea (//) o de bloque (/**/)\n\n"
           "Realiza operaciones aritmeticas y asignaciones de variables como: "
           "+,-,^,/,*,sin,cos,tan,log,exp,sqrt\n"
           "La calculadora mostrará el resultado de las operaciones cuando se ejecute un fichero, o se indique un "
           "';' al final de cada instruccion, cuando se ejecute el modo interactivo\n"
           "\n\n"
           "COMANDOS DE LA CALCULADORA:\n"
           "-ayuda: Muestra la ayuda de la calculadora\n"
           "-salir: Sale de la calculadora\n"
           "-cargarFichero(fichero): Carga el fichero indicado. Una vez a acaba de leerlo vuelve a modo interactivo\n"
           "-workSpace: Muestra el workspace de la calculadora\n"
           "-eliminarWorkspace: Elimina el workspace de la calculadora\n"
           "-eliminarVariable(variable): Elimina la variable indicada\n"
           "-import(libreria): Importa una libreria dinamicamente. Se debe indicar el nombre de la libreria\n"
           "\tLa librería matemática ya está incluida estáticamente para poder realizar las funcione mínimas indicadas"
           "previamente. \nPara ejecutar librerías creadas, hay que situar el arhcivo .so en la carpeta en la que busca dlopen (se puede "
           "ver ejecutando el comando ldd ejecutable, viendo donde están las bibliotecas compartidas que dependen del ejecutable)"
           ", o se puede indicar en la variable LD_LIBRARY_PATH el directorio donde se encuentran las librerías dinámicas. También se puede indicar"
           "directamente en el comando la ruta completa, import(/ruta_completa_al_archivo/libreria.so)\n"
           "-cerrarLibreria(libreria): Cierra la librería indicada. Se debe indicar el nombre de la librería, sin extensiones \n"