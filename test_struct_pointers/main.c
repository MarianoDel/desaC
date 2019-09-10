//----------------------------------------
// Prueba de filtro DSP con estructura de objetos
// compilador gcc
//----------------------------------------

#include <stdio.h>


// Module Types --------------------------------
typedef struct {
    unsigned short v_ma8[8];
    unsigned short * p_ma8;
    unsigned int total_ma8;
} ma8_data_obj_t;


// Module Globals ------------------------------

// Module Functions -----------------------------------
void MA8Circular_Reset (ma8_data_obj_t *);
unsigned short MA8Circular (ma8_data_obj_t *, unsigned short);

// Module Function Definitions -------------------------
int main (void)
{
    ma8_data_obj_t primer_filtro_st;
    
    printf ("Aplico Reset\n");
    MA8Circular_Reset(&primer_filtro_st);

    for (int i = 0; i < 16; i++)
    {
        printf("vuelta: %d resultado: %d\n", i, MA8Circular(&primer_filtro_st, 1000));
    }

    printf ("Aplico Reset\n");
    MA8Circular_Reset(&primer_filtro_st);

    for (int i = 0; i < 16; i++)
    {
        printf("vuelta: %d resultado: %d\n", i, MA8Circular(&primer_filtro_st, 4095));
    }
    
    return 0;
}


//set de punteros y vaciado del filtro
//recibe:
// puntero a estructura de datos del filtro "ma8_data_obj_t *"
void MA8Circular_Reset (ma8_data_obj_t * p_data)
{
    unsigned char i;
    
    for (i = 0; i < 8; i++)
        p_data->v_ma8[i] = 0;

    p_data->p_ma8 = p_data->v_ma8;
    p_data->total_ma8 = 0;
}

//Filtro circular, necesito activar previamente con MA8Circular_Reset()
//recibe:
// puntero a estructura de datos del filtro "ma8_data_obj_t *"
// nueva mustra "new_sample"
//contesta:
// resultado del filtro
unsigned short MA8Circular (ma8_data_obj_t *p_data, unsigned short new_sample)
{
    p_data->total_ma8 -= *(p_data->p_ma8);
    p_data->total_ma8 += new_sample;
    *(p_data->p_ma8) = new_sample;

    if (p_data->p_ma8 < ((p_data->v_ma8) + 7))
        p_data->p_ma8 += 1;
    else
        p_data->p_ma8 = p_data->v_ma8;

    return (unsigned short) (p_data->total_ma8 >> 3);    
}
