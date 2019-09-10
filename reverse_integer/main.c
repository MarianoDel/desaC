//----------------------------------------
// www.leetcode.com
//----------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int reverse(int x);

int main (void)
{
    int a = 1534236469;
    printf("result: %d\n", reverse(a));
    return 0;
}

//----------------------------------------
// Start Solution from this point
//----------------------------------------
int reverse(int x)
{
    int neg = 0;
    int len = 0;
    int value = 0;
    char s_value [32] = {'\0'};

    if (x == -2147483648)
        return 0;

    if (x < 0)
    {
        neg = 1;
        x = -x;
    }

    sprintf(s_value, "%d", x);
    len = strlen(s_value);

    printf("lenght: %d, s_value: %s\n", len, s_value);
    char a;
    if (len > 2)
    {
        for (int i = 0; i <= ((len/2) - 1); i++)
        {
            a = s_value[i];
            s_value[i] = s_value[len-i-1];
            s_value[len-i-1] = a;
            printf("i: %d, val: %s\n", i, s_value);
        }
    }
    else if (len == 2)
    {
        a = s_value[0];
        s_value[0] = s_value[1];
        s_value[1] = a;
    }

    if (len > 10)
        return 0;

    if (len == 10)
    {
        if (s_value[0] > '2')
            return 0;
        else if (s_value[0] == '2')
        {
            if (s_value[1] > '1')
                return 0;
            else if (s_value[1] == '1')
            {
                if (s_value[2] > '4')
                    return 0;
                else if (s_value[2] == '4')
                {
                    if (s_value[3] > '7')
                        return 0;
                    else if (s_value[3] == '7')
                    {
                        if (s_value[4] > '4')
                            return 0;
                        else if (s_value[4] == '4')
                        {
                            if (s_value[5] > '8')
                                return 0;
                            else if (s_value[5] == '8')
                            {
                                if (s_value[6] > '3')
                                    return 0;
                                else if (s_value[6] == '3')
                                {
                                    if (s_value[7] > '6')
                                        return 0;
                                    else if (s_value[7] == '6')
                                    {
                                        if (s_value[8] > '4')
                                            return 0;
                                        else if (s_value[8] == '4')
                                        {
                                            if (s_value[9] > '8')
                                                return 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    value = atoi(s_value);
    if (neg)
        value = -value;

    return value;
}
