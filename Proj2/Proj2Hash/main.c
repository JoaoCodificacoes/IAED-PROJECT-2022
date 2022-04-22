/*----------------------------------IAED PROJECT------------------------------*/
/*                          João David Natário Ferreira                       */
/*                                    103680                                  */
/*      Objective: Manage a system of Airports,Flights and Reservations       */
/*                              File: "main.c"                                */
/*----------------------------------------------------------------------------*/

#include "main.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aux.h"
#include "hashtable.h"
#include "verify.h"

Airport *airports;
Date today = {FIRSTDAY, FIRSTMONTH, FIRSTYEAR};
int numAirports = 0;
int numFlights = 0;

/*Code*/
int main() {
    /*Hash table with reservations*/
    ResHash **hashRes;
    char command;

    flights = (Flight *)calloc(MAXFLIGHTS, sizeof(Flight));
    airports = (Airport *)calloc(MAXAIRPORTS, sizeof(Airport));
    hashRes = (ResHash **)calloc(TABLESIZE, sizeof(ResHash *));

    while ((command = getchar()) != 'q') {
        switch (command) {
                /* command = "a" args: [AiportID] [Country] [City] */
            case 'a':
                addAirport();
                break;
            /*command = "p" args: Optional, [AiportID]...[AirportID] */
            case 'l':
                listAirports();
                break;
            /*command = "v" args: Optional,[flightCode] [departId] [ArrivalID]
            [Departure Date] [Departure Time] [Duration] [Capacity] */
            case 'v':
                addListFlights();
                break;
            /*command = "p" args: Optional, [AiportID]*/
            case 'p':
                listFlightsDepart();
                break;
            /*command = "c" args: Optional, [AiportID]*/
            case 'c':
                listFlightsArrival();
                break;
            /*command = "t" args: [date] */
            case 't':
                changeDate();
                break;
            /*command = "r" args: [flightCode] [date] Optional,
            [[reservationCode] [PassagengerNumber]]*/
            case 'r':
                addListReservations(hashRes);
                break;
            /*command = "e" args:  [flightCode]/[reservationCode]*/
            case 'e':
                delReservationsFlights(hashRes);
                break;
        }
    }
    releaseAllMem(hashRes);
    return 0;
}

/* Makes an airport with the arguments read */
void addAirport() {
    char id[MAXID], country[MAXCOUNTRY], city[MAXCITY], cityAux[MAXCITY];

    scanf("%3s %30s %s", id, country, city);
    fgets(cityAux, MAXCITY, stdin);
    strcat(city, cityAux);
    city[strcspn(city, "\n")] = 0;
    makeAirport(id, country, city);
}

/*Scans to see if the command "l" has arguments, and either lists all airports
by alphabetical order if it doesnt have arguments
or lists the asked ones by the order asked */
void listAirports() {
    char id[MAXID], c;
    int i;
    if ((c = getchar()) == '\n') {
        sortAirports();
        for (i = 0; i < numAirports; i++) {
            printf("%s %s %s %d\n", airports[i].id, airports[i].city,
                   airports[i].country, airports[i].airportFlights);
        }
    }

    else {
        while (c != '\n') {
            scanf("%s", id);
            c = getchar();

            printAirportInfo(id);
        }
    }
}

/*Scans to see if the command "v" has arguments, if there are arguments
makes a flight if not prints existing flights by creation order */
void addListFlights() {
    int i;
    Flight tempflight;
    /*Prints by creation order */
    if (getchar() == '\n') {
        for (i = 0; i < numFlights; i++) {
            printf("%s %s %s ", flights[i].flightCode, flights[i].departId,
                   flights[i].destid);

            printf("%02d-%02d-%d ", flights[i].date.day, flights[i].date.month,
                   flights[i].date.year);

            printf("%02d:%02d\n", flights[i].time.hour, flights[i].time.minute);
        }
    }

    else {
        /*Makes a new Flight*/
        tempflight = scanFlightInfo(tempflight);
        makeFlight(tempflight);
    }
    return;
}

/*Scans an airport id and prints the flights departing from that airport*/
void listFlightsDepart() {
    int airportIndex, airportFlightNum, i, countedflights = 0;
    char id[MAXID];
    Flight temp[MAXFLIGHTS];

    scanf("%s", id);

    airportIndex = searchAirport(id);

    if (airportIndex == NONEXISTENT) {
        printf(ERROR_NO_SUCH_AIRPORT_ID, id);
        return;
    }

    airportFlightNum = airports[airportIndex].airportFlights;

    for (i = 0; i < numFlights; i++) {
        /* If there are no flights left break */
        if (countedflights == airportFlightNum) {
            break;
        }

        if (strcmp(flights[i].departId, id) == 0) {
            temp[countedflights] = flights[i];
            countedflights += 1;
        }
    }

    sortFlights(temp, countedflights);

    for (i = 0; i < countedflights; i++) {
        printDepartFlight(temp, i);
    }
}

/* Scans an airport id and prints the flights departing from that airport */
void listFlightsArrival() {
    int i, countedFlights = 0;
    char id[MAXID];
    DestFlight temp[MAXFLIGHTS];

    scanf("%s", id);

    if (!existsAirport(id)) {
        return;
    }

    for (i = 0; i < numFlights; i++) {
        if (strcmp(flights[i].destid, id) == 0) {
            temp[countedFlights] = copyDestFlight(i);
            countedFlights += 1;
        }
    }

    sortDestFlights(temp, countedFlights);
    for (i = 0; i < countedFlights; i++) {
        printf("%s %s ", temp[i].flightCode, temp[i].departId);

        printf("%02d-%02d-%d ", temp[i].date.day, temp[i].date.month,
               temp[i].date.year);

        printf("%02d:%02d\n", temp[i].time.hour, temp[i].time.minute);
    }
}

/* Reads new date and updates existing one*/
void changeDate() {
    int d;
    Date tempdate;

    scanf("%d-%d-%d", &tempdate.day, &tempdate.month, &tempdate.year);

    if ((d = dateDistance(today, tempdate)) < 0 || d > DAYSINYEAR) {
        printf(ERROR_INVALID_DATE);
        return;
    }

    today = tempdate;
    printf("%02d-%02d-%d\n", today.day, today.month, today.year);
    return;
}

/* Adds a reservation to an existing flight or lists its reservationsfd*/
void addListReservations(ResHash **hashRes) {
    char flightCode[MAXFLIGHTCODE];
    Date date;
    int i;
    scanf("%s %d-%d-%d", flightCode, &date.day, &date.month, &date.year);
    /*with arguments*/
    if (getchar() != '\n') {
        addReservation(flightCode,date,hashRes);
    }
    /*without arguments*/
    else {
        if ((i = verifyDateFlight(flightCode, date)) == NONEXISTENT) {
            return;
        }
        printReservations(i);
    }
}

/* Removes a reservation or a flight from the flight system */
void delReservationsFlights(ResHash **hashRes) {
    char buffer[MAXRCODE], *Code;
    size_t len;
    scanf("%s", buffer);
    len = strlen(buffer);
    Code = (char *)calloc((len + 1), sizeof(char));
    checkMem(Code, hashRes);
    strcpy(Code, buffer);

    /*Flight*/
    if (len < MINRCODE) {
        removeFlights(Code, hashRes);
        free(Code);
    }
    /*Reservation*/
    else {
        resDelete(Code, NONEXISTENT, hashRes);
        free(Code);
    }
}

