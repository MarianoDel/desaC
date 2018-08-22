//----------------------------------------
// Prueba de MACROS preprocesador
// compilador gcc
//----------------------------------------

#include <stdio.h>
#include "pre_proc.h"

int main (void)
{
    printf(FEATURES);

    printf("\nEl nuevo:\n");

    printf(FEATURES_2);

    return 0;
}
