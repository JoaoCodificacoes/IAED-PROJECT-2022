#include "proj1.h"


#define MaxRCode 65535
#define ERROR_INVALID_RESERVATION_CODE "invalid reservation code"
#define ERROR_FLIGHT_DOES_NOT_EXIST "%s: flight does not exist"
#define ERROR_INVALID_PASSENGER_NUMBER "invalid passenger number"

typedef struct {
    char* rcode;
    int passengers;
    Res* next;
}Res;






int verify_reservationCode(char* str);
int verify_reservation(char* rCode,int p,char FlightCode[MaxFLightCode], Date date);