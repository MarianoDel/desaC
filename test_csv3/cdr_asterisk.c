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

#ifdef OUTPUT_IS_A_FILE
    FILE *outfile;    
#endif


    csv_init(&p, 0);    
    
    infile = fopen(input, "rb");
    if (infile == NULL) {
        fprintf(stderr, "Failed to open file %s: %s\n", input, strerror(errno));
        exit(EXIT_FAILURE);
    }

#ifdef OUTPUT_IS_A_FILE
    outfile = fopen(output, "w");
    if (outfile == NULL) {
        fprintf(stderr, "Failed to open file %s: %s\n", output, strerror(errno));
        exit(EXIT_FAILURE);
    }    
#endif
    
    while (fgets(buf, BUFF_SIZE, infile) != NULL)        
    {
        i = strlen(buf);
#ifdef OUTPUT_IS_A_FILE
        if (csv_parse(&p, buf, i, cb1, cb2, outfile) != i)
        {
            fprintf(stderr, "Error parsing file: %s\n", csv_strerror(csv_error(&p)));
            fclose(infile);
            fclose(outfile);            
            exit(EXIT_FAILURE);
        }
#else
        if (csv_parse(&p, buf, i, cb1, cb2, NULL) != i)
        {
            fprintf(stderr, "Error parsing file: %s\n", csv_strerror(csv_error(&p)));
            fclose(infile);
            exit(EXIT_FAILURE);
        }
#endif
    }

#ifdef ONLY_ONE_LINE_FOR_TEST
    // solo una linea para test
    if ((fgets(buf, BUFF_SIZE, infile)) != NULL)
    // {
    //     printf("%s\nbuffer size: %d\n", buf, strlen(buf));
    // }
    {
        i = strlen(buf);
#ifdef OUTPUT_IS_A_FILE
        if (csv_parse(&p, buf, i, cb1, cb2, outfile) != i)
        {
            fprintf(stderr, "Error parsing file: %s\n", csv_strerror(csv_error(&p)));
            fclose(infile);
            fclose(outfile);            
            exit(EXIT_FAILURE);
        }
#else
        if (csv_parse(&p, buf, i, cb1, cb2, NULL) != i)
        {
            fprintf(stderr, "Error parsing file: %s\n", csv_strerror(csv_error(&p)));
            fclose(infile);
            exit(EXIT_FAILURE);
        }
#endif
    }
#endif

#ifdef OUTPUT_IS_A_FILE
    csv_fini(&p, cb1, cb2, outfile);
#else
    csv_fini(&p, cb1, cb2, NULL);
#endif
    csv_free(&p);

    // if (ferror(infile)) {
    //     fprintf(stderr, "Error reading from input file");
    //     fclose(infile);
    //     remove(argv[2]);
    //     exit(EXIT_FAILURE);
    // }
#ifdef OUTPUT_IS_A_FILE
    fclose(outfile);
#endif
    fclose(infile);

    return EXIT_SUCCESS;    
}

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
#ifdef OUTPUT_IS_A_FILE

    fprintf((FILE *) outfile, "%22s,%22s,%s,%s,%s\n",
            cdr.abonado,
            cdr.destino,
            cdr.fecha_hora,
            cdr.segundos,
            cdr.estado);
    
#endif
    
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

}

void FlushCDR (void)
{
    memset(&cdr, '\0', sizeof(cdr));
}

//--- end of file ---//

