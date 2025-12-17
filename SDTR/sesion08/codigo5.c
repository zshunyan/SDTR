#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

void timer_handler(int sig)
{
    static int count = 0;
    printf("Timer expired %d times\n", ++count);
}

int main()
{
    timer_t timerid; // Declarar el temporizador
    struct sigevent sev; // Declarar el sigevent para notificar la expiración del temporizador
    struct itimerspec its; // Declarar el itimerspec para configurar el temporizador

    // Configurar la acción de la señal
    signal(SIGRTMIN, timer_handler);

    // Configurar la estructura sigevent para usar una señal en tiempo real: SIGRTMIN
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timerid;

    // Crear el temporizador con CLOCK_REALTIME
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1)
    {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    // Configurar el temporizador para expirar después de 3 segundos y cada 3 segundos repetidamente
    its.it_value.tv_sec = 3; // Primera expiración en 3 segundos
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 3; // Intervalo de repetición de 3 segundos
    its.it_interval.tv_nsec = 0;

    // Establecer el temporizador
    if (timer_settime(timerid, 0, &its, NULL) == -1)
    {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }

    // Pausa el programa y espera señales indefinidamente
    while (1)
    {
        pause(); // La función pause() suspende el programa hasta que llegue una señal
    }

    // Eliminar el temporizador (nunca se ejecutará en este caso)
    timer_delete(timerid);
    return 0;
}