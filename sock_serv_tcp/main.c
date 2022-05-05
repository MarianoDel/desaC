//------------------------------------------------------------
// Server TCP for testing pourpose
// OS: linux
// Compiler: gcc
// tags: Global
// http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/server.c
//------------------------------------------------------------

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

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
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");

    printf("New connection\n");
    int i = 0;

    while (1)
    {
        if (newsockfd < 0) 
            error("ERROR on accept");

        bzero(buffer,256);

        n = read(newsockfd,buffer,255);
        i++;

        if (n < 0)
            error("ERROR reading from socket");

        printf("%d Here is the message: %s\n", i, buffer);

        char answer_str[] = {"I got your message\r\n"};
        n = write(newsockfd, answer_str, sizeof(answer_str));

        if (n < 0)
            error("ERROR writing to socket");
    }

    return 0;
}


void error (char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

//--- end of file ---//
