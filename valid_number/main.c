//----------------------------------------
// www.leetcode.com
//----------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool isNumber(char * s);
int myisNumber(char *);
int isSign(char *);
int checkBigNumber (char *, int);

int main (void)
{
    char a [] = {" 96.5e-3 "};
    if (isNumber(a))
        printf("=> true\n");
    else
        printf("=> false\n");

    return 0;
}

//----------------------------------------
// Start Solution from this point
//----------------------------------------
bool isNumber(char * s)
{
    int number_index = -1;
    int sign_index = -1;    
    int len = 0;
    int number_len = 0;

    len = strlen(s);

    //check for raw validity
    for (int i = 0; i < len; i++)
    {
        if (myisNumber(str+i))
        {
                get_some_zeroes = 1;
                if (*(str+i) != '0')
                {
                    number_index = i;
                    number_len++;
                }
        }
        else if (isSign(str+i))
        {
                if (sign_index < 0)
                    sign_index = i;
                else
                    return 0;
            }
            else if ((*(str+i) != ' ') || (get_some_zeroes) || (sign_index >= 0))
                return 0;
        }
        else
        {
            return false;
        }
    }


}

int myAtoi(char * str)
{
    int number_index = -1;
    int sign_index = -1;    
    int len = 0;
    int number_len = 0;
    int get_some_zeroes = 0;

    len = strlen(str);

    //check for sign
    for (int i = 0; i < len; i++)
    {
        if (number_index < 0)
        {
            if (isNumber(str+i))
            {
                get_some_zeroes = 1;
                if (*(str+i) != '0')
                {
                    number_index = i;
                    number_len++;
                }
            }
            else if ((isSign(str+i)) && (!get_some_zeroes))
            {
                if (sign_index < 0)
                    sign_index = i;
                else
                    return 0;
            }
            else if ((*(str+i) != ' ') || (get_some_zeroes) || (sign_index >= 0))
                return 0;
        }
        else
        {
            //got the index numbers and maybe sign
            if (isNumber(str+i))
                number_len++;
            else
                i = len;
        }
    }

    printf("str: %s, str_len: %d, number_index: %d, number_len %d, sign_index: %d\n",
           str,
           len,
           number_index,
           number_len,
           sign_index);


    //no numbers in the string
    if (number_index < 0)
        return 0;

    int is_positive = 0;
    if (sign_index < 0)    //no sign must be positive
        is_positive = 1;
    else
    {
        if (*(str+sign_index) == '+')
            is_positive = 1;
    }

    
    //number is too big
    if (number_len > 10)
    {
        if (is_positive)
            return INT_MAX;
        else
            return INT_MIN;
    }

    if (number_len == 10)
    {
        if (is_positive)
        {
            if (checkBigNumber((str+number_index), 1) == 0)
                return INT_MAX;
        }
        else
        {
            if (checkBigNumber((str+number_index), 0) == 0)
                return INT_MIN;
        }
    }
        
    
    unsigned int total = 0;
    if (number_len <= 10)
    {
        for (int i = number_index; i < (number_index + number_len); i++)
        {
            unsigned int multiplier = 1;
            for (int j = 0; j < (number_len + number_index - i - 1); j++)
                multiplier = multiplier * 10;
            
            printf("multiplier: %d\n", multiplier);
            int to_mult = *(str+i) - '0';
            total += to_mult * multiplier;
            printf("total: %d\n", total);
        }
        printf("total: %d\n", total);
    }

    long long int ret_val = 0;
    
    if (is_positive)
        ret_val = total;
    else
        ret_val = -total;
    
    return (int) ret_val;
}

int myisNumber(char * str)
{
    if ((*str >= '0') && (*str <= '9'))
        return 1;

    return 0;
}

int isSign(char * str)
{
    if ((*str >= '+') && (*str <= '-'))
        return 1;

    return 0;
}

int checkBigNumber (char * s_value, int sign_positive)
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
                                        if (sign_positive)
                                        {
                                            if (s_value[9] > '7')
                                                return 0;
                                        }
                                        else
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
    return 1;
}    

