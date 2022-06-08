//------------------------------------------------------------
// Server UDP for testing pourpose
// OS: linux
// Compiler: gcc
// tags: Global
// https://docs.oracle.com/cd/E23824_01/html/821-1602/sockets-14.html
// http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/server.c
//------------------------------------------------------------

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

// Private Types Constants and Macros ------------------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Private Functions Declaration -----------------------------------------------
void error(char *msg);


// Function Definitions --------------------------------------------------------
int main (int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(EXIT_FAILURE);
    }
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    printf("Socket port #%d\n", ntohs(serv_addr.sin_port));

    int i = 0;    
    while (1)
    {
        // Read a packet from the socket.
        bzero(buffer, sizeof(buffer));
        
        if (read(sockfd, buffer, sizeof(buffer) - 1) == -1 )
            error("ERROR receiving datagram packet");

        i++;
        int len = strlen (buffer);
        printf("msg_num: %d, len: %d here is the message: %s\n",
               i,
               len,
               buffer);
    }

    return 0;
}


void error (char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

//--- end of file ---//
