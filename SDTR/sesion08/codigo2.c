#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main()
{
    struct timespec ts_realtimeSTART, ts_realtimeEND,
        ts_monotonic_START, ts_monotonic_END,
        ts_process_START, ts_process_END,
        ts_thread_START, ts_thread_END,
        res_realtime, res_monotonic, res_process, res_thread;

    // Consultar la resolución de CLOCK_REALTIME
    if (clock_getres(CLOCK_REALTIME, &res_realtime) == -1)
    {
        perror("Error al obtener resolución de CLOCK_REALTIME");
        return 1;
    }
    printf("Resolución de CLOCK_REALTIME: %ld segundos, %ld nanosegundos\n",
           res_realtime.tv_sec, res_realtime.tv_nsec);

    // Consultar la resolución de CLOCK_MONOTONIC
    if (clock_getres(CLOCK_MONOTONIC, &res_monotonic) == -1)
    {
        perror("Error al obtener resolución de CLOCK_MONOTONIC");
        return 1;
    }
    printf("Resolución de CLOCK_MONOTONIC: %ld segundos, %ld nanosegundos\n",
           res_monotonic.tv_sec, res_monotonic.tv_nsec);

    // Consultar la resolución de CLOCK_PROCESS_CPUTIME_ID
    if (clock_getres(CLOCK_PROCESS_CPUTIME_ID, &res_process) == -1)
    {
        perror("Error al obtener resolución de CLOCK_PROCESS_CPUTIME_ID");
        return 1;
    }
    printf("Resolución de CLOCK_PROCESS_CPUTIME_ID: %ld segundos, %ld nanosegundos\n",
           res_process.tv_sec, res_process.tv_nsec);

    // Consultar la resolución de CLOCK_THREAD_CPUTIME_ID
    if (clock_getres(CLOCK_THREAD_CPUTIME_ID, &res_thread) == -1)
    {
        perror("Error al obtener resolución de CLOCK_THREAD_CPUTIME_ID");
        return 1;
    }
    printf("Resolución de CLOCK_THREAD_CPUTIME_ID: %ld segundos, %ld nanosegundos\n",
           res_thread.tv_sec, res_thread.tv_nsec);

    // Consultar el reloj CLOCK_REALTIME (START)
    if (clock_gettime(CLOCK_REALTIME, &ts_realtimeSTART) == -1)
    {
        perror("Error con CLOCK_REALTIME [1]");
        return 1;
    }

    // Consultar el reloj CLOCK_MONOTONIC (START)
    if (clock_gettime(CLOCK_MONOTONIC, &ts_monotonic_START) == -1)
    {
        perror("Error con CLOCK_MONOTONIC [1]");
        return 1;
    }

    // Consultar el reloj CLOCK_PROCESS_CPUTIME_ID (START)
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts_process_START) == -1)
    {
        perror("Error con CLOCK_PROCESS_CPUTIME_ID [1]");
        return 1;
    }

    // Consultar el reloj CLOCK_THREAD_CPUTIME_ID (START)
    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts_thread_START) == -1)
    {
        perror("Error con CLOCK_THREAD_CPUTIME_ID [1]");
        return 1;
    }

    // Reemplazar el bucle por sleep(1)
    sleep(1);

    // Consultar el reloj CLOCK_REALTIME (END)
    if (clock_gettime(CLOCK_REALTIME, &ts_realtimeEND) == -1)
    {
        perror("Error con CLOCK_REALTIME [2]");
        return 1;
    }

    // Consultar el reloj CLOCK_MONOTONIC (END)
    if (clock_gettime(CLOCK_MONOTONIC, &ts_monotonic_END) == -1)
    {
        perror("Error con CLOCK_MONOTONIC [2]");
        return 1;
    }

    // Consultar el reloj CLOCK_PROCESS_CPUTIME_ID (END)
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts_process_END) == -1)
    {
        perror("Error con CLOCK_PROCESS_CPUTIME_ID [2]");
        return 1;
    }

    // Consultar el reloj CLOCK_THREAD_CPUTIME_ID (END)
    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts_thread_END) == -1)
    {
        perror("Error con CLOCK_THREAD_CPUTIME_ID [2]");
        return 1;
    }

    // Mostrar los tiempos obtenidos
    printf("Tiempo ejecución CLOCK_REALTIME: %ld segundos, %ld nanosegundos\n",
           ts_realtimeEND.tv_sec - ts_realtimeSTART.tv_sec,
           ts_realtimeEND.tv_nsec - ts_realtimeSTART.tv_nsec);

    printf("Tiempo ejecución CLOCK_MONOTONIC: %ld segundos, %ld nanosegundos\n",
           ts_monotonic_END.tv_sec - ts_monotonic_START.tv_sec,
           ts_monotonic_END.tv_nsec - ts_monotonic_START.tv_nsec);

    printf("Tiempo ejecución CLOCK_PROCESS_CPUTIME_ID: %ld segundos, %ld nanosegundos\n",
           ts_process_END.tv_sec - ts_process_START.tv_sec,
           ts_process_END.tv_nsec - ts_process_START.tv_nsec);

    printf("Tiempo ejecución CLOCK_THREAD_CPUTIME_ID: %ld segundos, %ld nanosegundos\n",
           ts_thread_END.tv_sec - ts_thread_START.tv_sec,
           ts_thread_END.tv_nsec - ts_thread_START.tv_nsec);

    return 0;
}
/*
¿Qué códigos están asignados a cada reloj?
    CLOCK_REALTIME: Tiempo real del sistema.
    CLOCK_MONOTONIC: Tiempo monotónico desde un punto fijo.
    CLOCK_PROCESS_CPUTIME_ID: Tiempo de CPU consumido por el proceso.
    CLOCK_THREAD_CPUTIME_ID: Tiempo de CPU consumido por el hilo.

¿Qué significa el primer tiempo obtenido por el CLOCK_REALTIME?
    Representa el tiempo actual desde el Epoch (1 de enero de 1970). Convertido a días y años, es el tiempo

¿Qué significa el primer tiempo obtenido por el CLOCK_MONOTONIC? Pásalo a
horas.
    Representa el tiempo transcurrido desde un punto fijo. Convertido a horas:
Compara los tiempos de ejecución obtenidos por cada reloj. ¿Son iguales? ¿Qué
conclusiones extraes?
    No son iguales. CLOCK_REALTIME puede verse afectado por cambios en el sistema, mientras que CLOCK_MONOTONIC es continuo y no se ve afectado por ajustes del sistema. Esto significa que CLOCK_MONOTONIC es más adecuado para medir intervalos de tiempo.
    CLOCK_PROCESS_CPUTIME_ID y CLOCK_THREAD_CPUTIME_ID miden el tiempo de CPU utilizado por el proceso y el hilo, respectivamente, y pueden ser diferentes de los otros dos.
    CLOCK_PROCESS_CPUTIME_ID mide el tiempo de CPU utilizado por el proceso, mientras que CLOCK_THREAD_CPUTIME_ID mide el tiempo de CPU utilizado por el hilo. Estos dos pueden ser diferentes entre sí y de los otros relojes, ya que dependen de la carga de trabajo del proceso y del hilo en particular.
*/