//----------------------------------------
// Prueba de MACROS preprocesador
// compilador gcc
//----------------------------------------
#ifndef _PRE_PROC_H_
#define _PRE_PROC_H_

// Stringify Utils --------------------
#define STRING_CONCAT(str1,str2) #str1 " " #str2
#define xstr(s) str(s)
#define str(s) #s

#define CURRENT_MODE        Current Mode for Hwd ver 1.2\n
#define WITH_OVER           WITH_OVERCURRENT_SHUTDOWN\n


#define FEATURES STRING_CONCAT(Current Mode for Hwd ver 1.2\n, WITH_OVERCURRENT_SHUTDOWN\n)
#define FEATURES_2 STRING_CONCAT(xstr(CURRENT_MODE), xstr(WITH_OVER))

#pragma message FEATURES
#pragma message FEATURES_2

#endif
//--- end of file ---//
