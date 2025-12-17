#!/bin/sh
# Fichero de compilacion para cuando falle el makefile
# chmod +x mi_script.sh

if [ $# -eq 0 ]; then
	echo "Incluye el NOMBRE de la interfaz sin .x"
	exit 1
fi

argumento=$1
#echo "Compilacion con rpcgen de $argumento.x\n"
#rpcgen -a $argumento.x

echo "Preparando compilacion para $argumento\n"

# COMPILAMOS EL CLIENTE
cc -g -c "$argumento"_clnt.c -o "$argumento"_clnt.o -I /usr/include/tirpc -l tirpc
cc -g -c "$argumento"_client.c -o "$argumento"_cliente.o -I /usr/include/tirpc -l tirpc
cc -g -o CLIENTE "$argumento"_clnt.o "$argumento"_cliente.o -lnsl -I /usr/include/tirpc -l tirpc
# COMPILAMOS EL SERVIDOR
cc -g -c "$argumento"_svc.c -o "$argumento"_svc.o -I /usr/include/tirpc -l tirpc
cc -g -c "$argumento"_server.c -o "$argumento"_server.o -I /usr/include/tirpc -l tirpc
cc -g -o SERVIDOR "$argumento"_svc.o "$argumento"_server.o -lnsl -I /usr/include/tirpc -l tirpc
