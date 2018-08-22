//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CDR_OMC.H #############################
//---------------------------------------------
#ifndef _CDR_OMC_H_
#define _CDR_OMC_H_

//----------- Defines For Configuration -----------------
#define CDR_WORKING_TMP_DIR    "./tmp"
#define CDR_WORKING_TMP_FILE    "normal_fecha.dbs"
// #define CDR_DIR    "/var/log/"
#define CDR_BKP_DIR    "./tmp"
#define CDR_CURRENT_DIR    "."
#define CDR_FILE    "Master.csv"
#define CDR_UTIL_CAT    "/usr/bin/cat"


//-- Exported Module Functions --------------------------
int CreateCdrFromDates (char *, char *, char *);

#endif    //_CDR_OMC_H_

//--- end of file ---//
