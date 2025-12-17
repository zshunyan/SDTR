#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *thread_function(void *arg) {
    // Variables para establecer el tipo de cancelación y habilitacion
    int type, oldtype;
    int enabling, oldenabling;

    // Establecer canceltype del hilo
    // 1.1: para que sea asíncrono con PTHREAD_CANCEL_ASYNCHRONOUS
    type = PTHREAD_CANCEL_ASYNCHRONOUS;
    pthread_setcanceltype(type, &oldtype);

    // Habilitamos o no la cancelación del hilo
    // 2.1: la habilitamos con PTHREAD_CANCEL_ENABLE
    enabling = PTHREAD_CANCEL_ENABLE;
    pthread_setcancelstate(enabling, &oldenabling);

    printf("Hilo configurado con tipo %d y habilitacion %d\n", type, enabling);

    // Bucle infinito para simular un trabajo
    while (1) {
        printf("Hilo trabajando...\n");
        sleep(1); // Simulamos un trabajo
    }

    return NULL;
}

int main() {
    pthread_t thread;
    void *result;

    // Crear un hilo
    pthread_create(&thread, NULL, thread_function, NULL);

    // Damos tiempo al hilo para que empiece y muestre algunos mensajes
    sleep(3);

    // Solicitamos la cancelación del hilo
    printf("Cancelando el hilo...\n");
    pthread_cancel(thread);

    // Esperamos a que el hilo responda a la cancelación con join
    pthread_join(thread, &result);

    if (result == PTHREAD_CANCELED)
        printf("El hilo fue cancelado %p\n", result);
    else
        printf("El hilo no fue cancelado correctamente %p\n", result);

    return 0;
}