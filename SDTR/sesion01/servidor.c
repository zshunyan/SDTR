#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <sys/utsname.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "cabecera.h"


int * getprocess();
char ** getuser();
double * squaredroot(int *num);
char ** getnodename();

int main(){

    if (registerrpc(NPROG_0, NVERS_0, NPROC_0, (char *(*)(char *))getprocess
, (xdrproc_t)xdr_void, (xdrproc_t)xdr_int) != 0) {
        printf("Error al registrar el procedimiento remoto: getprocess (%d)\
n", NPROC_0);
    }
    if (registerrpc(NPROG_0, NVERS_0, NPROC_1, (char *(*)(char *))getuser, (
xdrproc_t)xdr_void, (xdrproc_t)xdr_wrapstring) != 0) {
        printf("Error al registrar el procedimiento remoto: getuser (%d)\n",
 NPROC_1);
    }
    if (registerrpc(NPROG_0, NVERS_0, NPROC_2, (char *(*)(char *))getnodenam
e, (xdrproc_t)xdr_void, (xdrproc_t)xdr_wrapstring) != 0) {
        printf("Error al registrar el procedimiento remoto: getnodename (%d)
\n", NPROC_2);
    }


    svc_run();
    fprintf(stderr, "Error: svc_run returned\n");
    exit(1);
}


int * getprocess(){
    static int result;
    result = getpid();
    return &result;
}

char ** getuser(){
    static char *user;
    user = getenv("USER");
    return &user;
}

double * squaredroot(int *num){
    static double result;
    result = sqrt(*num);
    return &result;
}

char ** getnodename(){
    static char *nodename;
    static char nodename_buffer[256];
    struct utsname uts;
    struct hostent *host_entry;
    char ip[INET_ADDRSTRLEN];

    if (uname(&uts) == 0) {
        host_entry = gethostbyname(uts.nodename);
        if (host_entry != NULL) {
            inet_ntop(AF_INET, host_entry->h_addr_list[0], ip, sizeof(ip));
            snprintf(nodename_buffer, sizeof(nodename_buffer), "Nodo: %s, IP
: %s", uts.nodename, ip);
        } else {
            snprintf(nodename_buffer, sizeof(nodename_buffer), "Nodo: %s, IP
: desconocida", uts.nodename);
        }
    } else {
        snprintf(nodename_buffer, sizeof(nodename_buffer), "Error al obtener
 el nombre del nodo");
    }

    nodename = nodename_buffer;
    return &nodename;
}