#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 2
#define VECTOR_SIZE 10

int vector[VECTOR_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int partial_sums[NUM_THREADS];

void *sum_segment(void *arg) {
    int thread_id = *(int *)arg;
    int segment_size = VECTOR_SIZE / NUM_THREADS;
    int start = thread_id * segment_size;
    int end = start + segment_size;

    partial_sums[thread_id] = 0;
    for (int i = start; i < end; i++) {
        partial_sums[thread_id] += vector[i];
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    int total_sum = 0;

    // Mostrar el vector
    printf("Vector: ");
    for (int i = 0; i < VECTOR_SIZE; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");

    // Crear los hilos para calcular las sumas parciales
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, sum_segment, &thread_ids[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Sumar los resultados parciales
    for (int i = 0; i < NUM_THREADS; i++) {
        total_sum += partial_sums[i];
    }

    // Mostrar el resultado total
    printf("La suma total del vector es: %d\n", total_sum);

    return 0;
}
