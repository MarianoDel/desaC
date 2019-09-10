//--------------------------------------------------------------------
// Prueba de algos CRC
// compilador gcc
// info en esta carpeta y en el link:
// http://www.sunshine2k.de/articles/coding/crc/understanding_crc.html
//
// online calculator for check
// https://www.lammertbies.nl/comm/info/crc-calculation.html
//--------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

// Defines ---------------------------------------------------------------------
#define CRC8_POLY    0x1D
#define CRC16_POLY    0x1021


// Module Declarations ---------------------------------------------------------
unsigned char Compute_CRC8_Simple_OneByte (unsigned char);
unsigned char Compute_CRC8_Simple(unsigned char *, unsigned char);
unsigned short Compute_CRC16_Simple(unsigned char *, unsigned char);

// Module Definitions ----------------------------------------------------------
int main (int argc, char * argv[])
{
    unsigned char value_u8 = 0;
    unsigned short value_u16 = 0;
    unsigned char value[2] = { 0 };
    
    if (argc < 2)
    {
        printf("ingrese los numeros a calcular\n");
        return -1;
    }

    // printf("Prueba CRC polynomial: 0x%x, data: %s\n",
    //        CRC8_POLY,
    //        argv[1]);
    // value_u8 = Compute_CRC8_Simple_OneByte (value);
    
    printf("Prueba CRC polynomial: 0x%x,", CRC8_POLY);
    for (int i = 1; i < argc; i++)
        printf(" d in: %s ", argv[i]);

    printf("\n");

    value[0] = (unsigned char) atoi(argv[1]);
    value[1] = (unsigned char) atoi(argv[2]);

    value_u8 = Compute_CRC8_Simple(value, 2);
    printf("CRC8 output: 0x%02x\n", value_u8);

    value_u16 = Compute_CRC16_Simple(value, 2);
    printf("CRC16 output: 0x%04x\n", value_u16);

    return 0;
}

unsigned char Compute_CRC8_Simple_OneByte (unsigned char byteVal)
{
    // init crc directly with input byte
    unsigned char crc = byteVal; 

    for (int i = 0; i < 8; i++)
    {
        if ((crc & 0x80) != 0)
        {
            // most significant bit set
            // shift crc register and perform XOR operation
            // taking not-saved 9th set bit into account
            crc = (unsigned char)((crc << 1) ^ CRC8_POLY);
        }
        else
        {
            // most significant bit not set, go to next bit
            crc <<= 1;
        }
    }

    return crc;
}

unsigned char Compute_CRC8_Simple(unsigned char * data_8, unsigned char len)
{
    // start with 0 so first unsigned char can be 'xored' in
    unsigned char crc = 0;

    for (int j = 0; j < len; j++)
    {
        // XOR-in the next input unsigned char
        crc ^= *(data_8 + j);

        for (int i = 0; i < 8; i++)
        {
            if ((crc & 0x80) != 0)
            {
                crc = (unsigned char)((crc << 1) ^ CRC8_POLY);
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}


unsigned short Compute_CRC16_Simple(unsigned char * data_8, unsigned char len)
{
    unsigned short crc = 0;

    for (unsigned char j = 0; j < len; j++)
    {
        // move byte into MSB of 16bit CRC
        crc ^= (*(data_8 + j) << 8);

        for (int i = 0; i < 8; i++)
        {
            // test for MSB = bit 15
            if ((crc & 0x8000) != 0)
            {
                crc = ((crc << 1) ^ CRC16_POLY);
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}
//--- end of file ---//
