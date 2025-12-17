#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include <unistd.h>
#include <ctype.h>
#include "cabecera.h"

int main(int argc, char * argv[]){
    int stat;
    int *numproceso_server;
    char **nodename;
    int num_procedimiento;
    char *host;

    if (argc != 3) {
        fprintf(stderr, "Uso: %s <host> <num_procedimiento>\n", argv[0]);
        exit(1);
    }

    host = argv[1];
    num_procedimiento = atoi(argv[2]);

    if (stat = callrpc(host, NPROG_0, NVERS_0, num_procedimiento, (xdrproc_t
)xdr_void, 0, (xdrproc_t)xdr_int, (char *)&numproceso_server) != 0) {
        clnt_perrno(stat);
        printf("Error al llamar al procedimiento remoto\n");
    } else {
        printf("Proceso ejecutado en el cliente: %d, en el servidor: %d\n",
getpid(), *numproceso_server);
    }

    if (num_procedimiento == NPROC_2) {
        if (stat = callrpc(host, NPROG_0, NVERS_0, NPROC_2, (xdrproc_t)xdr_v
oid, NULL, (xdrproc_t)xdr_wrapstring, (char *)&nodename) != 0) {
            clnt_perrno(stat);
            printf("Error al llamar al procedimiento remoto getnodename\n");
        } else {
            printf("Nombre del nodo y IP del servidor: %s\n", *nodename);
        }
    }

    return 0;
}