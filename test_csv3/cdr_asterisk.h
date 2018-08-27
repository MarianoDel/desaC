//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CDR_ASTERISK.H ########################
//---------------------------------------------

#ifndef _CDR_ASTERISK_H_
#define _CDR_ASTERISK_H_

//----------- Defines For Configuration -----------------
#define ASTERISK_CDR_WORKING_TMP_DIR    "./tmp"
#define ASTERISK_CDR_SECLAMA_FILE    "seclama.log"
#define ASTERISK_CDR_SECLAMA_DIR    "."

// #define ONLY_ONE_LINE_FOR_TEST
#define OUTPUT_IS_A_FILE


//-- Exported Defines -----------------------------------
#define CDR_FIELD_SIZE    40

typedef struct {
    char abonado [CDR_FIELD_SIZE];
    char destino [CDR_FIELD_SIZE];
    char fecha_hora [CDR_FIELD_SIZE];
    char segundos [CDR_FIELD_SIZE];
    char estado [CDR_FIELD_SIZE];
} CDR_t;

CDR_t cdr;

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




//-- Exported Module Functions --------------------------
int AsteriskCDRProcessFile (char * , char * );
int AsteriskGetSecLama (void);

#endif    //_CDR_ASTERISK_H_

//--- end of file ---//
