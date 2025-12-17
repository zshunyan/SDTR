#include "directorios.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

namelist *get_directorios_1_svc(char **argp, struct svc_req *rqstp)
{
    	// MANUAL DEL BUEN PROGRAMADOR EN RPC:
    	// Creamos la estructura de forma estática
    	// y liberamos todo lo que hubiera antes
	//TODO: Busca información sobre la función xdr_free. Qué hace? Qué parámetros tiene? Etc.
	static namelist result = NULL;
    	xdr_free((xdrproc_t)xdr_namelist, (char *)&result);

	//TODO: Creamos la estructura directorio con dirent
	// busca información sobre esta estructura, qué campos tiene? Dónde se define?
    	struct dirent *directorio;

	//TODO: Abrimos el directorio 
	// Busca información sobre la función opendir. 
	// Qué se le debe pasar por parámetro de entrada
    	DIR *dir_padre = opendir(*argp);

    	if (dir_padre == NULL)
    	{
        	printf("ERROR: Directorio ** %s ** no abierto\n", *argp);
        	perror("ERROR AL ABRIR EL DIRECTORIO");
        	return NULL;
    	}
    	printf("OK: Directorio ** %s ** abierto\n", *argp);

	// Nos preparamos para recorrer lo que haya dentro del directorio
	// Creamos un puntero en la misma zona de memoria del resultado para 
	// almacenar siempre el último nodo de la lista.
    	namelist *current = &result; 

    	// Recorremos el directorio y almacenamos los nombres en la lista enlazada
	//TODO: ¿cómo se recorre un directorio, busca información de readdir
    	while ((directorio = readdir(dir_padre)) != NULL)
    	{
        	printf("Directorio actual: %s\n", directorio->d_name);

        	// Saltar las entradas "." y ".."
        	if (strcmp(directorio->d_name, ".") == 0 || strcmp(directorio->d_name, "..") == 0)
            		continue;

        	// Crear un nuevo nodo de la lista
		// TODO: explica qué hace esta linealidad de código y rellénala
        	namelist new_node = (namelist)malloc(sizeof(struct namenode));
        	if (!new_node)
        	{
            		perror("Error al asignar memoria para el nodo");
            		closedir(dir_padre);
            		return &result; // Devuelve lo que haya almacenado hasta ahora
        	}

		//TODO: copia el nombre del directorio al new_node
        	new_node->name = strdup(directorio->d_name);
        	new_node->next = NULL; // El nuevo nodo apunta a NULL

        	// Insertar el nodo en la lista
        	*current = new_node;
        	current = &new_node->next;
    	}
	
	//TODO cerramos el directorio. Buscar información de la función closedir
    	closedir(dir_padre);
    
	return &result;
}