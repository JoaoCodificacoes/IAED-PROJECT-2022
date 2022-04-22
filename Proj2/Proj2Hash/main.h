#ifndef MAIN_H
#define MAIN_H

/*----------------------------------IAED PROJECT------------------------------*/
/*                          João David Natário Ferreira                       */
/*                                    103680                                  */
/*      Objective: Manage a system of Airports,Flights and Reservations       */
/*                              File: "main.h"                                */
/*----------------------------------------------------------------------------*/

/*Constant assignment*/
#define MAXID 4
#define MAXCOUNTRY 31
#define MAXCITY 51
#define MAXAIRPORTS 40
#define MAXFLIGHTS 30000
#define MAXFLIGHTCODE 7
#define MINFLIGHTCODE 3
#define MAXFLIGHTCODENUM 9
#define MINFLIGHTCODENUM 0
#define MAXDURATION 12
#define MINCAPACITY 10
#define MINRCODE 10
#define MAXRCODE 65536
#define TRUE 1
#define FALSE 0
#define NONEXISTENT -1
#define AIRPORTPRINT "airport %s\n"

/*     Dates and Time      */
#define DAYSINYEAR 365
#define MINUTESINHOUR 60
#define MONTH31 31
#define MONTH30 30
#define MONTH28 28
#define MOTHS30DAYSNUM 4
#define FIRSTDAY 1
#define FIRSTMONTH 1
#define FIRSTYEAR 2022
#define MINUTESINDAY 1440
#define MONTHSINYEAR 12
#define FEB 2
#define APR 4
#define JUN 6
#define SEP 9
#define NOV 11
#define DEC 12

/*                     ERRORS                                */
#define ERROR_INVALID_AIRPORT "invalid airport ID\n"
#define ERROR_TOO_MANY_AIRPORTS "too many airports\n"
#define ERROR_DUPLICATE_AIRPORT "duplicate airport\n"
#define ERROR_NO_SUCH_AIRPORT_ID "%s: no such airport ID\n"
#define ERROR_INVALID_FLIGHT_CODE "invalid flight code\n"
#define ERROR_FLIGHT_ALREADY_EXISTS "flight already exists\n"
#define ERROR_TOO_MANY_FLIGHTS "too many flights\n"
#define ERROR_INVALID_DATE "invalid date\n"
#define ERROR_INVALID_DURATION "invalid duration\n"
#define ERROR_INVALID_CAPACITY "invalid capacity\n"
#define ERROR_INVALID_PASSENGER_NUMBER "invalid passenger number\n"
#define ERROR_INVALID_RESERVATION_CODE "invalid reservation code\n"
#define ERROR_FLIGHT_DOES_NOT_EXIST "%s: flight does not exist\n"
#define ERROR_TOO_MANY_RESERVATIONS "too many reservations\n"
#define ERROR_RESERVATION_ALREADY_USED "%s: flight reservation already used\n"
#define ERROR_NOT_FOUND "not found\n"
#define ERROR_NO_MEMORY "No memory\n"

/*Data Structures Definition*/
typedef struct {
    char id[MAXID];
    char city[MAXCITY];
    char country[MAXCOUNTRY];
    int airportFlights;

} Airport;

typedef struct {
    int hour;
    int minute;

} Time;

typedef struct {
    int day;
    int month;
    int year;

} Date;

typedef struct {
    Time time;
    Date date;

} FullDate;

typedef struct {
    char flightCode[MAXFLIGHTCODE];
    char departId[MAXID];
    Date date;
    Time time;

} DestFlight;

/* Doubly linked list structure */
typedef struct resList {
    char *rCode;
    int passengers;
    struct resList *next;
    struct resList *prev;
} ResList;

typedef struct {
    char flightCode[MAXFLIGHTCODE];
    char departId[MAXID];
    char destid[MAXID];
    Date date;
    Time time;
    Time duration;
    int capacity;
    ResList *reservation;
    int resNum;

} Flight;

/*Hash table structure */
typedef struct resHash {
    ResList *resInList;
    Flight *flight;
    struct resHash *next;
} ResHash;

/*------------------------------Command Functions ----------------------------*/

void addAirport(void);
void listAirports(void);
void addListFlights(void);
void listFlightsDepart(void);
void changeDate(void);
void listFlightsArrival(void);
void addListReservations(ResHash **hashRes);
void delReservationsFlights(ResHash **hashRes);

/* List with all airports */
Airport *airports;

/* List with all flights */
Flight *flights;

/* Number of airports */
int numAirports;

/* Number of Flights */
int numFlights;

/* Current Date */
Date today;

#endif