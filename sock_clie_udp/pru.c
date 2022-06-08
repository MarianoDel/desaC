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
        printf("for send ascii use: %s <server> <port> <data1> ... <dataN>\n", argv[0]);
        printf("for send binary use: %s <server> <port> <data> <wait in s>\n", argv[0]);
        printf("  <data> only numbers or A to F\n");
        printf("  <wait in s> seconds from 0 to 99\n");        
        _exit(1);
    }

    /* get server IP address (no check if input is IP address or DNS name */
    h = gethostbyname(argv[1]);
    if(h == NULL)
    {
        printf("%s: unknown host '%s' \n", argv[0], argv[1]);
        _exit(1);
    }

    unsigned short selected_port;
    selected_port = atoi(argv[2]);

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
    if ((argc == 5) &&
        ((*argv[3] >= '0') && (*argv[3] <= '9')) ||
        ((*argv[3] >= 'A') && (*argv[3] <= 'F')) ||
        ((*argv[3] >= 'a') && (*argv[3] <= 'f')))
    {
        char a = 0;
        unsigned char b = 0;
        unsigned char buff_to_send [30] = { 0 };
        int buff_len = strlen(argv[3]);

        for (int ii = 0; ii < buff_len; ii++)
        {
            a = *(argv[3] + ii);
            printf("%c ", a);
            if ((a >= '0') && (a <= '9'))
                b = a - '0';
            else if ((a >= 'A') && (a <= 'F'))
                b = a - 'A' + 10;
            else if ((a >= 'a') && (a <= 'f'))
                b = a - 'a' + 10;
            else
            {
                printf("data error");
                _exit(1);
            }

            buff_to_send[ii] = b;
        }

        printf("sending binary data bytes: %d\n", buff_len);
        rc = sendto(sd, buff_to_send, buff_len, 0,
                    (struct sockaddr *) &remoteServAddr,
                    sizeof(remoteServAddr));
        
        if(rc < 0)
        {
            printf("%s: cannot send data %d \n",argv[0],i-1);
            close(sd);
            _exit(1);
        }

        // wait for server answer
        int seconds = atoi(argv[4]);
        if ((seconds > 0) && (seconds < 100))
        {
            bzero(buff_to_send, sizeof(buff_to_send));
            printf("wait for seconds %d", seconds);
            fflush(stdout);
            while (seconds)
            {
                rc = recv(sd, buff_to_send, sizeof(buff_to_send), MSG_DONTWAIT);
                if (rc > 0)
                {
                    printf("\nasnwer getted!!! message: %s\n", buff_to_send);
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
    }
    else
    {
        for(i=3;i<argc;i++)
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
    }
	
    return 1;
}
