#include "m.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mostrar_ayuda(char *nombre_programa)
{
    printf("Uso: %s <servidor> <comando> [argumentos...]\n", nombre_programa);
    printf("Comandos disponibles:\n");
    printf("  registrar <nombre_estacion>   - Registra una nueva estación\n");
    printf("  media <nombre_estacion> <tipo_sensor>  - Obtiene la media de un sensor (0=Temp, 1=Hum, 2=Pres)\n");
    printf("  sensor <nombre_estacion> <tipo_sensor> - Obtiene todas las mediciones de un sensor\n");
    printf("  mediasglobales                - Obtiene las medias de todos los sensores de todas las estaciones\n");
    printf("  baja                          - Elimina todas las estaciones\n");
}

void registrar_estacion(CLIENT *clnt, char *nombre)
{
    int *result;
    NombreEstacion setestacion_1_arg;

    // Validar entrada
    if (nombre == NULL || strlen(nombre) == 0)
    {
        printf("Error: Nombre de estación inválido.\n");
        return;
    }

    printf("(DENTRO) Registrando estación: %s\n", nombre);

    // Llamar al procedimiento remoto
    result = setestacion_1(&nombre, clnt);
    if (result == NULL)
    {
        clnt_perror(clnt, "Error en llamada RPC a setEstacion");
        return;
    }

    if (*result == -1)
    {
        printf("Error: Ya existe una estación con el nombre '%s'.\n", nombre);
    }
    else if (*result == -2)
    {
        printf("Error: Lista de estaciones llena. No se puede registrar '%s'.\n", nombre);
    }
    else if (*result == 0)
    {
        printf("Estación '%s' registrada correctamente.\n", nombre);
    }
    else
    {
        printf("Error desconocido al registrar la estación '%s'.\n", nombre);
    }
}

void obtener_media_sensor(CLIENT *clnt, char *nombre, TipoSensor tipo)
{
    float *result;
    ConsultaSensor consulta;

    // Validar entrada
    if (nombre == NULL || strlen(nombre) == 0)
    {
        printf("Error: Nombre de estación inválido.\n");
        return;
    }

    if (tipo < TEMPERATURA || tipo > PRESION)
    { // Validar tipo de sensor
        printf("Error: Tipo de sensor inválido. Debe ser 0 (Temperatura), 1 (Humedad) o 2 (Presión).\n");
        return;
    }

    // Inicializar la consulta
    consulta.nombre = nombre;
    consulta.tipo = tipo;

    // Llamar al procedimiento remoto
    result = getmediasensor_estacion_1(&consulta, clnt);
    if (result == NULL)
    {
        clnt_perror(clnt, "Error en llamada RPC a getMediaSensor_Estacion");
        return;
    }

    if (*result == -1)
    {
        printf("Error: No se encontró la estación '%s'.\n", nombre);
    }
    else if (*result >= 0)
    {
        printf("Media del sensor %d de la estación '%s': %.2f\n", tipo, nombre, *result);
    }
    else
    {
        printf("Error desconocido al obtener la media del sensor %d de la estación '%s'.\n", tipo, nombre);
    }
}

void obtener_sensor(CLIENT *clnt, char *nombre, TipoSensor tipo)
{
    Resultado1 *result;
    ConsultaSensor consulta;

    // Validar entrada
    if (nombre == NULL || strlen(nombre) == 0)
    {
        printf("Error: Nombre de estación inválido.\n");
        return;
    }

    if (tipo < TEMPERATURA || tipo > PRESION)
    { // Validar tipo de sensor
        printf("Error: Tipo de sensor inválido. Debe ser 0 (Temperatura), 1 (Humedad) o 2 (Presión).\n");
        return;
    }

    // Inicializar la consulta
    memset(&consulta, 0, sizeof(consulta));
    consulta.nombre = nombre;
    consulta.tipo = tipo;

    // Llamar al procedimiento remoto
    result = getsensor_estacion_1(&consulta, clnt);
    if (result == NULL)
    {
        clnt_perror(clnt, "Error en llamada RPC a getSensor_Estacion");
        return;
    }

    printf("Valores del sensor %d de la estación '%s':\n", tipo, nombre);
    for (u_int i = 0; i < result->valores.valores_len; i++)
    {
        printf("  Muestra %d: %.2f\n", i + 1, result->valores.valores_val[i]);
    }

    if (result->valores.valores_len == 0)
    {
        printf("No se encontraron muestras.\n");
    }
}

void obtener_medias_globales(CLIENT *clnt)
{
    Resultado2 *result;

    // Llamar al procedimiento remoto
    result = getmediasallestaciones_1(NULL, clnt);
    if (result == NULL)
    {
        clnt_perror(clnt, "Error en llamada RPC a getMediasAllEstaciones");
        return;
    }

    printf("Medias globales de todas las estaciones:\n");
    printf("  Temperatura: %.2f\n", result->mediaTemperatura);
    printf("  Humedad: %.2f\n", result->mediaHumedad);
    printf("  Presión: %.2f\n", result->mediaPresion);
}

void dar_baja(CLIENT *clnt)
{
    int *result;

    // Llamar al procedimiento remoto
    result = darbaja_1(NULL, clnt);
    if (result == NULL)
    {
        clnt_perror(clnt, "Error en llamada RPC a darBaja");
        return;
    }

    printf("Todas las estaciones han sido eliminadas.\n");
}

int main(int argc, char *argv[])
{
    CLIENT *clnt;
    char *host;

    if (argc < 3)
    { // Cambiar a 3 porque "mediasglobales" y "baja" no necesitan más argumentos
        mostrar_ayuda(argv[0]);
        exit(1);
    }

    host = argv[1];

    // Crear el cliente RPC
    clnt = clnt_create(host, METEO_PROG, VERS_GRUPO, "udp");
    if (clnt == NULL)
    {
        clnt_pcreateerror(host);
        exit(1);
    }
    printf("Conexión al servidor %s establecida correctamente.\n", host);

    // Procesar el comando
    if (strcmp(argv[2], "registrar") == 0)
    {
        if (argc < 4)
        {
            printf("Error: El nombre de la estación no proporcionado.\n");
            mostrar_ayuda(argv[0]);
            return 1;
        }
        printf("Registrando estación: %s\n", argv[3]);
        registrar_estacion(clnt, argv[3]);
        printf("Estación registrada correctamente.\n");
    }
    else if (strcmp(argv[2], "media") == 0)
    {
        if (argc < 5)
        { // Cambiar a 5 porque "media" necesita dos argumentos adicionales
            printf("Error: El nombre de la estación o el tipo de sensor no proporcionado.\n");
            mostrar_ayuda(argv[0]);
            return 1;
        }
        printf("Obteniendo media del sensor %s en la estación %s\n", argv[4], argv[3]);
        obtener_media_sensor(clnt, argv[3], atoi(argv[4]));
        printf("Media del sensor obtenida correctamente.\n");
    }
    else if (strcmp(argv[2], "sensor") == 0)
    {
        if (argc < 5)
        { // Cambiar a 5 porque "sensor" necesita dos argumentos adicionales
            printf("Error: El nombre de la estación o el tipo de sensor no proporcionado.\n");
            mostrar_ayuda(argv[0]);
            return 1;
        }
        printf("Obteniendo valores del sensor %s en la estación %s\n", argv[4], argv[3]);
        obtener_sensor(clnt, argv[3], atoi(argv[4]));
        printf("Valores del sensor obtenidos correctamente.\n");
    }
    else if (strcmp(argv[2], "mediasglobales") == 0)
    {
        // No se necesitan argumentos adicionales
        printf("Obteniendo medias globales de todas las estaciones.\n");
        obtener_medias_globales(clnt);
        printf("Medias globales obtenidas correctamente.\n");
    }
    else if (strcmp(argv[2], "baja") == 0)
    {
        // No se necesitan argumentos adicionales
        printf("Dando de baja todas las estaciones.\n");
        dar_baja(clnt);
        printf("Todas las estaciones han sido eliminadas correctamente.\n");
    }
    else
    {
        printf("Comando no reconocido: %s\n", argv[2]);
        mostrar_ayuda(argv[0]);
    }

    // Liberar recursos
    clnt_destroy(clnt);
    return 0;
}