//
// Created by fernando on 18/04/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define Nmax 600
void producto(float x, float y, float *z) {
    *z=x*y; }

int main() {

    struct timeval inicio2, inicio1, final;
    double tiempo,overhead;
    long double x=0;
    gettimeofday(&inicio1,NULL);
    gettimeofday(&inicio2,NULL);


    float A[Nmax][Nmax], B[Nmax][Nmax], C[Nmax][Nmax], t, r;
    int i,j,k;
    for(i=0;i<Nmax;i++) /* Valores de las matrices */
        for(j=0;j<Nmax;j++) {
            A[i][j]=(i+j)/(j+1.1);
            B[i][j]=(i-j)/(j+2.1); }
    for(i=0;i<Nmax;i++) /* Producto matricial */
        for(j=0;j<Nmax;j++) {
            t=0;
            for (k=0;k<Nmax;k++) {
                producto(A[i][k],B[k][j],&r);
                t+=r; }
            C[i][j]=t; }


    gettimeofday(&final,NULL);
    overhead = (inicio2.tv_sec-inicio1.tv_sec+(inicio2.tv_usec-inicio1.tv_usec)/1.e6);

    tiempo = (final.tv_sec-inicio1.tv_sec+(final.tv_usec-inicio1.tv_usec)/1.e6);

    tiempo = tiempo - overhead;
    printf("Tiempo: %lf\n", tiempo);

    return EXIT_SUCCESS;
}