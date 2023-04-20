#include <stdio.h>
#include <sys/time.h>

int main() {
    struct timeval inicio2, inicio1, final;
    double tiempo,overhead;
    long double x=0;
    gettimeofday(&inicio1,NULL);
    gettimeofday(&inicio2,NULL);
    /*
    for(int i=0;i<10000000;i++){
        x=x+i; //Para la parte de el ejemplo con muchas operaciones aritmeticas
    }*/

    for(int i=0;i<10000000;i++){
        printf("EY"); //De algo que tenga que ir al disco para el ejemplo de entrada/salida
    }
    gettimeofday(&final,NULL);
    overhead = (inicio2.tv_sec-inicio1.tv_sec+(inicio2.tv_usec-inicio1.tv_usec)/1.e6);

    tiempo = (final.tv_sec-inicio1.tv_sec+(final.tv_usec-inicio1.tv_usec)/1.e6);

    tiempo = tiempo - overhead;
    printf("Tiempo: %lf\n", tiempo);
    return 0;

}
