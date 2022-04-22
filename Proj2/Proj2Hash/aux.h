#ifndef AUX_H
#define AUX_H

/*----------------------------------IAED PROJECT------------------------------*/
/*                          João David Natário Ferreira                       */
/*                                    103680                                  */
/*      Objective: Manage a system of Airports,Flights and Reservations       */
/*                              File: "aux.h"                                 */
/*----------------------------------------------------------------------------*/

#include "hashtable.h"
#include "main.h"

/*---------------------------Printing Functions-------------------------------*/

void printDepartFlight(Flight temp[], int i);

void printAirportInfo(char id[]);

void printReservations(int i);

/*---------------------------Sorting Functions--------------------------------*/

void sortFlights(Flight fArray[], int flightNum);

void sortAirports(void);

void sortDestFlights(DestFlight fArray[], int flightNum);

/*---------------------------Memory Functions---------------------------------*/

void releaseAllMem(ResHash** hashRes);

void checkMem(void* ptr, ResHash** hashRes);

void removeAllFlights(ResHash** hashRes);

/*-------------------------Linked List Functions------------------------------*/

void removeResInList(ResList* res, Flight* flight);

ResList* insertRes(ResList* res, Flight* flight);

/*------------------------Date and Time Functions-----------------------------*/

int is30Days(int month);

FullDate sumTimeDate(Date date1, Time time1, Time time2);

int monthsToDays(int month);

int sumMinutes(Time time1, Time time2);

int dateDistance(Date date1, Date date2);

int timeDistance(Time time1, Time time2);

Date changesMonthYear(Date date1);

/*------------------------Remove Functions------------------------------------*/
void removeRes(ResList* res, Flight* flight);

void removeReservations(int i, ResHash** hashRes);

void removeFlights(char* Code, ResHash** hashRes);

/*-------------------Other Flight and Airport Functions-----------------------*/

void makeAirport(char id[], char country[], char city[]);

void makeFlight(Flight flight);

DestFlight copyDestFlight(int i);

int searchAirport(char id[]);

Flight scanFlightInfo(Flight flight);

void addReservation(char* flightCode,Date date,ResHash **hashRes);

#endif
