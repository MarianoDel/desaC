//----------------------------------------
// programa de prueba libcsv
//----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "csv.h"

#define BUFF_SIZE    1024

int put_comma = 0;
int campos_por_linea = 0;
int lineas = 0;

#define CDR_FIELD_SIZE    40

//definiciones de los campos CDR
typedef enum {
    CDR_NO_USE_a = 0,
    CDR_ORIGIN,
    CDR_DEST,
    CDR_CONTEXT,
    CDR_ORIGIN_CHN,
    CDR_ORIGIN_CHN_PROC,
    CDR_DEST_CHN,
    CDR_APP,
    CDR_APP_ARGS,
    CDR_DATE_ORIGIN,
    CDR_DATE_ANSWERED,
    CDR_DATE_FINISH,
    CDR_SECS_CHN_UP,
    CDR_SECS_CHN_COMM,
    CDR_CALL_STATE,
    CDR_DOCS,
    CDR_UTG,
    CDR_NO_USE_b
} cdr_fiels_t;
    

typedef struct {
    char abonado [CDR_FIELD_SIZE];
    char destino [CDR_FIELD_SIZE];
    char fecha_hora [CDR_FIELD_SIZE];
    char segundos [CDR_FIELD_SIZE];
    char estado [CDR_FIELD_SIZE];
} CDR_t;

CDR_t cdr;

//-- Module Functions Declarations ---------------
void cb1 (void *, size_t, void *);
void cb2 (int, void *);
void FlushCDR (void);

//-- Module Functions Definitions ---------------
int main (int argc, char *argv[])
{
    char buf[BUFF_SIZE];
    size_t i;
    struct csv_parser p;
    FILE *infile;
    
    csv_init(&p, 0);

    if (argc != 2)
    {
        fprintf(stderr, "Usage: csv_fix infile\n");
        return EXIT_FAILURE;
    }

    infile = fopen(argv[1], "rb");
    if (infile == NULL) {
        fprintf(stderr, "Failed to open file %s: %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (fgets(buf, BUFF_SIZE, infile) != NULL)        
    {
        i = strlen(buf);
        if (csv_parse(&p, buf, i, cb1, cb2, NULL) != i)
        {
            fprintf(stderr, "Error parsing file: %s\n", csv_strerror(csv_error(&p)));
            fclose(infile);
            remove(argv[2]);
            exit(EXIT_FAILURE);
        }
    }

    //solo una linea para test
    // if ((fgets(buf, BUFF_SIZE, infile)) != NULL)
    // // {
    // //     printf("%s\nbuffer size: %d\n", buf, strlen(buf));
    // // }
    // {
    //     i = strlen(buf);
    //     if (csv_parse(&p, buf, i, cb1, cb2, NULL) != i)
    //     {
    //         fprintf(stderr, "Error parsing file: %s\n", csv_strerror(csv_error(&p)));
    //         fclose(infile);
    //         remove(argv[2]);
    //         exit(EXIT_FAILURE);
    //     }
    // }
    
    csv_fini(&p, cb1, cb2, NULL);
    csv_free(&p);

    if (ferror(infile)) {
        fprintf(stderr, "Error reading from input file");
        fclose(infile);
        remove(argv[2]);
        exit(EXIT_FAILURE);
    }

    fclose(infile);
    
    return EXIT_SUCCESS;
}

//aparentemente es el wrapper del final de campo
//tengo que editarlo segun lo que quiera ahi
//en s tengo el campo
//en i el tamanio
//outfile es el stream o file que quiera usar
void cb1 (void *s, size_t i, void *outfile)
{
    if (put_comma)
        putc(',', stdout);
    csv_fwrite(stdout, s, i);
    put_comma = 1;

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
    put_comma = 0;
    putc('\n', stdout);
    printf("campos: %d lineas: %d\n", campos_por_linea, lineas);

    printf(" abonado: %s\n", cdr.abonado);
    printf(" destino: %s\n", cdr.destino);
    printf(" fecha_hora: %s\n", cdr.fecha_hora);
    printf(" segundos: %s\n", cdr.segundos);
    printf(" estado: %s\n", cdr.estado);    

    FlushCDR();
    campos_por_linea = 0;
    lineas++;

    // fseek((FILE *)outfile, -1, SEEK_CUR);
    // fputc('\n', (FILE *)outfile);
}

void FlushCDR (void)
{
    memset(&cdr, '\0', sizeof(cdr));
}

//--- end of file ---//
