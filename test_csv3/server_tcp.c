//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SERVER_TCP.C ########################
//---------------------------------------------
#include "server_tcp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

int StartServerTCP (void)
{
    int sockfd, newsockfd, portnum, clilen;
    struct sockaddr_in serv_addr, client_addr;
    char buffer[1024]; //
    
    // socklen_t longc; //Debemos declarar una variable que contendrá la longitud de la estructura
        
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //creamos el socket

    if (sockfd < 0)
    {
        printf("no fue posible crear socket\n");
        return -1;
    }
    
    bzero((char *)&serv_addr, sizeof(serv_addr)); //init the struct
    portnum = SERVER_TCP_PORT;
    
    serv_addr.sin_family = AF_INET; //asignamos a la estructura
    serv_addr.sin_port = htons(portnum);
    serv_addr.sin_addr.s_addr = INADDR_ANY; //esta macro especifica nuestra dirección
    
    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    { 
        printf("Error al asociar el puerto a la conexion\n");
        close(sockfd);
        return -1;
    }

    //accept bloquea hasta la nueva conexion
    listen(sockfd, 1);
    printf("A la escucha en el puerto %d\n", ntohs(serv_addr.sin_port));
    
    clilen = sizeof(client_addr); //Asignamos el tamaño de la estructura a esta variable

    //bucle de conexiones
    while (1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &clilen); //Esperamos una conexion
    
        if(newsockfd < 0)
        {
            printf("Error on accept\n");
            // close(sockfd);
            return -1;
        }

        //fork para el nuevo socket
        pid = fork();
        if (pid < 0)
        {
            printf("Error on fork()");
            return -1;
        }

        if (pid == 0)    //child
        {
            close(sockfd);
            
        }
        else    //parent
        {
            close(newsockfd);
        }
        
        printf("Conectando con %s:%d\n", inet_ntoa(client_addr.sin_addr),htons(client_addr.sin_port));
        bzero(buffer,sizeof(buffer));
        int n_bytes = recv (newsockfd, buffer, sizeof(buffer), 0);

        if (n_bytes < 0)
        { //Comenzamos a recibir datos del cliente
            //Si recv() recibe 0 el cliente ha cerrado la conexion. Si es menor que 0 ha habido algún error.
            printf("Error al recibir los datos\n");
            // close(sockfd);
            // return -1;
            close(newsockfd);
        }

        if (n_bytes == 0)
        {
            //conn closed
            printf("conn closed\n");
            close(newsockfd);
            // return 0;
        }
        else
        {
            printf("mensaje: %s\n", buffer);     
            send(newsockfd, "I got your message", 18, 0);
        }
    }
    close(sockfd);
    return 0;
}
