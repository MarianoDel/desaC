//----------------------------------------
// programa de prueba libcsv
//----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "cdr_omc.h"
#include "cdr_asterisk.h"
#include "server_tcp.h"


//-- Module Functions Declarations ---------------




//-- Module Functions Definitions ---------------
int main (int argc, char *argv[])
{
    char working_buff [100];
    
    // if (argc != 3)
    // {
    //     fprintf(stderr, "Usage: init date YYYYMMDD end date YYYYMMDD\n");
    //     return EXIT_FAILURE;
    // }

    // if (CreateCdrFromDates (argv[1], argv[2], "loco.txt") != 0)
    //     return EXIT_FAILURE;
    
    // sprintf(working_buff, "%s/%s", CDR_WORKING_TMP_DIR, CDR_WORKING_TMP_FILE);
    // printf("%s creado\n", working_buff);

    // if (AsteriskCDRProcessFile (working_buff, "lama.cal") == 0)
    // {
    //     printf("LAMA.CAL creado\n");
    // }
    int error_counter = 0;

    if (error_counter < 10)
    {
        if (StartServerTCP() < 0)
        {
            printf("Main. Error al crear server\n");
            error_counter++;
            sleep(5);
        }        
    }
            
    return EXIT_SUCCESS;
}


//--- end of file ---//
