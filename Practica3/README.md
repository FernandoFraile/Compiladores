Esta es la implementación de un intérprete matemático, realizado en código C, utilizando las herramientas flex y bison. 

COMPILACIÓN Y EJECUCIÓN: 

Se aporta un archivo Makefile para compilar el proyecto. Solo habrá que ejecutar el comando "make", en la carpeta donde se sitúen todos los archivos que se aportan en este proyecto. 

Uso: ./ejecutable <nombre_archivo> . El <nombre_archivo> es opcional
Si no se indica el nombre del archivo se ejecuta en modo interactivo, utilizando la terminal como entrada. 
El archivo puede contener comentarios de una linea (//) o de bloque (/*\*/) 
Realiza operaciones aritmeticas y asignaciones de variables como: 
+,-,^,/,*,sin,cos,tan,log,exp,sqrt
La calculadora mostrará el resultado de las operaciones cuando se ejecute un fichero, o se indique un 
';' al final de cada instruccion, cuando se ejecute el modo interactivo. 
Permitirá cargar librerías dinámicamente, preinstaladas en el sistema operativo, o creadas por el usuario (leer la explicación de el comdando import() para más detalles. 


COMANDOS DE LA CALCULADORA:


-ayuda: Muestra la ayuda de la calculadora
-salir: Sale de la calculadora
-cargarFichero(fichero): Carga el fichero indicado. Una vez a acaba de leerlo vuelve a modo interactivo
-workSpace: Muestra el workspace de la calculadora
-eliminarWorkspace: Elimina el workspace de la calculadora
-eliminarVariable(variable): Elimina la variable indicada
-import(libreria): Importa una libreria dinamicamente. Se debe indicar el nombre de la libreria
La librería matemática ya está incluida estáticamente para poder realizar las funcione mínimas indicadas
previamente. \nPara ejecutar librerías creadas, hay que situar el arhcivo .so en la carpeta en la que busca dlopen (se puede 
ver ejecutando el comando ldd ejecutable, viendo donde están las bibliotecas compartidas que dependen del ejecutable)
, o se puede indicar en la variable LD_LIBRARY_PATH el directorio donde se encuentran las librerías dinámicas. También se puede indicar
directamente en el comando la ruta completa, import(/ruta_completa_al_archivo/libreria.so)
-cerrarLibreria(libreria): Cierra la librería indicada. Se debe indicar el nombre de la librería, sin extensiones
