//----------------------------------------
// Prueba de filtro DSP
// compilador gcc
//----------------------------------------

#include <stdio.h>

// Module Globals ------------------------------
unsigned short * p_ma8;
unsigned short v_ma8 [8];

// Module Functions -----------------------------------
void MA8Circular_Start (void);
void MA8Circular_Reset (void);
unsigned short MA8Circular (unsigned short);


int main (void)
{
    printf ("Aplico Start\n");
    MA8Circular_Start();

    printf ("Aplico Reset\n");
    MA8Circular_Reset();

    for (int i = 0; i < 16; i++)
    {
        printf("vuelta: %d resultado: %d\n", i, MA8Circular(1000));
    }

    printf ("Aplico Reset\n");
    MA8Circular_Reset();

    for (int i = 0; i < 16; i++)
    {
        printf("vuelta: %d resultado: %d\n", i, MA8Circular(4095));
    }
    
    return 0;
}

void MA8Circular_Start (void)
{
    p_ma8 = &v_ma8[0];
}

//reset de punteros al filtro circular
void MA8Circular_Reset (void)
{
    unsigned char i;
    
    MA8Circular_Start();
    for (i = 0; i < 8; i++)
        *(p_ma8 + i) = 0;
}

//Filtro circular, necesito activar previamente con MA8Circular_Start()
//MA8Circular_Reset() vacia el filtro
//recibe: new_sample
//contesta: resultado
unsigned short MA8Circular (unsigned short new_sample)
{
    unsigned int total_ma;

    *p_ma8 = new_sample;

    total_ma = *(v_ma8) + *(v_ma8 + 1) + *(v_ma8 + 2) +
        *(v_ma8 + 3) + *(v_ma8 + 4) + *(v_ma8 + 5) +
        *(v_ma8 + 6) + *(v_ma8 + 7);

    if (p_ma8 < (v_ma8 + 7))
        p_ma8 += 1;
    else
        p_ma8 = &v_ma8[0];

    return (unsigned short) (total_ma >> 3);
}
