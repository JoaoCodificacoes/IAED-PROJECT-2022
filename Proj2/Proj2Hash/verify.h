#ifndef VERIFY_H
#define VERIFY_H

/*----------------------------------IAED PROJECT------------------------------*/
/*                          João David Natário Ferreira                       */
/*                                    103680                                  */
/*      Objective: Manage a system of Airports,Flights and Reservations       */
/*                              File: "verify.h"                              */
/*----------------------------------------------------------------------------*/

/*Libraries*/
#include "hashtable.h"
#include "main.h"

int existsFlight(char code[], Date date);

int verifyAirport(char id[]);

int verifyFlight(Flight flight);

int verifyFlightAux(Flight flight);

int verifyflightCode(char code[]);

int existsAirport(char id[]);

int verifyResCode(char* str);

int verifyRes(char* rCode, int p, char* flightCode, Date date,
              ResHash** hashRes);

int verifyDateFlight(char* flightCode, Date date);

int verifyResNum(int p, int i);

int isUpperStr(char str[]);

#endif