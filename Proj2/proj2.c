/*-------------------IAED PROJECT------------------*/
/*             João David Natário Ferreira         */
/*                     103680                      */
/* ------------------------------------------------*/


/*Libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "proj2.h"

typedef struct{
    char* rcode;
    int passengers;
    Res* next;
}Res;


void command_r(){
    char buffer[MaxRCode], *rCode, FlightCode[MaxFLightCode];
    Date date;
    int passengers;

    scanf("%s %d-%d-%d", FlightCode, &date.Day, &date.Month, &date.Year);
    if (scanf("%s %d", buffer,&passengers) == 1){
        rCode = (char*)malloc(sizeof(char)*(strlen(buffer)+1));
        strcpy(rCode,buffer);
        if (!verify_reservation(rCode,passengers,FlightCode,date)) return;

    }

    

}

int verify_reservation(char* rCode,int p,char FlightCode[MaxFLightCode], Date date){
    int d,i;

    if (!verify_reservationCode(rCode)){
        printf(ERROR_INVALID_RESERVATION_CODE);
        return False;
    }
     

    if ( Flight_exists(FlightCode,date) == NonExistant){
        printf(ERROR_FLIGHT_DOES_NOT_EXIST,FlightCode);
        return False;
    }


    if ( (d =Date_Distance(Today,date)) <0 || d > DaysInYear){
        printf(ERROR_INVALID_DATE);
        return False;
    }

    if ( !isdigit(p) || p <= 0){
        printf(ERROR_INVALID_PASSENGER_NUMBER);
        return False;
    }

    return True;
}

int verify_reservationCode(char* str){
    size_t i,len = strlen(str);

    if (len < 10){
        return False;
    }

    while (i++ < len){
        if ( !isdigit(str[i])  ){
            if ( !isalpha(str[i]) ){
                return False;
            }
            if ( !isupper(str[i]) ){
                return False;
            }
        }
    }
    return True;
}




/*Verifies if the entire string is in uppercase */
int is_upper_str(char str[]){
    size_t i;
        for (i=0; i < strlen(str); i++ ){
            if ( !(isupper(str[i])) ){
               return False;
            }
        }
    return True;
}
