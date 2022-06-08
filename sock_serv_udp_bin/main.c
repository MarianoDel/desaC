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
#include <arpa/inet.h>

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
    unsigned char buffer[2048];
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

        int flags = 0;
        int sock_len = recvfrom(sockfd, buffer, sizeof(buffer), flags,
                                (struct sockaddr *) &cli_addr, &clilen);
        
        if (sock_len == -1)
            error("ERROR receiving datagram packet");

        i++;
        int ascii_len = strlen (buffer);
        char cli_addr_str [30] = { 0 };
        strcpy(cli_addr_str, (char *) inet_ntoa(cli_addr.sin_addr));

        if (ascii_len == (sock_len - 1))
        {
            printf("ascii_msg: %d, ascii_len: %d cli_addr: %s cli_port: %d\n",
                   i,
                   ascii_len,
                   cli_addr_str,
                   htons(cli_addr.sin_port));
            printf("message: %s\n", buffer);
        }
        else
        {
            printf("binary_msg: %d, sock_len: %d cli_addr: %s cli_port: %d\n",
                   i,
                   sock_len,
                   cli_addr_str,
                   htons(cli_addr.sin_port));

            printf("message: 0x");
            for (int j = 0; j < sock_len; j++)
                printf("%x", buffer[j]);

            printf("\n");
        }
    }

    return 0;
}


void error (char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

//--- end of file ---//
