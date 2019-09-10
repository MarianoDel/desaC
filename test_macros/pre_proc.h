//----------------------------------------
// Prueba de MACROS preprocesador
// compilador gcc
//----------------------------------------
#ifndef _PRE_PROC_H_
#define _PRE_PROC_H_

// Stringify Utils --------------------
#define STRING_CONCAT(str1,str2) #str1 " " #str2
#define STRING_CONCAT_NEW_LINE(str1,str2) xstr(str1) " " #str2 "\n"
// #define STRING_CONCAT_VA_ARGS( ... ) ##__VA_ARGS__
#define xstr(s) str(s)
#define str(s) #s

#define LOG_VA_ARGS( ... )    do {\
        printf("[%s] %s\n", __FILE__, xstr(__VA_ARGS__));       \
    } while (0)


#define C1 "current mode"
#define C2 "voltage mode"
#define CURRENT_MODE        Current Mode for Hwd ver 1.2
#define WITH_OVER           WITH_OVERCURRENT_SHUTDOWN

#define LOG_VA_ARGS_HARCODED LOG_VA_ARGS(C1,C2)
//este va bien siempre que el define sea distinto del argumento
// #define FEATURES STRING_CONCAT_NEW_LINE(WITH_OVER, WITH_OVERCURRENT_SHUTDOWN)
//este va bien siempre que el define sea distinto del argumento
#define FEATURES STRING_CONCAT_NEW_LINE(CURRENT_MODE, WITH_OVERCURRENT_SHUTDOWN)
#define FEATURES_2 STRING_CONCAT(xstr(CURRENT_MODE), xstr(WITH_OVER))


#pragma message FEATURES
#pragma message FEATURES_2

//Welcome Code
// #define WELCOME_CODE(X)     do {                                \
//         printf("[%s] %s\n", __FILE__, (X));                     \
//         // #ifdef CURRENT_MODE \
//         // printf("[%s] %s\n", __FILE__, str(CURRENT_MODE));       \
//         // #endif \
//     } while (0)
#define WELCOME_CODE(X)     do {                                \
        printf("[%s] %s\n", __FILE__, (X));                     \
    } while (0)

void WelcomeCode (char * str)
{
#ifdef CURRENT_MODE
    printf("[%s] %s\n", __FILE__, str(CURRENT_MODE));
#endif
#ifdef VOLTAGE_MODE
    printf("[%s] %s\n", __FILE__, str(VOLTAGE_MODE));
#endif
#ifdef FEATURES
    printf("[%s] %s\n", __FILE__, str(FEATURES));
#endif
}

#endif
//--- end of file ---//
