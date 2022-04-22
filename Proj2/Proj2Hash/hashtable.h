#ifndef HASHTABLE_H
#define HASHTABLE_H

/*----------------------------------IAED PROJECT------------------------------*/
/*                          João David Natário Ferreira                       */
/*                                    103680                                  */
/*      Objective: Manage a system of Airports,Flights and Reservations       */
/*                              File: "hashtable.h"                           */
/*----------------------------------------------------------------------------*/

#include "main.h"

#define TABLESIZE 26153

ResHash *hashSearch(char *rCode, ResHash **hashRes);

void hashInsert(ResHash *res, ResHash **hashRes);

void resDelete(char *rCode, int mode, ResHash **hashRes);

unsigned int hashFunc(char *key);

#endif