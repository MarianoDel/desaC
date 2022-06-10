/* fpont 12/99 */
/* pont.net    */
/* udpClient.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <sys/time.h> /* select() */
#include <stdlib.h>

// Module Private Types Constants and Macros -----------------------------------
// argc know positions
#define SERVER_IP    1
#define SERVER_PORT    2
#define ASCII_DATA_POS    3


// Module Private Functions ----------------------------------------------------
void get_answer (int sock, int seconds, char * buff_get, int buff_get_len);


// Module Functions ------------------------------------------------------------
int main (int argc, char *argv[])
{
    int sd, rc, i;
    struct sockaddr_in cliAddr, remoteServAddr;
    struct hostent *h;

    // for (int ii = 0; ii < argc; ii++)
    //     printf("argv[%d]: %s\n", ii, argv[ii]);
        
    // check command line args
    if (argc < 5)
    {
        printf("for send ascii use: %s <server> <port> <data1> ... <dataN> <wait in s>\n", argv[0]);
        printf("  <wait in s> seconds from 0 to 99\n");        
        _exit(1);
    }

    /* get server IP address (no check if input is IP address or DNS name */
    h = gethostbyname(argv[SERVER_IP]);
    if(h == NULL)
    {
        printf("%s: unknown host '%s' \n", argv[0], argv[SERVER_IP]);
        _exit(1);
    }

    unsigned short selected_port;
    selected_port = atoi(argv[SERVER_PORT]);

    printf("%s: sending data to '%s' (IP : %s) \n", argv[0], h->h_name,
           inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));

    remoteServAddr.sin_family = h->h_addrtype;
    memcpy((char *) &remoteServAddr.sin_addr.s_addr, 
           h->h_addr_list[0], h->h_length);
    remoteServAddr.sin_port = htons(selected_port);

    /* socket creation */
    sd = socket(AF_INET,SOCK_DGRAM,0);
    if(sd < 0)
    {
        printf("%s: cannot open socket \n",argv[0]);
        _exit(1);
    }
  
    /* bind any port */
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    cliAddr.sin_port = htons(selected_port+1);
  
    rc = bind(sd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
    if(rc < 0)
    {
        printf("%s: cannot bind port\n", argv[0]);
        _exit(1);
    }

    // wait for server answer?
    int seconds = 0;
    unsigned char buff_to_send [300] = { 0 };
    int max_argc_index = argc;

    if ((strlen(argv[argc - 1]) <= 3))    //no more than three digits
    {
        seconds = atoi(argv[argc - 1]);    
        if ((seconds > 0) && (seconds < 100))    //if seconds != 0 wait for server
            max_argc_index--;
    }

    for(i = ASCII_DATA_POS; i < max_argc_index; i++)
    {
        strcat(buff_to_send, argv[i]);
        if (i < (max_argc_index - 1))
            strcat(buff_to_send, " ");
    }        
        
    rc = sendto(sd, buff_to_send, strlen(buff_to_send), 0,                    
                (struct sockaddr *) &remoteServAddr,
                sizeof(remoteServAddr));
    
    if(rc<0)
    {
        printf("%s: cannot send data %d \n",argv[0], i-1);
        close(sd);
        _exit(1);
    }
        
    // wait for server?
    if ((seconds > 0) && (seconds < 100))
        get_answer(sd, seconds, buff_to_send, sizeof(buff_to_send));

	
    return 1;
}


void get_answer (int sock, int seconds, char * buff_get, int buff_get_len)
{
    int rc;
    bzero(buff_get, buff_get_len);
    printf("wait for seconds %d", seconds);
    fflush(stdout);
    while (seconds)
    {
        rc = recv(sock, buff_get, buff_get_len, MSG_DONTWAIT);
        if (rc > 0)
        {
            printf("\nascii answer %d bytes message: %s\n", strlen(buff_get), buff_get);
            seconds = 0;
        }
        else
        {
            sleep(1);
            seconds--;
            printf(" %d", seconds);
            fflush(stdout);
        }
    }
    printf("\n");
}
