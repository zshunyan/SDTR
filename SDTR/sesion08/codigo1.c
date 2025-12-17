/*Un tick es la mínima unidad de tiempo del sistema.Este valor es útil para calcular la
    duración de intervalos de tiempo en sistemas POSIX.Mediante
    sysconf(_SC_CLK_TCK)
se puede consultar el número de ticks por segundo, i.e., la resolución de tiempo del sistema : CÓDIGO 1
*/
#include <stdio.h>
#include <unistd.h>
int main()
{
    long ticks_per_second;
    // Consultar el número de ticks por segundo
    if ((ticks_per_second = sysconf(_SC_CLK_TCK)) == -1)
    {
        perror("Error al obtener el número de ticks por segundo");
        return 1;
    }
    printf("Número de ticks por segundo: %ld\n", ticks_per_second);

    // Preguntas:
    // 1. ¿Cuántos ticks por segundo tiene tu sistema?
    //    - Este valor depende del sistema operativo. En la mayoría de los sistemas POSIX modernos, suele ser 100 o 1000.
    //
    // 2. ¿Cuál es la relación entre el número de ticks por segundo y la resolución de tiempo del sistema?
    //    - El número de ticks por segundo define la resolución mínima de tiempo que el sistema puede medir.
    //      Por ejemplo, si hay 100 ticks por segundo, la resolución mínima es de 10 ms.
    //
    // 3. ¿Cuál es el impacto de cambiar el valor devuelto por sysconf(_SC_CLK_TCK) en un sistema POSIX?
    //    - Cambiar este valor afectaría la granularidad de las mediciones de tiempo y podría impactar el rendimiento
    //      de aplicaciones sensibles al tiempo, como sistemas en tiempo real.
    //
    // 4. ¿Está relacionado con el reloj de la CPU?
    //    - Indirectamente. Aunque los ticks no están directamente relacionados con la frecuencia de la CPU,
    //      están vinculados al temporizador del sistema, que puede depender del hardware subyacente.

    return 0;
}