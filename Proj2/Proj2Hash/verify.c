
/*----------------------------------IAED PROJECT------------------------------*/
/*                          João David Natário Ferreira                       */
/*                                    103680                                  */
/*      Objective: Manage a system of Airports,Flights and Reservations       */
/*                              File: "verify.c"                              */
/*----------------------------------------------------------------------------*/

#include "verify.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aux.h"
#include "hashtable.h"
#include "main.h"

/*Gets a temporary flight,and the current date
and verifies if the temporary flight is valid */
int verifyFlight(Flight flight) {
    if (!(verifyflightCode(flight.flightCode))) {
        printf(ERROR_INVALID_FLIGHT_CODE);
        return FALSE;
    }

    if (existsFlight(flight.flightCode, flight.date) != NONEXISTENT) {
        printf(ERROR_FLIGHT_ALREADY_EXISTS);
        return FALSE;
    }

    if (!existsAirport(flight.departId) || !existsAirport(flight.destid)) {
        return FALSE;
    }

    if (!verifyFlightAux(flight)) {
        return FALSE;
    }

    return TRUE;
}

/* Helps verifyFlight to verify all the conditions needed */
int verifyFlightAux(Flight flight) {
    int days, hourDuration;

    if (numFlights == MAXFLIGHTS) {
        printf(ERROR_TOO_MANY_FLIGHTS);
        return FALSE;
    }

    if ((days = dateDistance(today, flight.date)) < 0 || days > DAYSINYEAR) {
        printf(ERROR_INVALID_DATE);
        return FALSE;
    }

    if ((hourDuration = flight.duration.hour) > MAXDURATION ||
        ((hourDuration == MAXDURATION) && flight.duration.minute > 0)) {
        printf(ERROR_INVALID_DURATION);
        return FALSE;
    }

    if (flight.capacity < MINCAPACITY) {
        printf(ERROR_INVALID_CAPACITY);
        return FALSE;
    }

    return TRUE;
}

/* Gets a flightCode and verifies if the code is valid */
int verifyflightCode(char code[]) {
    int i, len;
    len = strlen(code);

    if (len < MINFLIGHTCODE) {
        return FALSE;
    }

    if ((!isalpha(code[0])) || (!isalpha(code[1])) || (!isupper(code[0])) ||
        (!isupper(code[1]))) {
        return FALSE;
    }

    if ((code[2] - '0') < 1) {
        return FALSE;
    }

    for (i = 2; i < len; i++) {
        if (((code[i] - '0') < MINFLIGHTCODENUM) ||
            (code[i] - '0') > MAXFLIGHTCODENUM) {
            return FALSE;
        }
    }
    return TRUE;
}

/* Verifies if the airport is valid */
int verifyAirport(char id[]) {
    if (!(isUpperStr(id))) {
        printf(ERROR_INVALID_AIRPORT);
        return FALSE;
    }

    if (numAirports == MAXAIRPORTS) {
        printf(ERROR_TOO_MANY_AIRPORTS);
        return FALSE;
    }

    if (searchAirport(id) != NONEXISTENT) {
        printf(ERROR_DUPLICATE_AIRPORT);
        return FALSE;
    }
    return TRUE;
}

/* Gets and airport array and a airport id  and verifies if the airport exists*/
int existsAirport(char id[]) {
    if (searchAirport(id) == NONEXISTENT) {
        printf(ERROR_NO_SUCH_AIRPORT_ID, id);
        return FALSE;
    }
    return TRUE;
}

/*Gets a flight code and returns its index on the flight array if it exists
 or -1 if it doesnt */
int existsFlight(char* code, Date date) {
    int i;

    for (i = 0; i < numFlights; i++) {
        if (strcmp(code, flights[i].flightCode) == 0 &&
            dateDistance(date, flights[i].date) == 0) {
            return i;
        }
    }
    return NONEXISTENT;
}

/*Gets a date and a flight code and returns the flight index if it exists and
the date is valid and -1 if it doesnt exists or the date is invalid */
int verifyDateFlight(char* flightCode, Date date) {
    int i, d;
    if ((i = existsFlight(flightCode, date)) == NONEXISTENT) {
        printf(ERROR_FLIGHT_DOES_NOT_EXIST, flightCode);
        return NONEXISTENT;
    }

    if ((d = dateDistance(today, date)) < 0 || d > DAYSINYEAR) {
        printf(ERROR_INVALID_DATE);
        return NONEXISTENT;
    }
    return i;
}

/*Checks if flight reservation is valid*/
int verifyRes(char* rCode, int p, char* flightCode, Date date,
              ResHash** hashRes) {
    int i, d;

    if (!verifyResCode(rCode)) {
        printf(ERROR_INVALID_RESERVATION_CODE);
        return NONEXISTENT;
    }

    if ((i = existsFlight(flightCode, date)) == NONEXISTENT) {
        printf(ERROR_FLIGHT_DOES_NOT_EXIST, flightCode);
        return NONEXISTENT;
    }

    if (hashSearch(rCode, hashRes)) {
        printf(ERROR_RESERVATION_ALREADY_USED, rCode);
        return NONEXISTENT;
    }
    if (!verifyResNum(p, i)) {
        printf(ERROR_TOO_MANY_RESERVATIONS);
        return NONEXISTENT;
    }
    if ((d = dateDistance(today, date)) < 0 || d > DAYSINYEAR) {
        printf(ERROR_INVALID_DATE);
        return NONEXISTENT;
    }

    if (p <= 0) {
        printf(ERROR_INVALID_PASSENGER_NUMBER);
        return NONEXISTENT;
    }

    return i;
}

/*Checks if the number of existent passangers plus the passangers in the new
reservation doesnt exceed the flight capaciy threshold set */
int verifyResNum(int p, int i) {
    if ((flights[i].resNum + p) > flights[i].capacity) {
        return FALSE;
    }
    return TRUE;
}

/*Checks if the reservation code is valid*/
int verifyResCode(char* str) {
    int i = 0;
    while (str[i]) {
        if (!isdigit(str[i])) {
            if (!isalpha(str[i])) {
                return FALSE;
            }
            if (!isupper(str[i])) {
                return FALSE;
            }
        }
        i += 1;
    }
    if (i < 10) {
        return FALSE;
    }
    return TRUE;
}

/*Verifies if the entire string is in uppercase */
int isUpperStr(char str[]) {
    size_t i;
    for (i = 0; i < strlen(str); i++) {
        if (!(isupper(str[i]))) {
            return FALSE;
        }
    }
    return TRUE;
}