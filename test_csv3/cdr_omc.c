//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CDR_OMC.C #############################
//---------------------------------------------
#include "cdr_omc.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>
#include <errno.h>

// Documentation for the record
//struct tm on time.h
// struct tm {
//    int tm_sec;         /* seconds,  range 0 to 59          */
//    int tm_min;         /* minutes, range 0 to 59           */
//    int tm_hour;        /* hours, range 0 to 23             */
//    int tm_mday;        /* day of the month, range 1 to 31  */
//    int tm_mon;         /* month, range 0 to 11             */
//    int tm_year;        /* The number of years since 1900   */
//    int tm_wday;        /* day of the week, range 0 to 6    */
//    int tm_yday;        /* day in the year, range 0 to 365  */
//    int tm_isdst;       /* daylight saving time             */
// };

//-- Module Private Functions -----------------------------
#define DiffTimeTm(X,Y)    DiffTime(X.tm_year,X.tm_mon,X.tm_mday,Y.tm_year,Y.tm_mon,Y.tm_mday)

int DiffTime (int, int, int, int, int, int);
//-- Module Functions ---------------------------------

//dates must be string DD/MM/YY
int CreateCdrFromDates2 (char * first_date, char * last_date, char * dest_file)
{
    char fini [20];
    char fend [20];    

    bzero(fini, sizeof(fini));
    strcpy(fini, "2000");
    strncpy((fini + 2), (first_date + 6), 2);
    strncpy((fini + 4), (first_date + 3), 2);
    strncpy((fini + 6), (first_date), 2);    

    bzero(fend, sizeof(fend));
    strcpy(fend, "2000");
    strncpy((fend + 2), (last_date + 6), 2);
    strncpy((fend + 4), (last_date + 3), 2);
    strncpy((fend + 6), (last_date), 2);    

    // printf ("dest_file: %s\n", dest_file);
    // printf ("fini: %s, fend: %s\n", fini, fend);
    // return 0;
    
    return CreateCdrFromDates (fini, fend, dest_file);          
}

//dates must be string YYYYMMDD
int CreateCdrFromDates (char * first_date, char * last_date, char * dest_file)
{
    char buf [10];
    int one_bkp_finded = 0;
    int only_this_day = 0;
    int today_backup = 0;

    struct tm today_tm;
    struct tm *ptoday_tm;
    time_t today_time;

    struct tm first_date_tm;
    struct tm last_date_tm;

    bzero (&first_date_tm, sizeof(struct tm));
    //sanity checks
    //last_date >= first_date
    memset(buf, '\0', sizeof(buf));    
    strncpy (buf, first_date, 4);
    first_date_tm.tm_year = atoi (buf) - 1900;
    memset(buf, '\0', sizeof(buf));
    strncpy (buf, (first_date + 4), 2);
    first_date_tm.tm_mon = atoi (buf) - 1;
    memset(buf, '\0', sizeof(buf));    
    strncpy (buf, (first_date + 6), 2);
    first_date_tm.tm_mday = atoi (buf);

    bzero (&last_date_tm, sizeof(struct tm));
    memset(buf, '\0', sizeof(buf));    
    strncpy (buf, last_date, 4);
    last_date_tm.tm_year = atoi (buf) - 1900;
    memset(buf, '\0', sizeof(buf));
    strncpy (buf, (last_date + 4), 2);
    last_date_tm.tm_mon = atoi (buf) - 1;
    memset(buf, '\0', sizeof(buf));    
    strncpy (buf, (last_date + 6), 2);
    last_date_tm.tm_mday = atoi (buf);
    
    //last_date == today
    today_time = time (NULL);
    ptoday_tm = localtime (&today_time);    //esto debe estar creando una const struct tm escondida

    printf("year: %d month: %d day: %d\n",
           ptoday_tm->tm_year + 1900,
           ptoday_tm->tm_mon + 1,
           ptoday_tm->tm_mday);    

    today_tm.tm_year = ptoday_tm->tm_year;
    today_tm.tm_mon = ptoday_tm->tm_mon;
    today_tm.tm_mday = ptoday_tm->tm_mday;
    
    char bkp_day [10];
    char bkp_cmd [256];

    // ultimo dia es hoy
    if (DiffTimeTm (last_date_tm, today_tm) == 0)
    {
        today_backup = 1;
        // el backup es solo por hoy?
        if (DiffTimeTm (first_date_tm, last_date_tm) == 0)
        {
            only_this_day = 1;
        }
    }

    one_bkp_finded = 0;
    if (!only_this_day)
    {
        while (DiffTimeTm(first_date_tm, last_date_tm) >= 0)
        {
            sprintf(bkp_day, "%d%d%d", first_date_tm.tm_year + 1900,
                    first_date_tm.tm_mon + 1,
                    first_date_tm.tm_mday);
        
            printf ("backup day: %s\n", bkp_day);

            if (!one_bkp_finded)
            {
                //primera parte del archivo de backups           
                sprintf(bkp_cmd, "%s %s/%s/%s > %s/%s",
                        CDR_UTIL_CAT,
                        CDR_BKP_DIR,
                        bkp_day,
                        CDR_FILE,
                        CDR_WORKING_TMP_DIR,
                        CDR_WORKING_TMP_FILE);
            }
            else
            {
                //partes sub-siguientes
                sprintf(bkp_cmd, "%s %s/%s/%s >> %s/%s",
                        CDR_UTIL_CAT,
                        CDR_BKP_DIR,
                        bkp_day,
                        CDR_FILE,
                        CDR_WORKING_TMP_DIR,
                        CDR_WORKING_TMP_FILE);            
            }

            if (system(bkp_cmd) != 0)
            {
                printf("error on cat\n");
                //TODO: cuando tenga el log avisar los dias que no encontre!!!
            }
            else
            {
                //necesito un success para crear el archivo por primera vez
                printf("\nsucess on cat\n");
                one_bkp_finded = 1;
            }

            //Normalizo first_date
            first_date_tm.tm_mday += 1;        
            mktime (&first_date_tm);
        }    //fin while days

        if (today_backup)    //debo incluir un backup de hoy
        {
            sprintf(bkp_day, "%d%d%d", today_tm.tm_year + 1900,
                    today_tm.tm_mon + 1,
                    today_tm.tm_mday);
        
            printf ("today backup: %s\n", bkp_day);

            if (one_bkp_finded)    //esto es lo normal,algo tuve que haber encontrado
            {
                sprintf(bkp_cmd, "%s %s/%s >> %s/%s",
                        CDR_UTIL_CAT,
                        CDR_CURRENT_DIR,
                        CDR_FILE,
                        CDR_WORKING_TMP_DIR,
                        CDR_WORKING_TMP_FILE);

                if (system(bkp_cmd) != 0)
                {
                    printf("error on cat\n");
                    //TODO: cuando tenga el log avisar los dias que no encontre!!!
                }                
            }
            else    //raro, no habia nada?
            {
                sprintf(bkp_cmd, "%s %s/%s > %s/%s",
                        CDR_UTIL_CAT,
                        CDR_CURRENT_DIR,
                        CDR_FILE,
                        CDR_WORKING_TMP_DIR,
                        CDR_WORKING_TMP_FILE);

                if (system(bkp_cmd) != 0)
                {
                    printf("error on cat\n");
                    //TODO: cuando tenga el log avisar los dias que no encontre!!!
                }
                else
                    one_bkp_finded = 1;
            }
        }
    }
    else    //backup solo de hoy
    {
        sprintf(bkp_day, "%d%d%d", today_tm.tm_year + 1900,
                today_tm.tm_mon + 1,
                today_tm.tm_mday);
        
        printf ("today backup: %s\n", bkp_day);
        
        sprintf(bkp_cmd, "%s %s/%s > %s/%s",
                CDR_UTIL_CAT,
                CDR_CURRENT_DIR,
                CDR_FILE,
                CDR_WORKING_TMP_DIR,
                CDR_WORKING_TMP_FILE);

        if (system(bkp_cmd) != 0)
        {
            printf("error on cat\n");
            //TODO: cuando tenga el log avisar los dias que no encontre!!!
        }
        else
            one_bkp_finded = 1;
    }

    if (one_bkp_finded)
        return 0;
    else
        return -1;
}

//devuelve:
//1 si es menor date1 a date2
//0 si son iguales
//-1 si date1 mayor a date2
int DiffTime (int y1, int m1, int d1, int y2, int m2, int d2)
{
    if (y1 < y2)
        return 1;
    else if (y1 > y2)
        return -1;
    else   //mismo anio
    {
     
        if (m1 < m2)
            return 1;
        else if (m1 > m2)
            return -1;
        else   //mismo mes
        {
            if (d1 < d2)
                return 1;
            else if (d1 > d2)
                return -1;
            else    //mismo dia
                return 0;
        }
    }      
}

//--- end of file ---//
