//----------------------------------------
// Prueba de filtro DSP
// Prueba de Lazo PID
// compilador gcc
//----------------------------------------

#include <stdio.h>

// Module Config Defines ------------------------------
// #define TEST_MA_CIRCULAR
// #define TEST_DIVISORS
// #define TEST_PID
#define TEST_PID_SMALL_KI

#ifdef TEST_PID
#define TEST_PID_LENGTH    300
#endif
#ifdef TEST_PID_SMALL_KI
#define TEST_PID_LENGTH    600
#endif


// Module Constants ------------------------------------
#define ABS_VALUE(X)    do { if ((X) < 0) \
                                 (X) = -(X); \
                           } while (0)



// Module Types ------------------------------
typedef struct {
    short setpoint;
    short sample;
    short last_d;
    short error_z1;
    short error_z2;
    short ki_accumulator;
    unsigned short kp;
    unsigned short ki;
    unsigned short kd;
} pid_data_obj_t;


// Module Globals ------------------------------
unsigned short * p_ma8;
unsigned short v_ma8 [8];


// Module Functions Declarations -----------------------------------
void MA8Circular_Start (void);
void MA8Circular_Reset (void);
unsigned short MA8Circular (unsigned short);

void PID_Flush_Errors (pid_data_obj_t *);
short PID (pid_data_obj_t *);
void PID_Small_Ki_Flush_Errors (pid_data_obj_t *);
short PID_Small_Ki (pid_data_obj_t *);

// Module Functions Definitions -----------------------------------
int main (void)
{
#ifdef TEST_DIVISORS
    short number = -1;
    short div = 128;
    short result = 0;
    short desplazamiento = 7;
    
    result = number / div;
    printf("La division entera %d / %d = %d\n",
           number,
           div,
           result);

    result = number >> desplazamiento;
    printf("El desplazamiento a la derecha de %d en %d veces es = %d\n",
           number,
           desplazamiento,
           result);

#endif

#ifdef TEST_PID_SMALL_KI
    // unsigned short input [TEST_PID_LENGTH] = {[0 ... (TEST_PID_LENGTH - 1)] =  900 };
    unsigned short input [TEST_PID_LENGTH] = { 0 };    
    short output [TEST_PID_LENGTH] = { 0 };

    pid_data_obj_t voltage_pid;
    // voltage_pid.kp = 128;
    // voltage_pid.ki = 0;
    voltage_pid.kd = 0;
    voltage_pid.kp = 0;
    voltage_pid.ki = 1;
    // voltage_pid.kd = 3 * 128;
    voltage_pid.setpoint = 100;

    PID_Small_Ki_Flush_Errors(&voltage_pid);    
    printf ("Prueba PID Small KI %d puntos\n", TEST_PID_LENGTH);

    //armo el input
    for (int i = 0; i < (TEST_PID_LENGTH / 2); i++)
    {
        input[i] = 100;
    }

    for (int i = (TEST_PID_LENGTH/2); i < TEST_PID_LENGTH; i++)
    {
        input[i] = 101;
    }

    for (int i = 0; i < TEST_PID_LENGTH; i++)
    {
        voltage_pid.sample = input[i];
        output[i] = PID_Small_Ki (&voltage_pid);
    }

    printf("input[%d]:\n", TEST_PID_LENGTH);
    for (int i = 0; i < TEST_PID_LENGTH; i++)
    {
        printf("%d ", input[i]);
    }
    printf("\n");

    printf("output[%d]:\n", TEST_PID_LENGTH);
    for (int i = 0; i < TEST_PID_LENGTH; i++)
    {
        printf("%d ", output[i]);
    }
    printf("\n");
    
#endif

#ifdef TEST_PID
    // unsigned short input [TEST_PID_LENGTH] = {[0 ... (TEST_PID_LENGTH - 1)] =  900 };
    unsigned short input [TEST_PID_LENGTH] = { 0 };    
    short output [TEST_PID_LENGTH] = { 0 };

    pid_data_obj_t voltage_pid;
    // voltage_pid.kp = 128;
    // voltage_pid.ki = 0;
    voltage_pid.kd = 0;
    voltage_pid.kp = 0;
    voltage_pid.ki = 42;
    // voltage_pid.kd = 3 * 128;
    voltage_pid.setpoint = 100;

    PID_Flush_Errors(&voltage_pid);
    printf ("Prueba PID %d puntos, setpoint: %d\n", TEST_PID_LENGTH, voltage_pid.setpoint);

    //armo el input
    for (int i = 0; i < (TEST_PID_LENGTH / 2); i++)
    {
        input[i] = 100;
    }

    for (int i = (TEST_PID_LENGTH/2); i < TEST_PID_LENGTH; i++)
    {
        input[i] = 103;
        // input[i] = 98;        
    }

    for (int i = 0; i < TEST_PID_LENGTH; i++)
    {
        voltage_pid.sample = input[i];
        output[i] = PID (&voltage_pid);
    }

    printf("input[%d]:\n", TEST_PID_LENGTH);
    for (int i = 0; i < TEST_PID_LENGTH; i++)
    {
        printf("%d ", input[i]);
    }
    printf("\n");

    printf("output[%d]:\n", TEST_PID_LENGTH);
    for (int i = 0; i < TEST_PID_LENGTH; i++)
    {
        printf("%d ", output[i]);
    }
    printf("\n");
    
#endif
    
#ifdef TEST_MA_CIRCULAR
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
#endif
    
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

#define PID_CONSTANT_DIVIDER    7
short PID (pid_data_obj_t * p)
{
    int acc = 0;
    short error = 0;
    short d = 0;

    unsigned short k1 = 0;
    unsigned short k2 = 0;
    unsigned short k3 = 0;
    
    short val_k1 = 0;
    short val_k2 = 0;
    short val_k3 = 0;

    k1 = p->kp + p->ki + p->kd;
    k2 = p->kp + p->kd + p->kd;
    k3 = p->kd;
    
    error = p->setpoint - p->sample;

    //K1
    acc = k1 * error;
    // if (acc < 0)
    // {
    //     acc = -acc;
    //     val_k1 = acc >> PID_CONSTANT_DIVIDER;
    //     val_k1 = -val_k1;
    // }
    // else
        // val_k1 = acc >> PID_CONSTANT_DIVIDER;
    val_k1 = acc / 128;

    //K2
    acc = k2 * p->error_z1;
    // if (acc < 0)
    // {
    //     acc = -acc;
    //     val_k2 = acc >> PID_CONSTANT_DIVIDER;
    //     val_k2 = -val_k2;
    // }
    // else
        // val_k2 = acc >> PID_CONSTANT_DIVIDER;
    val_k2 = acc / 128;    

    //K3
    acc = k3 * p->error_z2;
    // if (acc < 0)
    // {
    //     acc = -acc;
    //     val_k3 = acc >> PID_CONSTANT_DIVIDER;
    //     val_k3 = -val_k3;
    // }
    // else
        // val_k3 = acc >> PID_CONSTANT_DIVIDER;
    val_k3 = acc / 128;

    d = p->last_d + val_k1 - val_k2 + val_k3;

    //Update PID variables
    p->error_z2 = p->error_z1;
    p->error_z1 = error;
    p->last_d = d;

    return d;
}


void PID_Flush_Errors (pid_data_obj_t * p)
{
    p->last_d = 0;
    p->error_z1 = 0;
    p->error_z2 = 0;
}

short PID_Small_Ki (pid_data_obj_t * p)
{
    int acc = 0;
    short error = 0;
    short d = 0;

    unsigned short k1_small = 0;
    unsigned short k2 = 0;
    unsigned short k3 = 0;
    
    short val_k1 = 0;
    short val_k2 = 0;
    short val_k3 = 0;

    // k1 = p->kp + p->ki + p->kd;
    k2 = p->kp + p->kd + p->kd;
    k3 = p->kd;
    
    error = p->setpoint - p->sample;

    //K1 -- desarmo K1 para mayor definicion del integral
    p->ki_accumulator += p->ki * error;
    k1_small = p->kp + p->kd;
    acc = k1_small * error + p->ki_accumulator;
    // if (p->ki_accumulator > 127)
    // {
    //     p->ki_accumulator = 0;
    // }
    // else if (p->ki_accumulator < -127)
    // {
    //     p->ki_accumulator = 0;
    // }

    if ((p->ki_accumulator > 127) || (p->ki_accumulator < -127))
        p->ki_accumulator = 0;
    
    val_k1 = acc / 128;

    //K2
    acc = k2 * p->error_z1;
    val_k2 = acc / 128;

    //K3
    acc = k3 * p->error_z2;
    val_k3 = acc / 128;

    d = p->last_d + val_k1 - val_k2 + val_k3;

    //Update PID variables
    p->error_z2 = p->error_z1;
    p->error_z1 = error;
    p->last_d = d;

    return d;
}


void PID_Small_Ki_Flush_Errors (pid_data_obj_t * p)
{
    p->last_d = 0;
    p->ki_accumulator = 0;
    p->error_z1 = 0;
    p->error_z2 = 0;
}

//--- end of file ---//
