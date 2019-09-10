//----------------------------------------
// Prueba de MACROS preprocesador
// compilador gcc
//----------------------------------------

#include <stdio.h>
#include "pre_proc.h"

int main (void)
{
    printf(FEATURES);

    WELCOME_CODE("test 1");
    printf("\nEl nuevo welcome:\n");
    WelcomeCode("nuevo");

    // printf(FEATURES_2);

    // printf("\nEl multiline:\n");

    // LOG_VA_ARGS(FEATURES,FEATURES_2);

    // printf("El multiline desde el header\n");
    // LOG_VA_ARGS_HARCODED;

    return 0;
}
