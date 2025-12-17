#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h> // For exit

void *funcion_tarea(void *argp)
{
    unsigned long hiloID = (unsigned long)argp;

    // Mostrar el mensaje: “Tarea iniciada. Hilo $ID está corriendo”
    printf("Tarea iniciada. Hilo %lu está corriendo\n", hiloID);

    // Esperar 2 segundos
    sleep(2);

    // Mostrar el mensaje: “Tarea del hilo $ID va a finalizar ahora”
    printf("Tarea del hilo %lu va a finalizar ahora\n", hiloID);

    // Usar la función exit de los hilos, devolviendo el ID del hilo
    pthread_exit((void *)hiloID);
}

int main()
{
    unsigned long i;
    pthread_t tarea;

    // Crear la tarea con pthread_create
    pthread_create(&tarea, NULL, funcion_tarea, (void *)pthread_self());

    // Esperar que la tarea complete y capturar el valor devuelto
    void *retorno;
    pthread_join(tarea, &retorno);

    // Mostrar el valor devuelto por el hilo
    i = (unsigned long)retorno;
    printf("El hilo devolvió: %lu\n", i);

    return 0;
}
