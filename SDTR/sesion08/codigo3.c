#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int main(void)
{
    struct timespec retardo, retardo_pend;
    int res;

    // Preparar un retardo relativo de 2.550 s con retardo
    retardo.tv_sec = 2;
    retardo.tv_nsec = 150E6; // 150 ms
    printf("Voy a esperar %ld.%09ld s ......\n",
           retardo.tv_sec, retardo.tv_nsec);

    // Usar nanosleep para realizar el retardo
    if ((res = nanosleep(&retardo, &retardo_pend)) == -1)
        printf("He despertado por una señal, Retardo pendiente: %ld s %ld nsec\n",
               retardo_pend.tv_sec, retardo_pend.tv_nsec);
    else
        printf("He despertado y ya termino.\n");

    exit(0);
}