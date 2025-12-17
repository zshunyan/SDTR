/*
 * CLIENTE.c 
 */
 #include <rpc/rpc.h>
 #include "estu_cabecera.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>

 void call_remote_procedure(char *hostserver, int proc_num, xdrproc_t inproc, char *in, xdrproc_t outproc, char *out, char *proc_name);
 int main(int argc, char * argv[])
 {
        char * hostserver;   // para ubicar al servidor
        int    stat;         // para error en las llamadas
        int    serverpid;    // para PID del servidor
        char * serveruser = malloc(20*sizeof(char)); //nombre de usuario del server
        char * serverip   = malloc(20*sizeof(char)); //ip del server
        char *servername = malloc(256 * sizeof(char)); // nombre del host del server

        // Comprobamos que al llamar tiene la IP del servidor:
        if (argc != 2)
        {
                printf("Necesitamos la IP del server: ./client serverIP\n");
                return -1;
        }
        // Cargamos la dirección del servidor:
        hostserver = argv[1];
        printf("Conectaremos con el servidor %s\n",hostserver);

        /*
        * Llamamos a los procedimientos del servidor
        */
        call_remote_procedure(hostserver, NPROC_1, (xdrproc_t)xdr_void, NULL, (xdrproc_t)xdr_int, (char *)&serverpid, "getprocessID");
        printf("[PROC-1] PID del proceso servidor:  %d\n", serverpid);
    
        call_remote_procedure(hostserver, NPROC_2, (xdrproc_t)xdr_void, NULL, (xdrproc_t)xdr_wrapstring, (char *)&serveruser, "getuserNAME");
        printf("[PROC-2] USERNAME del proceso servidor:  %s\n", serveruser);
    
        call_remote_procedure(hostserver, NPROC_3, (xdrproc_t)xdr_void, NULL, (xdrproc_t)xdr_wrapstring, (char *)&serverip, "getserverIP");
        printf("[PROC-3] IP del proceso servidor:  %s\n", serverip);
    
        call_remote_procedure(hostserver, NPROC_4, (xdrproc_t)xdr_void, NULL, (xdrproc_t)xdr_wrapstring, (char *)&servername, "gethostnameSERVER");
        printf("[PROC-4] Hostname del proceso servidor:  %s\n", servername);
    
        //liberamos
        free(serveruser);
        free(serverip);
        free(servername);
         
        return 0;
 }
 void call_remote_procedure(char *hostserver, int proc_num, xdrproc_t inproc, char *in, xdrproc_t outproc, char *out, char *proc_name)
 {
     int stat;
     if ((stat = callrpc(hostserver, NPROG, NVERS, proc_num, inproc, in, outproc, out)) != 0)
     {
         clnt_perrno(stat);
         printf("[PROC-%d] Error al llamar al procedimiento remoto %s\n", proc_num, proc_name);
         exit(1);
     }
 }