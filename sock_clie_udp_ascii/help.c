#include "help.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void err_quit (char * msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void xfer_data (int source, int target)
{
	char buff[1024];
	int cont, len;
	
	//leo el archivo de entrada y escribo la salida
	while ((cont = read(source, buff, sizeof(buff))) > 0)
	{
		if (cont < 0)
			err_quit("help.c :xfer_data:read");
		if ((len = write(target, buff, cont)) != cont)
			err_quit("help.c :xfer_data:write");
	}
}
