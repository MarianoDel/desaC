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
#define ASCII_OR_BIN_POS    1
#define SERVER_IP    2
#define SERVER_PORT    3
#define BINARY_DATA_POS    4
#define BINARY_WAIT_POS    5
#define BINARY_TOTAL_ARGC    6

#define ASCII_DATA_POS    BINARY_DATA_POS

// for select ascii or binary
#define USE_UNKNOW    0
#define USE_ASCII    1
#define USE_BINARY    2


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
        
    /* check command line args */
    if(argc < 4)
    {
        printf("for send ascii use: %s A <server> <port> <data1> ... <dataN>\n", argv[0]);
        printf("for send binary use: %s B <server> <port> <data> <wait in s>\n", argv[0]);
        printf("  <data> only numbers or A to F\n");
        printf("  <wait in s> seconds from 0 to 99\n");        
        _exit(1);
    }

    int ascii_or_binary = USE_UNKNOW;
    if (*argv[ASCII_OR_BIN_POS] == 'A')
        ascii_or_binary = USE_ASCII;
    else if (*argv[ASCII_OR_BIN_POS] == 'B')
        ascii_or_binary = USE_BINARY;
    else
    {
        printf("please select ascii 'A' or binary 'B'\n");
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

    // send data ASCII or BINARY?
    if ((ascii_or_binary == USE_BINARY) &&
        (argc == BINARY_TOTAL_ARGC))
    {
        char a = 0;
        unsigned char b = 0;
        unsigned char buff_to_send [30] = { 0 };
        int buff_len = strlen(argv[BINARY_DATA_POS]);
        
        // check only numbers or a to f
        for (int ii = 0; ii < buff_len; ii++)
        {
            a = *(argv[BINARY_DATA_POS] + ii);

            if (((a >= '0') && (a <= '9')) ||
                ((a >= 'A') && (a <= 'F')) ||
                ((a >= 'a') && (a <= 'f')))
            {
            }
            else
            {
                printf("data error, only 0 to 9 or a to f permited!");
                _exit(1);
            }
        }

        // show the numbers and set the buffer
        for (int ii = 0; ii < buff_len; ii++)
        {
            a = *(argv[BINARY_DATA_POS] + ii);
            printf("%c ", a);
            b = atoi(&a);
            buff_to_send[ii] = b;
        }

        printf("sending binary data bytes: %d\n", buff_len);
        rc = sendto(sd, buff_to_send, buff_len, 0,
                    (struct sockaddr *) &remoteServAddr,
                    sizeof(remoteServAddr));
        
        if(rc < 0)
        {
            printf("%s: cannot send data %d \n", argv[0], i-1);
            close(sd);
            _exit(1);
        }

        // wait for server answer
        int seconds = atoi(argv[BINARY_WAIT_POS]);
        if ((seconds > 0) && (seconds < 100))
            get_answer(sd, seconds, buff_to_send, sizeof(buff_to_send));
        
    }
    else if (ascii_or_binary == USE_ASCII)
    {
        // wait for server answer?
        int seconds = 0;
        unsigned char buff_to_send [30] = { 0 };
        int max_argc_index = argc;

        if ((strlen(argv[argc - 1]) <= 3))    //no more than three digits
        {
            seconds = atoi(argv[argc - 1]);    
            if ((seconds > 0) && (seconds < 100))    //if seconds != 0 wait for server
                max_argc_index--;
        }

        for(i = ASCII_DATA_POS; i < max_argc_index; i++)
        {
            rc = sendto(sd, argv[i], strlen(argv[i])+1, 0,
                        (struct sockaddr *) &remoteServAddr,
                        sizeof(remoteServAddr));
            if(rc<0)
            {
                printf("%s: cannot send data %d \n",argv[0], i-1);
                close(sd);
                _exit(1);
            }
        }

        // wait for server
        if ((seconds > 0) && (seconds < 100))
            get_answer(sd, seconds, buff_to_send, sizeof(buff_to_send));

    }
    else
        printf("some command line error\n");
	
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
            if (strlen(buff_get) == rc - 1)
                printf("\nascii answer %d bytes message: %s\n", rc - 1, buff_get);
            else
            {
                printf("\nbinary answer %d bytes ", rc);
                printf("message: 0x");
                for (int j = 0; j < rc; j++)
                    printf("%x", buff_get[j]);

                printf("\n");
            }
            
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
