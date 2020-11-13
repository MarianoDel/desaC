//--------------------------------------------------------------------
// Prueba de algunos algoritmos
// compilador gcc
//--------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

// Defines ---------------------------------------------------------------------
#define SEGMENTS_QTTY    16
unsigned char const_segments[SEGMENTS_QTTY] = {15, 31, 47, 63, 79, 95, 111, 127,
                                                143, 159, 175, 191, 207, 223, 239, 255};
#define SEGMENTS_VALUE    16

unsigned short mem_conf_segments[SEGMENTS_QTTY] = {49, 266, 274, 381, 527, 615, 735, 755,
                                                   774, 793, 811, 829, 846, 864, 880, 897};

// Module Declarations ---------------------------------------------------------
unsigned char GetProcessedSegment (unsigned char);
unsigned short HARD_Map_New_DMX_Data (unsigned short * p_seg, unsigned char, unsigned char);


// Module Definitions ----------------------------------------------------------
int main (int argc, char * argv[])
{

    for (unsigned short i = 0; i <= 255; i++)
    // for (unsigned short i = 90; i <= 110; i++)        
    {
        printf("dmx: %3d mapped to: %3d\n", i, HARD_Map_New_DMX_Data (
                   mem_conf_segments,
                   (unsigned char) i,
                   6));
               
    }

    //test processed segment
    // for (unsigned short i = 0; i <= 255; i++)
    // {
    //     printf("i: %3d seg: %2d\n",
    //            i,
    //            GetProcessedSegment((unsigned char) i));
    // }

    // printf("segments[%d] ", SEGMENTS_QTTY);
    // for (unsigned char i = 0; i < SEGMENTS_QTTY; i++)
    //     printf("%d ", const_segments[i]);

    // printf("\n");
    
    return 0;
}


unsigned short HARD_Map_New_DMX_Data (unsigned short * p_seg, unsigned char dmx_data, unsigned char slow_seg)
{
    unsigned char segment_number = 0;
    unsigned int dummy = 0;
                    
    //mapeo los segmentos
    segment_number = GetProcessedSegment(dmx_data);

    if (segment_number < slow_seg)
    {
        //the segment is less or equal to (ch_slow_segment - 1)

        dummy = dmx_data * (p_seg[slow_seg - 1]);
        dummy /= const_segments[slow_seg - 1];        
    }
    else if (segment_number == slow_seg)
    {
        //segment is equal to ch_slow_segment

        dummy = p_seg[slow_seg] - p_seg[slow_seg - 1];
        dummy = dummy * (dmx_data - const_segments[slow_seg - 1]);
        dummy /= const_segments[slow_seg] - const_segments[slow_seg - 1];
        dummy += p_seg[slow_seg - 1];
    }
    else
    {
        //segment is at the end on CCM mode

        dummy = p_seg[SEGMENTS_QTTY - 1] - p_seg[slow_seg];
        dummy = dummy * (dmx_data - const_segments[slow_seg]);
        dummy /= const_segments[SEGMENTS_QTTY - 1] - const_segments[slow_seg];
        dummy += p_seg[slow_seg];
    }

    // if (pwm_output > DUTY_MAX_ALLOWED)
    //     pwm_output = DUTY_MAX_ALLOWED;    
    // return pwm_output;

    return (unsigned short) dummy;
}


unsigned char GetProcessedSegment (unsigned char check_segment_by_value)
{
    unsigned char * s;
    s = const_segments;
    
    for (unsigned char i = 0; i < SEGMENTS_QTTY; i++)
    {
        if (check_segment_by_value <= *(s + i))
            return i;
    }
    
    return 0;
}


//--- end of file ---//
