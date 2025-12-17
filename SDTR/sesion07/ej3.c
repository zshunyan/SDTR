#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define NUM_THREADS 3
#define NUM_VALUES 10
void *compute_sine(void *threadid)
{
    for (int i = 0; i < NUM_VALUES; i++)
    {
        printf("Seno %d: %f\n", i, sin(i));
    }
    pthread_exit(NULL);
}
void *compute_cosine(void *threadid)
{
    for (int i = 0; i < NUM_VALUES; i++)
    {
        printf("Coseno %d: %f\n", i, cos(i));
    }
    pthread_exit(NULL);
}
void *compute_log(void *threadid)
{
    for (int i = 1; i <= NUM_VALUES; i++)
    { // Log 0 error
        printf("Log %d: %f\n", i, log(i));
    }
    pthread_exit(NULL);
}
int main()
{
    pthread_t threads[NUM_THREADS];
    int rc;

    // Crear los hilos y asignar las funciones
    rc = pthread_create(&threads[0], NULL, compute_sine, NULL);
    if (rc)
    {
        printf("Error al crear el hilo para seno\n");
        exit(-1);
    }

    rc = pthread_create(&threads[1], NULL, compute_cosine, NULL);
    if (rc)
    {
        printf("Error al crear el hilo para coseno\n");
        exit(-1);
    }

    rc = pthread_create(&threads[2], NULL, compute_log, NULL);
    if (rc)
    {
        printf("Error al crear el hilo para logaritmo\n");
        exit(-1);
    }

    // Esperar a que los hilos terminen
    for (int t = 0; t < NUM_THREADS; t++)
    {
        pthread_join(threads[t], NULL);
    }

    printf("Todos los cálculos han sido completados.\n");
    return 0;
}