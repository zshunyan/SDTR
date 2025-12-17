#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#include <string.h> // Para strerror

// Función auxiliar para manejar errores
void handle_error(const char *message) {
    fprintf(stderr, "%s: %s\n", message, strerror(errno));
    exit(EXIT_FAILURE);
}

int main() {
    // Mostrar los valores de las políticas de planificación
    printf("FIFO %d\n", SCHED_FIFO);
    printf("RR %d\n", SCHED_RR);
    printf("OTHER %d\n", SCHED_OTHER);

    struct sched_param param;
    int current_policy, new_policy;

    // Obtener la política actual y los parámetros del proceso
    current_policy = sched_getscheduler(0);
    if (current_policy == -1) {
        handle_error("Error obteniendo la política actual");
    }

    if (sched_getparam(0, &param) != 0) {
        handle_error("Error obteniendo la prioridad actual");
    }

    printf("Política actual: %d, Prioridad actual: %d\n", current_policy, param.sched_priority);

    // Mostrar valores de prioridad mínimo y máximo para cada política
    printf("Prioridad mínima SCHED_OTHER: %d\n", sched_get_priority_min(SCHED_OTHER));
    printf("Prioridad máxima SCHED_OTHER: %d\n", sched_get_priority_max(SCHED_OTHER));
    printf("Prioridad mínima SCHED_FIFO: %d\n", sched_get_priority_min(SCHED_FIFO));
    printf("Prioridad máxima SCHED_FIFO: %d\n", sched_get_priority_max(SCHED_FIFO));
    printf("Prioridad mínima SCHED_RR: %d\n", sched_get_priority_min(SCHED_RR));
    printf("Prioridad máxima SCHED_RR: %d\n", sched_get_priority_max(SCHED_RR));

    // Obtener y mostrar el quantum de tiempo para SCHED_RR
    struct timespec ts;
    if (sched_rr_get_interval(0, &ts) == 0) {
        printf("Quantum de tiempo para SCHED_RR: %ld.%09ld segundos\n", ts.tv_sec, ts.tv_nsec);
    } else {
        handle_error("Error obteniendo el quantum de tiempo para SCHED_RR");
    }

    // Intentar cambiar a SCHED_FIFO o SCHED_RR si SCHED_OTHER es la actual
    new_policy = (current_policy == SCHED_OTHER) ? SCHED_FIFO : SCHED_OTHER;
    param.sched_priority = sched_get_priority_max(new_policy);

    if (param.sched_priority == -1) {
        handle_error("Error obteniendo la prioridad máxima para la nueva política");
    }

    // Establecer la nueva política y prioridad
    if (sched_setscheduler(0, new_policy, &param) != 0) {
        if (errno == EPERM) {
            fprintf(stderr, "Error: No tienes permisos suficientes para cambiar la política y prioridad. Ejecuta el programa como root o usa 'setcap cap_sys_nice=eip ./procesos'.\n");
        } else {
            handle_error("Error estableciendo la nueva política y prioridad");
        }
    }

    // Verificar y mostrar la nueva configuración
    if (sched_getparam(0, &param) != 0) {
        handle_error("Error obteniendo la nueva prioridad");
    }
    new_policy = sched_getscheduler(0);
    if (new_policy == -1) {
        handle_error("Error obteniendo la nueva política");
    }

    printf("Nueva política: %d, Nueva prioridad: %d\n", new_policy, param.sched_priority);

    // Modificar la política a SCHED_RR y asignar la prioridad máxima
    new_policy = SCHED_RR;
    param.sched_priority = sched_get_priority_max(SCHED_RR);

    if (param.sched_priority == -1) {
        handle_error("Error obteniendo la prioridad máxima para SCHED_RR");
    }

    if (sched_setscheduler(0, new_policy, &param) != 0) {
        handle_error("Error estableciendo SCHED_RR con prioridad máxima");
    }

    // Verificar y mostrar la nueva configuración
    if (sched_getparam(0, &param) != 0) {
        handle_error("Error obteniendo la nueva prioridad");
    }
    new_policy = sched_getscheduler(0);
    if (new_policy == -1) {
        handle_error("Error obteniendo la nueva política");
    }

    printf("Nueva política: %d (SCHED_RR), Nueva prioridad: %d\n", new_policy, param.sched_priority);

    return EXIT_SUCCESS;
}
