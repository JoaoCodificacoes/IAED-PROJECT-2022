
/*----------------------------------IAED PROJECT------------------------------*/
/*                          João David Natário Ferreira                       */
/*                                    103680                                  */
/*      Objective: Manage a system of Airports,Flights and Reservations       */
/*                              File: "hashtable.c"                           */
/*----------------------------------------------------------------------------*/

#include "hashtable.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aux.h"

/*Searchs in the hash table for a reservation with its code */
ResHash *hashSearch(char *rCode, ResHash **hashRes) {
    unsigned int i = hashFunc(rCode);
    ResHash *temp = hashRes[i];

    while (temp && strcmp(temp->resInList->rCode, rCode) != 0) {
        temp = temp->next;
    }
    return temp;
}

/*Inserts a new reservation in the hash table */
void hashInsert(ResHash *res, ResHash **hashRes) {
    int i = hashFunc(res->resInList->rCode);
    res->next = hashRes[i];
    hashRes[i] = res;
}

/*Gets a reservation code and codifies it into a number, that will be its index
on the hash table */
unsigned int hashFunc(char *key) {
    unsigned int h, a = 31415, b = 27183;
    for (h = 0; *key != '\0'; key++, a = a * b % (TABLESIZE - 1))
        h = (a * h + *key) % TABLESIZE;
    return h;
}

/*Delete a reservation from the hash table and prompt its deletion from the
sorted linked list */
void resDelete(char *rCode, int mode, ResHash **hashRes) {
    int i = hashFunc(rCode);
    ResHash *temp = hashRes[i];
    ResHash *prev = NULL;

    /*While it cant be found */
    while (temp && strcmp(temp->resInList->rCode, rCode) != 0) {
        prev = temp;
        temp = temp->next;
    }
    /*If the temp reaches the final of the list in the index */
    if (!temp) {
        printf(ERROR_NOT_FOUND);
        return;
    }

    /* If the reservation is the top of the list on that index */
    if (!prev) {
        hashRes[i] = temp->next;
    }

    else {
        prev->next = temp->next;
    }

    /*If the reservation comes from the removeAllFlights function its index has
    been changed and the pointer to the flight in the hash  table needs to be
    updated to its new index*/
    if (mode != NONEXISTENT) {
        temp->flight = &flights[mode];
    }
    removeResInList(temp->resInList, temp->flight);
    free(temp);
}