/*
 * SERVIDOR.C
 */
 #include <rpc/rpc.h>
 #include "estu_cabecera.h"
 #include <unistd.h>     // para getpid, getlogin
 #include <netdb.h>      // para gethostbyname, getaddrinfo
 #include <arpa/inet.h>  // para inet_ntoa
 #include <sys/socket.h> // para estructuras de socket
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 
 // CABECERAS DE LOS PROCEDIMIENTOS
 // Devuelven punteros. 
 // En particular, las funciones que devuelven mensajes 
 // en un puntero a una cadena de caracteres
 int *getprocessID();
 char **getuserNAME();
 char **getserverIP();
 char **gethostnameSERVER();
 
 int main() 
 {
	// Registramos primer procedimiento
	if (registerrpc(NPROG, 
			NVERS, 
			NPROC_1, 
			(char *(*)(char *))getprocessID, 
			(xdrproc_t)xdr_void, 
			(xdrproc_t)xdr_int) != 0)
			printf("Error al registrar el procedimiento: getprocessID (%d)\n", NPROC_1);

	// Registramos segundo procedimiento
	if (registerrpc(NPROG, 
			NVERS, 
			NPROC_2, 
			(char *(*)(char *))getuserNAME, 
			(xdrproc_t)xdr_void, 
			(xdrproc_t)xdr_string) != 0)
			printf("Error al registrar el procedimiento: getuserNAME (%d)\n", NPROC_2);

	if (registerrpc(NPROG, 
			NVERS, 
			NPROC_3, 
			(char *(*)(char *))getserverIP, 
			(xdrproc_t)xdr_void, 
			(xdrproc_t)xdr_string) != 0)
			printf("Error al registrar el procedimiento: getserverIP (%d)\n", NPROC_3);

	if (registerrpc(NPROG, 
			NVERS, 
			NPROC_4, 
			(char *(*)(char *))gethostnameSERVER, 
			(xdrproc_t)xdr_void, 
			(xdrproc_t)xdr_string) != 0)
			printf("Error al registrar el procedimiento: gethostnameSERVER (%d)\n", NPROC_4);

	// Servidor a la escucha
	svc_run();
    fprintf(stderr, "Error: svc_run returned\n");
    exit(1);
 }
 
 /*
  * OBTENEMOS EL PID DEL PROCESO SERVIDOR
  * Usamos el metodo getpid
  */
 int * getprocessID()
 {
	 static int result;
	 result=getpid();
	 return &result;
 }
 
 /*
  * OBTENEMOS EL NOMBRE DEL USUARIO QUE EJECUTA EL SERVIDOR
  * Usamos el metodo getlogin(). Devuelve un puntero a una 
  * cadena de caracteres (char *), pero esta cadena no es 
  * asignada dinamicamente, por lo que no se debe modificar 
  * ni liberar con free(). 
  * NO CONFUNDIR: devuelve el nombre del usuario que inicio 
  * sesion, no es el hostname
  * Usamos la funcion strdup(...). Copia la cadena en una 
  * nueva ubicacion de memoria para evitar la sobreescritura
  */
 char ** getuserNAME()
 {
	 static char * result;
	 result=strdup(getlogin());
	 return &result;
 }
 
 /*
  * OBTENEMOS LA IP DEL HOST QUE ALBERGA EL SERVIDOR
  * [Version con gethostbyname]
  * Busca informacion de los metodos y estructuras de datos que
  * se usan en el metodo. Explicalo todo en la memoria de la P1
  * 
  * AMPLIACION: ¿podrias adaptarlo para usar la funcion getaddrinfo 
  * en vez de gethostbyname?
  */
 char **getserverIP()
 {
	// Preparamos la variable resultado. 
	// Al contrario que los int, float...
	// la variable result es un puntero.
	static char *result = NULL;
	// buffer auxiliar para almacenar el nombre del host
	char hostbuffer[256]; 
	// Estructura que almacena información sobre el host
	// TODO: busca informacion de la estructura hostent.
	// ¿donde se declara? ¿para que sirve? ¿que campos tiene? ¿que almacena?
	// TODO: Como se organiza el hosts database
		// Estructura que almacena información sobre el host
	struct hostent *host_entry; 
	// puntero a una lista de direcciones IP del host
	// TODO: busca informacion de la estructura in_addr
	// ¿donde se declara? ¿para que sirve? ¿que campos tiene? ¿que almacena? 
	    // puntero a una lista de direcciones IP del host
	struct in_addr **addr_list; //
 
	// TODO Consultamos el hostname con gethostname
	// Esta funcion obtiene el nombre del host y lo 
	// almacena en una variable de entrada/salida
	// TODO Busca informacion sobre gethostname
	// ¿donde se declara? ¿para que sirve? ¿parametros de entrada y salida?
	    // Consultamos el hostname con gethostname
	if(gethostname(hostbuffer, sizeof(hostbuffer))==-1)
	{
		printf("[ERROR] Gethostname ha fallado\n");
		exit(1);
	}
	// TODO imprimimos el contenido de la variable que tiene el hostname
	printf("Hostname: %s\n", hostbuffer); 
 
	// Usamos el nombre del host para obtener información sobre él, 
	// incluyendo su direccion IP usando la funcion gethostbyname
	// TODO: busca informacion sobre gethostbyname
	// ¿para que sirve? ¿parametros de entrada? ¿que devuelve?
	host_entry = gethostbyname(hostbuffer);
	if (host_entry == NULL) // comprobamos la salida de gethostbyname
	{
		perror("gethostbyname error");
		exit(1);
	}
	
	// Sacamos de la variable devuelta por gethostbyname el campo h_addr_list 
	// TODO: ¿que es el campo h_addr_list? ¿que contiene?
	// le hacemos un casting a struct in_addr ** para acceder facilmente 
	// al contenido de h_addr_list
	addr_list = (struct in_addr **)host_entry->h_addr_list;
	
	// Si el host tiene varias IPs podemos recorrer en un blucle (no es el caso):
	/*
	for (int i = 0; .....[i] != NULL; i++) 
	{
		printf("IP address %d: %s\n", i, ...(*.....[i]));
	}
	*/
	// En nustro caso solo tenemos una IP
	// Almacenamos en result la IP en texto legible
	// para ello usamos inet_ntoa, que convierte la direccion 
	// en formato binario a texto legible.
	result = strdup(inet_ntoa(*addr_list[0]));
 
	return &result;
 }
 /*
 * OBTENEMOS EL NOMBRE DEL HOST QUE ALBERGA EL SERVIDOR
 * Usamos el metodo gethostname
 */
char **gethostnameSERVER()
{
	
    static char * result;
	char hostbuffer[256];
	if (gethostname(hostbuffer, sizeof(hostbuffer)) == -1)
	{
		printf("[ERROR] Gethostname ha fallado\n");
		exit(1);
	}
	result = strdup(hostbuffer);
    return &result;
}
