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

#include "cdr_omc.h"
#include "cdr_asterisk.h"

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
        int pid = fork();
        if (pid < 0)
        {
            printf("Error on fork()");
            return -1;
        }

        if (pid == 0)    //child
        {
            close(sockfd);
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
                // send(newsockfd, "I got your message", 18, 0);

                //proceso los requerimientos
                ProcessReqTCP(buffer, newsockfd);
            }
            exit(0);    //close the child            
        }
        else    //parent
        {
            close(newsockfd);
        }        
    }
    //fuera de bucle no debo llegar aca!
    close(sockfd);
    return 0;
}


//Procesa los requerimientos que le hacen al server TCP
int ProcessReqTCP (char * b, int target_socket)
{
    //el pedido debe ser "C TOT,10/10/18,11/10/18,FIN"
    //                          day/month/year    
    char working_file [100];
    char fecha_ini [20];
    char fecha_fin [20];
    
    //chequeos de seguridad
    if (strncmp(b, "C TOT,", sizeof ("C TOT,") - 1) != 0)
        return -1;

    if (strncmp((b + 23), ",FIN", sizeof (",FIN") - 1) != 0)
        return -1;

    //busco fechas
    strncpy (fecha_ini, (b + 6), 8);
    strncpy (fecha_fin, (b + 15), 8);

    sprintf(working_file, "%s/%s", CDR_WORKING_TMP_DIR, CDR_WORKING_TMP_FILE);

    // printf("working_file: %s\n", working_file);
    // printf("fecha_ini: %s, fecha_fin: %s",fecha_ini, fecha_fin);
    // return 0;

    //creo el archivo de bkp general
    if (CreateCdrFromDates2(fecha_ini, fecha_fin, working_file) != 0)
        return -1;

    printf("%s creado\n", working_file);

    //proceso el archivo
    if (AsteriskCDRProcessFile(working_file, ASTERISK_CDR_LAMA_FILE) != 0)
        return -1;

    printf("%s creado\n", ASTERISK_CDR_SECLAMA_FILE);

#ifdef MAX_NUMBER_OF_USERS    
    // //proceso la cantidad de usuarios permitidos    
    if (AsteriskCDRLimitUsers(ASTERISK_CDR_LAMA_FILE, ASTERISK_CDR_LAMA_USERS_FILE) != 0)
        return -1;
#endif
    
    //envio los registros al puerto TCP
    FILE * pfile;
    char buff_read [256];
    int len;

#ifdef MAX_NUMBER_OF_USERS    
    pfile = fopen(ASTERISK_CDR_LAMA_USERS_FILE, "r");
#else
    pfile = fopen(ASTERISK_CDR_LAMA_FILE, "r");
#endif
    
    if (pfile == NULL)
        return -1;

    while (fgets(buff_read, sizeof(buff_read), pfile) != NULL)
    {
        len = strlen(buff_read);        
        write(target_socket, buff_read, len);        
    }
    fclose(pfile);

    //ya procese todo y lo envie
    return 0;
        
}
