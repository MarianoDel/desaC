#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "help.h"

int main (int argc, char *argv[])
{
	int desc_socket;
	struct sockaddr_in struct_clie;	//la estructura del cliente
	socklen_t size_direccion;
	
	printf("PRUEBA DE INTERFACE CON SOCKETS CAPA 4\n");
	if (argc != 3)
	{
		printf("Modo de empleo pru_clie <dir IP> <port tcp>\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Intento abrir un socket\n");
	if ((desc_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		err_quit("socket");

	memset(&struct_clie, 0, sizeof(struct_clie));
	struct_clie.sin_family = AF_INET;
	
	unsigned short selected_port;
	selected_port = atoi(argv[2]);
	struct_clie.sin_port = htons(selected_port);	//htons para cambiar el orden de bytes

	printf("verifico la direccion que se ingreso\n");
	if(!(inet_aton(argv[1], &struct_clie.sin_addr)))
		err_quit("inet aton");
	
	printf("Intento conectarme al socket\n");
	size_direccion = sizeof(struct_clie);
	if (connect(desc_socket, (struct sockaddr *)& struct_clie, size_direccion))
		err_quit("connect");

	printf("Cliente conectado\n");

	xfer_data(fileno(stdin), desc_socket);
	exit(EXIT_SUCCESS);
	return 0;
}
