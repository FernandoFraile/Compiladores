Intrucciones de compilacion y ejecucion: 

Es necesario tener todos los archivos en el mismo directorio (main, cabeceras, codigo fuente y regression.d). Para compilarlo se proporciona un Makefile, 
por lo que solo es necesario ejecutar el comando 'make'. Cuando esté compilado, ejecutar con el comando './ejecutable regression.d'. Si se desea 
ejecutar con otro archivo de entrada, ejeutar como './ejecutable <nombre_archivo>'

Qué se va a mostra al ejecutarse: 

Primero se mostrará la tabla de símbolos después de iniciarse. Posteriormente, se mostrará el resultado del análisis léxico, mostrando el par lexema y 
componente léxico de la forma <Componente_léxico,Lexema>. Finalmente se vuelve a imprimir la tabla de símbolos para ver como ha quedado. 

Tabla de símbolos: 

Se ha utilizado una tabla Hash para la realización de la tabla de símbolos. Se ha considerado que, teniendo en cuenta que el tamaño es menos del doble que los componentes léxicos almacenados y que se producen solo 2 colisiones en la inserción, las ventajas que aporta el rápido acceso a los elementos es favorable en cuanto a la elección de esta estructura con respecto a otra. 
