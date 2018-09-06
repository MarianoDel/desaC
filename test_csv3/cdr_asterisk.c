//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CDR_ASTERISK.C ########################
//---------------------------------------------
#include "cdr_asterisk.h"
#include "csv.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>



#define BUFF_SIZE    1024

static int put_comma = 0;
static int campos_por_linea = 0;
static int lineas = 0;
static int processed_lines = 0;


//-- Private Module Functions Declarations ---------------
void cb1 (void *, size_t, void *);
void cb2 (int, void *);
void FlushCDR (void);


//--- Module Functions Definition ------------------------
int AsteriskCDRProcessFile (char * input, char * output)
{
    char buf[BUFF_SIZE];
    size_t i;
    struct csv_parser p;
    FILE *infile;
    int seq = 0;

    FILE *outfile;    

    seq = AsteriskGetSecLama();
    
    if (seq == -1)
        exit(EXIT_FAILURE);
        
    csv_init(&p, 0);    
    
    infile = fopen(input, "rb");
    if (infile == NULL) {
        fprintf(stderr, "Failed to open file %s: %s\n", input, strerror(errno));
        exit(EXIT_FAILURE);
    }

    outfile = fopen(output, "w");
    if (outfile == NULL) {
        fprintf(stderr, "Failed to open file %s: %s\n", output, strerror(errno));
        exit(EXIT_FAILURE);
    }

    processed_lines = 0;
    //C TOT,622,SECUENCIA
    sprintf(buf, "C TOT,%d,SECUENCIA\r\n", seq);
    fputs(buf, outfile);
    
    while (fgets(buf, BUFF_SIZE, infile) != NULL)        
    {
        i = strlen(buf);

        if (csv_parse(&p, buf, i, cb1, cb2, outfile) != i)
        {
            fprintf(stderr, "Error parsing file: %s\n", csv_strerror(csv_error(&p)));
            fclose(infile);
            fclose(outfile);            
            exit(EXIT_FAILURE);
        }
    }

#ifdef ONLY_ONE_LINE_FOR_TEST
    // solo una linea para test
    if ((fgets(buf, BUFF_SIZE, infile)) != NULL)
    // {
    //     printf("%s\nbuffer size: %d\n", buf, strlen(buf));
    // }
    {
        i = strlen(buf);

        if (csv_parse(&p, buf, i, cb1, cb2, outfile) != i)
        {
            fprintf(stderr, "Error parsing file: %s\n", csv_strerror(csv_error(&p)));
            fclose(infile);
            fclose(outfile);            
            exit(EXIT_FAILURE);
        }
    }
#endif    //ONLY_ONE_LINE_FOR_TEST

    //cierro el ultimo renglon
    //,FIN,622
    sprintf(buf, ",FIN,%d\r\n", processed_lines);
    fputs(buf, outfile);    

    csv_fini(&p, cb1, cb2, outfile);
    csv_free(&p);

    // if (ferror(infile)) {
    //     fprintf(stderr, "Error reading from input file");
    //     fclose(infile);
    //     remove(argv[2]);
    //     exit(EXIT_FAILURE);
    // }
    fclose(outfile);
    fclose(infile);

    return EXIT_SUCCESS;    
}

#ifdef MAX_NUMBER_OF_USERS
//Procesa un archivo lama ya trabajado previamente por AsteriskCDRProcessFile()
//y lo limita con la cantidad de usuarios
int AsteriskCDRLimitUsers (char * input, char * output)
{
    //armo un vector de la cantidad de usuarios que permito y lo voy completando
    //si llego a lenght_max, borro las entradas de los usuarios de la lista
    long long int users [MAX_NUMBER_OF_USERS] = { 0 };

    FILE *infile;
    FILE *outfile;

    infile = fopen(input, "r");
    if (infile == NULL)
        return -1;

    outfile = fopen(output, "w");
    if (outfile == NULL)
        return -1;

    //leo una linea de entrada cargo y veo el vector
    //luego escribo una linea de salida
    return 0;
}
#endif

//toma la secuencia del lama desde un archivo que se define en
//cdr_asterisk.h
int AsteriskGetSecLama (void)
{
    char buf_file[128];
    FILE * seclama_file;
    int sequence = 0;

    sprintf(buf_file, "%s/%s", ASTERISK_CDR_SECLAMA_DIR, ASTERISK_CDR_SECLAMA_FILE);
    
    seclama_file = fopen(buf_file, "r");
    if (seclama_file == NULL) {
        // fprintf(stderr, "Failed to open file %s: %s\n", buf_file, strerror(errno));
        return -1;
    }

    fgets(buf_file, sizeof(buf_file), seclama_file);
    sequence = atoi(buf_file);
    fclose(seclama_file);
    
    if ((sequence < 0) || (sequence > 65000))
        return -1;

    sprintf(buf_file, "%s/%s", ASTERISK_CDR_SECLAMA_DIR, ASTERISK_CDR_SECLAMA_FILE);
    seclama_file = fopen(buf_file, "w");
    if (seclama_file == NULL) {
        // fprintf(stderr, "Failed to open file %s: %s\n", buf_file, strerror(errno));
        return -1;
    }
    
    sprintf (buf_file, "%d", sequence + 1);
    fputs (buf_file, seclama_file);
    fclose(seclama_file);

    return sequence;
}

//aparentemente es el wrapper del final de campo
//tengo que editarlo segun lo que quiera ahi
//en s tengo el campo
//en i el tamanio
//outfile es el stream o file que quiera usar
void cb1 (void *s, size_t i, void *outfile)
{
    // if (put_comma)
    //     putc(',', stdout);
    // csv_fwrite(stdout, s, i);
    // put_comma = 1;

    switch (campos_por_linea)
    {
    case CDR_ORIGIN:
        if (i <= CDR_FIELD_SIZE)
            strncpy(cdr.abonado, s, i);
        else
            strncpy(cdr.abonado, s, CDR_FIELD_SIZE);
        break;

    case CDR_DEST:
        if (i <= CDR_FIELD_SIZE)        
            strncpy(cdr.destino, s, i);
        else
            strncpy(cdr.destino, s, CDR_FIELD_SIZE);
        break;

    case CDR_DATE_ORIGIN:
        if (i <= CDR_FIELD_SIZE)
            strncpy(cdr.fecha_hora, s, i);
        else
            strncpy(cdr.fecha_hora, s, CDR_FIELD_SIZE);
        break;

    case CDR_SECS_CHN_COMM:
        if (i <= CDR_FIELD_SIZE)
            strncpy(cdr.segundos, s, i);
        else
            strncpy(cdr.segundos, s, CDR_FIELD_SIZE);
        break;

    case CDR_CALL_STATE:
        if (i <= CDR_FIELD_SIZE)
            strncpy(cdr.estado, s, i);
        else
            strncpy(cdr.estado, s, CDR_FIELD_SIZE);
        break;
    }        
        
    campos_por_linea++;

    // csv_fwrite((FILE *)outfile, s, i);
    // fputc(',',(FILE *)outfile);
}

//aparentemente es el wrapper del final de linea
//tengo que editarlo segun lo que quiera ahi
void cb2 (int c, void *outfile)    
{
    char fecha [20];
    char hora [20];

    //hago ciertos chequeos para ver si graba el llamado en el LAMA.CAL
    //primero veo si fue contestado
    if (strncmp(cdr.estado, "ANSWERED", sizeof("ANSWERED") - 1) != 0)
    {
        FlushCDR();
        campos_por_linea = 0;
        lineas++;
        return;
    }

    //me fijo si el tiempo de comunicacion es mayor a 0
    int tiempo_comm = atoi(cdr.segundos);
    if (tiempo_comm < 1)
    {
        FlushCDR();
        campos_por_linea = 0;
        lineas++;
        return;        
    }
    //tengo que separar la fecha y la hora en campos distintos
    char * space_chr = strchr (cdr.fecha_hora, ' ');

    if (space_chr != NULL)
    {
        int pos = space_chr - cdr.fecha_hora;
        // int pos = space_chr - &cdr.fecha_hora;
        memset(fecha, '\0', sizeof(fecha));
        strncpy(fecha, cdr.fecha_hora, pos);
        strcpy(hora, (cdr.fecha_hora + pos + 1));
    }
    
    fprintf((FILE *) outfile, "C TOT,%s,%s,%s,%s,%s,1,1\r\n",
            cdr.abonado,
            cdr.destino,
            fecha,
            hora,
            cdr.segundos);

    // fprintf((FILE *) outfile, "C TOT,%s,%s,%s,%s,%s,%s,1,1\r\n",
    //         cdr.abonado,
    //         cdr.destino,
    //         fecha,
    //         hora,
    //         cdr.segundos,
    //         cdr.estado);
    
    processed_lines++;
    
    FlushCDR();
    campos_por_linea = 0;
    lineas++;
}

void FlushCDR (void)
{
    memset(&cdr, '\0', sizeof(cdr));
}

//--- end of file ---//

