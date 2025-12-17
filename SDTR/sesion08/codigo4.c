#include <stdio.h>
#include <unistd.h>
#include <time.h> 
int main()
{
    struct timespec next, period;
    if (clock_gettime(CLOCK_MONOTONIC, &next) != 0)
        return -1;

    period.tv_sec = 0;
    period.tv_nsec = 10.0E6; /* 10 ms */

    while (1)
    {
        if (clock_nanosleep(CLOCK_MONOTONIC,
                            TIMER_ABSTIME,
                            &next, 0) != 0)
            return -1;

        // Imprimir los valores de next
        printf("next: %ld segundos, %ld nanosegundos\n", next.tv_sec, next.tv_nsec);

        // Actualizar next sumando el periodo
        next.tv_sec += period.tv_sec;
        next.tv_nsec += period.tv_nsec;

        // Ajustar los nanosegundos si exceden 1 segundo
        if (next.tv_nsec >= 1E9)
        {
            next.tv_sec += 1;
            next.tv_nsec -= 1E9;
        }
    }
    return 0;
}