/*----------------------------------IAED PROJECT------------------------------*/
/*                          João David Natário Ferreira                       */
/*                                    103680                                  */
/*      Objective: Manage a system of Airports,Flights and Reservations       */
/*                              File: "aux.c"                                 */
/*----------------------------------------------------------------------------*/

/*Libraries*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Header Files */
#include "aux.h"
#include "hashtable.h"
#include "main.h"
#include "verify.h"

/*---------------------------Printing Functions-------------------------------*/

/* Gets an airport id and prints its city country and flights */
void printAirportInfo(char id[]) {
    int i;
    if ((i = searchAirport(id)) == NONEXISTENT) {
        printf(ERROR_NO_SUCH_AIRPORT_ID, id);
        return;
    }
    printf("%s %s %s %d\n", id, airports[i].city, airports[i].country,
           airports[i].airportFlights);
}

/* Gets an array, and an index an prints the flight information
 of its corresponding flight */
void printDepartFlight(Flight temp[], int i) {
    printf("%s %s ", temp[i].flightCode, temp[i].destid);

    printf("%02d-%02d-%d ", temp[i].date.day, temp[i].date.month,
           temp[i].date.year);

    printf("%02d:%02d\n", temp[i].time.hour, temp[i].time.minute);
}

/*Gets a flight index and prints all the reservations made for that flight*/
void printReservations(int i) {
    ResList *temp;
    temp = flights[i].reservation;
    while (temp != NULL) {
        printf("%s %d\n", temp->rCode, temp->passengers);
        temp = temp->next;
    }
}

/*---------------------------Sorting Functions--------------------------------*/

/*Sorts the airport system by ID alphabetically */
void sortAirports() {
    int j, i;
    Airport aux;
    for (i = 0; i < numAirports - 1; i++) {
        for (j = i + 1; j < (numAirports); j++) {
            if (strcmp(airports[i].id, airports[j].id) > 0) {
                aux = airports[i];
                airports[i] = airports[j];
                airports[j] = aux;
            }
        }
    }
}

/*Gets a flight array and sorts its from the oldest date
and time to the most recent one */
void sortFlights(Flight fArray[], int FlightsToSort) {
    int i, j, days;
    Flight aux;
    for (i = 0; i < FlightsToSort - 1; i++) {
        for (j = i + 1; j < FlightsToSort; j++) {
            if ((days = dateDistance(fArray[i].date, fArray[j].date)) < 0 ||
                ((days == 0) &&
                 (timeDistance(fArray[i].time, fArray[j].time)) < 0)) {
                aux = fArray[i];
                fArray[i] = fArray[j];
                fArray[j] = aux;
            }
        }
    }
}

/*Gets a destination flights array and sorts its from
 the oldest date and time to the most recent one */
void sortDestFlights(DestFlight fArray[], int FlightsToSort) {
    int i, j, days;
    DestFlight aux;

    for (i = 0; i < FlightsToSort - 1; i++) {
        for (j = i + 1; j < FlightsToSort; j++) {
            if ((days = dateDistance(fArray[i].date, fArray[j].date)) < 0 ||
                (days == 0 &&
                 timeDistance(fArray[i].time, fArray[j].time) < 0)) {
                aux = fArray[i];
                fArray[i] = fArray[j];
                fArray[j] = aux;
            }
        }
    }
}

/*---------------------------Memory Functions---------------------------------*/

/*Checks if memory can be allocated with size of ptr */
void checkMem(void *ptr, ResHash **hashRes) {
    if (ptr) return;
    releaseAllMem(hashRes);
    printf(ERROR_NO_MEMORY);
    exit(0);
}
/*Frees all allocated Memory */
void releaseAllMem(ResHash **hashRes) {
    free(airports);
    removeAllFlights(hashRes);
    free(flights);
    free(hashRes);
    return;
}
/*Removes all flights from the flight sytem */
void removeAllFlights(ResHash **hashRes) {
    int i;
    for (i = 0; i < numFlights; i++) removeReservations(i, hashRes);
}

/*-------------------------Linked List Functions------------------------------*/

/* Removes a reservation from the linked list */
void removeResInList(ResList *res, Flight *flight) {
    /*If only head*/
    if (!res->prev && !res->next) {
        flight->reservation = NULL;
    }

    /*If head*/
    else if (!res->prev) {
        res->next->prev = NULL;
        flight->reservation = res->next;
    }
    /*If end of list */
    else if (!res->next) {
        res->prev->next = NULL;
    }

    /*If middle of List*/
    else {
        res->prev->next = res->next;
        res->next->prev = res->prev;
    }

    removeRes(res, flight);
}

/*Inserts a new reservation in the respective reservation Linked List*/
ResList *insertRes(ResList *res, Flight *flight) {
    ResList *temp;
    temp = flight->reservation;
    /*             Insert When List Is Empty          */
    if (!temp) {
        flight->reservation = res;
        return res;
    }
    /*             Insert at Top         */
    if (strcmp(res->rCode, temp->rCode) < 0) {
        res->next = temp;
        res->prev = NULL;
        temp->prev = res;
        flight->reservation = res;
        return res;
    }
    /*Find if it should be inserted in middle or list end */
    while (temp->next && strcmp(res->rCode, temp->next->rCode) > 0) {
        temp = temp->next;
    }
    res->next = temp->next;

    /*If in middle*/
    if (temp->next) res->next->prev = res;

    temp->next = res;
    res->prev = temp;

    return res;
}

/*------------------------Date and Time Functions-----------------------------*/

/* Gets two dates and calculates how far apart they are in days */
int dateDistance(Date date1, Date date2) {
    int days, days1, days2;

    days1 =
        (date1.year) * DAYSINYEAR + date1.day + monthsToDays(date1.month - 1);
    days2 =
        (date2.year) * DAYSINYEAR + date2.day + monthsToDays(date2.month - 1);
    days = days2 - days1;
    return days;
}

/*Gets two times and calculates how far apart they are in minutes */
int timeDistance(Time time1, Time time2) {
    int time;

    time = time2.hour * MINUTESINHOUR + time2.minute -
           (time1.hour * MINUTESINHOUR + time1.minute);
    return time;
}

/* Gets a date and two times and adds the sum of the times to the date
returns the new date and time */
FullDate sumTimeDate(Date date1, Time time1, Time time2) {
    FullDate fulldate;
    int sum = sumMinutes(time1, time2);

    if (sum >= MINUTESINDAY) {
        date1 = changesMonthYear(date1);
        sum -= MINUTESINDAY;
    }

    time1.hour = sum / 60;
    time1.minute = sum % 60;
    fulldate.date = date1;
    fulldate.time = time1;
    return fulldate;
}

/* Gets two time structures and returns their sum in minutes */
int sumMinutes(Time time1, Time time2) {
    int min1, min2, sum;

    min2 = time2.hour * MINUTESINHOUR + time2.minute;
    min1 = time1.hour * MINUTESINHOUR + time1.minute;
    sum = min1 + min2;
    return sum;
}

/* Verifies if the Month or year will change after
 the addition of the time to the date */
Date changesMonthYear(Date date1) {
    if (date1.month == FEB && date1.day == (MONTH28)) {
        date1.day = 0;
        date1.month += 1;
    }

    else if (is30Days(date1.month) && (date1.day == MONTH30)) {
        date1.day = 0;
        date1.month += 1;
    }

    else if (!(is30Days(date1.month)) && (date1.day == MONTH31)) {
        date1.day = 0;

        if (date1.month == DEC) {
            date1.year += 1;
            date1.month = 0;
        }

        date1.month += 1;
    }

    date1.day += 1;

    return date1;
}

/*Verifies if a month has 30 days*/
int is30Days(int month) {
    int i;
    const int months[MOTHS30DAYSNUM] = {APR, JUN, SEP, NOV};

    for (i = 0; i < MOTHS30DAYSNUM; i++) {
        if (month == months[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

/* Gets a month a calculates how many days have passed
since the start of the year */
int monthsToDays(int month) {
    int days = 0;

    const int monthsdays[MONTHSINYEAR] = {0,   31,  59,  90,  120, 151,
                                          181, 212, 243, 273, 304, 334};

    days = monthsdays[month];

    return days;
}

/*------------------------Remove Functions------------------------------------*/

/*Remove all existent reservations from a flight */
void removeReservations(int i, ResHash **hashRes) {
    ResList *temp;

    while (flights[i].reservation) {
        temp = flights[i].reservation;
        flights[i].reservation = temp->next;
        resDelete(temp->rCode, i, hashRes);
    }
}

/*Remove a single reservation */
void removeRes(ResList *res, Flight *flight) {
    flight->resNum -= res->passengers;
    free(res->rCode);
    free(res);
}

/*Remove all flights in the system that share the flight code given */
void removeFlights(char *Code, ResHash **hashRes) {
    int i, j, found = FALSE;

    for (i = 0; i < numFlights; i++) {
        if (strcmp(flights[i].flightCode, Code) == 0) {
            found = TRUE;
            removeReservations(i, hashRes);
            numFlights -= 1;

            /*Pull array one position back */
            for (j = i; j < numFlights; j++) {
                flights[j] = flights[j + 1];
            }
            /*Decrement index so a flight isnt skipped*/
            i -= 1;
        }
    }

    if (!found) {
        printf(ERROR_NOT_FOUND);
        return;
    }
}

/*-------------------Other Flight and Airport Functions-----------------------*/

/*Scans information and fills the flight structure with it*/
Flight scanFlightInfo(Flight tempflight) {
    scanf("%s %s %s", tempflight.flightCode, tempflight.departId,
          tempflight.destid);

    scanf("%d-%d-%d", &tempflight.date.day, &tempflight.date.month,
          &tempflight.date.year);

    scanf("%d:%d", &tempflight.time.hour, &tempflight.time.minute);

    scanf("%d:%d %d", &tempflight.duration.hour, &tempflight.duration.minute,
          &tempflight.capacity);

    return tempflight;
}

/* Gets a temporary flight and the current date
and inserts the flight into the flight array and adds it to the depart airport*/
void makeFlight(Flight flight) {
    int airportIndex;

    if (!(verifyFlight(flight))) {
        return;
    }
    flight.reservation = NULL;
    flight.resNum = 0;
    airportIndex = searchAirport(flight.departId);
    airports[airportIndex].airportFlights += 1;
    flights[numFlights] = flight;
    numFlights += 1;
    return;
}

/*Gets an airport ID and an airport array
and returns its index on the airport array or -1 if it doesnt exist*/
int searchAirport(char id[]) {
    int i, airportIndex = NONEXISTENT;
    for (i = 0; i < numAirports; i++) {
        if (strcmp(id, airports[i].id) == 0) {
            airportIndex = i;
            break;
        }
    }
    return airportIndex;
}

/* Makes an airport structure and inserts it into an airport array */
void makeAirport(char id[], char country[], char city[]) {
    Airport airport;
    if (!(verifyAirport(id))) {
        return;
    }
    strcpy(airport.id, id);
    strcpy(airport.country, country);
    strcpy(airport.city, city);
    airport.airportFlights = 0;
    airports[numAirports] = airport;
    printf(AIRPORTPRINT, id);
    numAirports += 1;
    return;
}

/*Gets an index and returns a copy of the flight
on that index on the flight array */
DestFlight copyDestFlight(int i) {
    DestFlight destflight;
    FullDate fulldate;

    strcpy(destflight.departId, flights[i].departId);
    strcpy(destflight.flightCode, flights[i].flightCode);

    fulldate =
        sumTimeDate(flights[i].date, flights[i].time, flights[i].duration);

    destflight.date = fulldate.date;
    destflight.time = fulldate.time;
    return destflight;
}

void addReservation(char* flightCode,Date date,ResHash **hashRes){
    int i;
    int passengers;
    char buffer[MAXRCODE], *rCode;
    ResList *resList;
    ResHash *resInHash;
    Flight *temp;
    
    scanf("%s %d", buffer, &passengers);
    rCode = (char *)calloc((strlen(buffer) + 1), sizeof(char));
    checkMem(rCode, hashRes);
    strcpy(rCode, buffer);

    if ((i = verifyRes(rCode, passengers, flightCode, date, hashRes)) ==
        NONEXISTENT) {
        free(rCode);
        return;
    }
    temp = &flights[i];
    /*Allocate memory for reservation to put in list */
    resList = (ResList *)calloc(1, sizeof(ResList));
    checkMem(resList, hashRes);

    /*Allocate memory for reservation to put in hash table */
    resInHash = (ResHash *)calloc(1, sizeof(ResHash));
    checkMem(hashRes, hashRes);

    /*Make reservation to put in list */
    temp->resNum += passengers;
    resList->passengers = passengers;
    resList->rCode = rCode;

    /*Make reservation to put in hash table */
    resInHash->resInList = insertRes(resList, temp);
    resInHash->flight = temp;

    hashInsert(resInHash, hashRes);
}
