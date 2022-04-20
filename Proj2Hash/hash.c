/*-------------------IAED PROJECT------------------*/
/*             João David Natário Ferreira         */
/*                     103680                      */
/* ------------------------------------------------*/


/*Libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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
#define TABLESIZE 26153

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
typedef struct{

    char id[MAXID];
    char city[MAXCITY];
    char country[MAXCOUNTRY];
    int  airportFlights;

}Airport; 


typedef struct{

    int hour;
    int minute;

}Time;

typedef struct{

    int day;
    int month;
    int year;

}Date;

typedef struct{

    Time time;
    Date date;

}FullDate;


typedef struct{

    char flightCode[MAXFLIGHTCODE];
    char departId[MAXID];
    Date date;
    Time time;

}DestFlight;

struct resList{
    char* rCode;
    int passengers;
    struct resList* next;
    struct resList* prev;
};

typedef struct resList ResList;


typedef struct{

    char flightCode[MAXFLIGHTCODE];
    char departId[MAXID];
    char destid[MAXID];
    Date date;
    Time time;
    Time duration;
    int capacity;
    ResList* reservation;
    int resNum;

}Flight;

struct resHash{
    ResList* resInList;
    Flight* flight;
    struct resHash* next;
};

typedef struct resHash ResHash;



/*Prototypes*/

/* Commands */

void addAirport(void);
void listAirports(void);
void addListFlights(void);
void listFlightsDepart(void);
void changeDate(void);
void listFlightsArrival(void);
void addReservation(void);
void delReservationsFlights(void);

/* Make Structures */

void makeAirport(char id[], char country[], char city[]);
void makeFlight(Flight flight);

/*Sorting*/

void sortFlights(Flight fArray[], int flightNum);
void sortAirports(void);
void sortDestFlights(DestFlight fArray[], int flightNum);

/*Auxiliary Functions*/

int isUpperStr(char str[]);
int monthsToDays(int month);
int sumMinutes(Time time1, Time time2);
Date changesMonthYear(Date date1);
DestFlight copyDestFlight(int i);
int searchAirport(char id[]);
int dateDistance(Date date1, Date date2);
int timeDistance(Time time1, Time time2);
void printDepartFlight(Flight temp[],int i);
void printAirportInfo(char id[]);
Flight scanFlightInfo(Flight flight);
int is30Days(int month);
FullDate sumTimeDate(Date date1, Time time1, Time time2);
void releaseAllMem(void);
void checkMem(void *ptr);
void removeRes(ResList* res,Flight* flight);
void removeReservations(int i);
void removeAllFlights(void);

/*Verification Functions */
int existsFlight(char code[], Date date);
int verifyAirport(char id[]);
int verifyFlight(Flight flight);
int verifyFlightAux(Flight flight);
int verifyflightCode(char code[]);
int existsAirport(char id[]);
int verifyResCode(char* str);
int verifyRes(char* rCode,int p,char flightCode[MAXFLIGHTCODE], Date date);
ResList *insertRes(ResList* res, Flight* flight);
int verifyDateFlight(char* flightCode, Date date);
void printReservations(int i);
int verifyResNum(int p , int i);
void removeResInList(ResList* res, Flight* flight);
unsigned int hashFunc(char* key);
void resDelete(char *rCode,int mode);
ResHash *hashSearch(char* rCode);
void hashInsert(ResHash *res);
void removeFlights(char* Code);



ResHash **hashRes;
/* List with all airports */
Airport *airports;

/* List with all flights */
Flight *flights;

/* Number of airports */
int numAirports = 0;

/* Number of Flights */
int numFlights = 0;

/* Current Date */
Date today = {FIRSTDAY, FIRSTMONTH, FIRSTYEAR};



/*Code*/
int main(){
    char command;

    flights = (Flight*)calloc(MAXFLIGHTS,sizeof(Flight));
    airports = (Airport*)calloc(MAXAIRPORTS,sizeof(Airport));
    hashRes = (ResHash**)calloc(TABLESIZE,sizeof(ResHash*));

    while((command = getchar()) != 'q'){
        switch(command){
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
            case 'r':
                addReservation();
                break;      
            case 'e':
                delReservationsFlights();
                break;
        }

    }
    releaseAllMem();
    return 0;
}



/*Gets a flight array and sorts its from the oldest date
and time to the most recent one */
void sortFlights(Flight fArray[],int FlightsToSort){
    int i,j,days;
    Flight aux;
    for (i=0; i < FlightsToSort-1; i++){

        for (j=i+1; j < FlightsToSort; j++){

            if ( (days = dateDistance(fArray[i].date,fArray[j].date))<0
            || ( (days == 0)
            && (timeDistance(fArray[i].time,fArray[j].time)) < 0) ){
                aux = fArray[i];
                fArray[i] = fArray[j];
                fArray[j] = aux;
            }
        }
    }
}



/*Gets a destination flights array and sorts its from
 the oldest date and time to the most recent one */
void sortDestFlights(DestFlight fArray[], int FlightsToSort){
    int i,j,days;
    DestFlight aux;

    for (i=0; i< FlightsToSort-1; i++){

        for (j=i+1; j < FlightsToSort; j++){

            if ( (days = dateDistance(fArray[i].date,fArray[j].date)) < 0
            || (days == 0 && timeDistance(fArray[i].time,fArray[j].time) < 0)){

                aux = fArray[i];
                fArray[i] = fArray[j];
                fArray[j] = aux;
            }
        }
    }
}

/*---------------------------------VERFUNCS--------------------------------*/




/* Verifies if the airport is valid */ 
int verifyAirport(char id[]){
    if ( !(isUpperStr(id)) ){
        printf(ERROR_INVALID_AIRPORT);
        return FALSE;
    }

    if (numAirports == MAXAIRPORTS){
        printf(ERROR_TOO_MANY_AIRPORTS);
        return FALSE;
    }

    if ( searchAirport(id) != NONEXISTENT) {
        printf(ERROR_DUPLICATE_AIRPORT);
        return FALSE;
    }
    return TRUE;
}


/*Verifies if the entire string is in uppercase */
int isUpperStr(char str[]){
    size_t i;
        for (i=0; i < strlen(str); i++ ){
            if ( !(isupper(str[i])) ){
               return FALSE;
            }
        }
    return TRUE;
}


/* Gets and airport array and a airport id  and verifies if the airport exists*/
int existsAirport(char id[]){
    if (searchAirport(id) == NONEXISTENT){
        printf(ERROR_NO_SUCH_AIRPORT_ID,id);
        return FALSE;
    }
    return TRUE;
}



/*Gets a temporary flight,and the current date
and verifies if the temporary flight is valid */
int verifyFlight(Flight flight){
    if (!(verifyflightCode(flight.flightCode))){
        printf(ERROR_INVALID_FLIGHT_CODE);
        return FALSE;
    }

    if( existsFlight(flight.flightCode,flight.date) != NONEXISTENT){
        printf(ERROR_FLIGHT_ALREADY_EXISTS);
        return FALSE;
    }

    if (!existsAirport(flight.departId) || !existsAirport(flight.destid) ){
        return FALSE;
    }

    if ( !verifyFlightAux(flight)){
        return FALSE;
    }

    return TRUE;
}


/* Helps verifyFlight to verify all the conditions needed */
int verifyFlightAux(Flight flight){
    int days,hourDuration;

    if (numFlights == MAXFLIGHTS){
        printf(ERROR_TOO_MANY_FLIGHTS);
        return FALSE;
    }

    if ( (days = dateDistance(today,flight.date)) < 0 || days > DAYSINYEAR){
        printf(ERROR_INVALID_DATE);
        return FALSE;
    }

    if ((hourDuration=flight.duration.hour) > MAXDURATION 
    || ((hourDuration == MAXDURATION) && flight.duration.minute > 0  )){
        printf(ERROR_INVALID_DURATION);
        return FALSE;
    }

    if ( flight.capacity < MINCAPACITY){
        printf(ERROR_INVALID_CAPACITY);
        return FALSE;
    }

    return TRUE;
}


/* Gets a flightCode and verifies if the code is valid */
int verifyflightCode(char code[]){
    int i,len;
    len = strlen(code);

    if (len < MINFLIGHTCODE){
        return FALSE;
    }

    if ( (!isalpha(code[0])) || (!isalpha(code[1])) 
    || (!isupper(code[0])) || (!isupper(code[1])) ){
        return FALSE;
    }
    
    if ( (code[2] - '0') < 1){
        return FALSE;   
    }

    for (i = 2; i < len; i++){

        if( ((code[i]- '0') < MINFLIGHTCODENUM)
        ||  (code[i]- '0') > MAXFLIGHTCODENUM){
            return FALSE;
        }
    }
    return TRUE;
}



/*Gets a flight code and returns its index on the flight array if it exists
 or -1 if it doesnt */
int existsFlight(char* code, Date date){
    int i;

    for (i=0; i < numFlights; i++){

        if ( strcmp(code,flights[i].flightCode) == 0 &&
         dateDistance(date,flights[i].date) == 0){
            return i;
        }
    }
    return NONEXISTENT;
}




int verifyRes(char* rCode,int p,char* flightCode, Date date){
    int i,d;

    if (!verifyResCode(rCode)){
        printf(ERROR_INVALID_RESERVATION_CODE);
        return NONEXISTENT;
    }
     
    if ( (i = existsFlight(flightCode,date)) == NONEXISTENT){
        printf(ERROR_FLIGHT_DOES_NOT_EXIST,flightCode);
        return NONEXISTENT;
    }

    if (hashSearch(rCode) ){
        printf(ERROR_RESERVATION_ALREADY_USED,rCode);
        return NONEXISTENT;
    }
    if (!verifyResNum(p,i)){
        printf(ERROR_TOO_MANY_RESERVATIONS);
        return NONEXISTENT;
    }
    if ( (d =dateDistance(today,date)) <0 || d > DAYSINYEAR){
        printf(ERROR_INVALID_DATE);
        return NONEXISTENT;
    }

    if ( p <= 0){
        printf(ERROR_INVALID_PASSENGER_NUMBER);
        return NONEXISTENT;
    }

    return i;
}


int verifyDateFlight(char* flightCode, Date date){
    int i,d;
    if ( (i = existsFlight(flightCode,date)) == NONEXISTENT){
        printf(ERROR_FLIGHT_DOES_NOT_EXIST,flightCode);
        return NONEXISTENT;
    }


    if ( (d =dateDistance(today,date)) <0 || d > DAYSINYEAR){
        printf(ERROR_INVALID_DATE);
        return NONEXISTENT;
    }
    return i;
}




int verifyResCode(char* str){
    int i=0;
    while (str[i]){
        if ( !isdigit(str[i])  ){
            if ( !isalpha(str[i]) ){
                return FALSE;
            }
            if ( !isupper(str[i]) ){
                return FALSE;
            }
        }
        i += 1;
    }
    if (i < 10){
        return FALSE;
    }
    return TRUE;
}


/* Gets two dates and calculates how far apart they are in days */
int dateDistance(Date date1, Date date2){
    int days,days1,days2;

    days1 = (date1.year)*DAYSINYEAR + date1.day + monthsToDays(date1.month-1);
    days2 = (date2.year)*DAYSINYEAR + date2.day + monthsToDays(date2.month-1);
    days = days2-days1;
    return days;
}


/*Gets two times and calculates how far apart they are in minutes */
int timeDistance(Time time1, Time time2 ){
    int time;

    time = time2.hour*MINUTESINHOUR + time2.minute -
    (time1.hour*MINUTESINHOUR + time1.minute);
    return time;
}


/* Gets an array, and an index an prints the flight information
 of its corresponding flight */
void printDepartFlight(Flight temp[],int i){

    printf("%s %s ",temp[i].flightCode, temp[i].destid);

    printf("%02d-%02d-%d ",temp[i].date.day, temp[i].date.month,
    temp[i].date.year);

    printf("%02d:%02d\n",temp[i].time.hour, temp[i].time.minute);

}




/*---------------------------------VERFUNCS------------------------------------------*/



/*-----------------------------------AUXFUNCS-----------------------------*/




/*Scans information and fills the flight structure with it*/
Flight scanFlightInfo(Flight tempflight){
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
void makeFlight(Flight flight){
    int airportIndex;

    if ( !(verifyFlight(flight)) ){
        return;
    }
    flight.reservation = NULL;
    flight.resNum = 0;
    airportIndex = searchAirport(flight.departId);
    airports[airportIndex].airportFlights += 1;
    flights[numFlights] = flight;
    numFlights +=1;
    return ;
}





/*Gets an airport ID and an airport array
and returns its index on the airport array or -1 if it doesnt exist*/

int searchAirport(char id[]){
    int i,airportIndex = -1;
    for (i=0; i < numAirports; i++){
        if (strcmp(id,airports[i].id) == 0 ){
            airportIndex = i;
            break;
        }
    }
    return airportIndex;
}



/*Sorts the airport system by ID alphabetically */
void sortAirports(){
    int j,i;
    Airport aux;
    for (i=0; i < numAirports-1; i++){

        for (j= i+1; j < (numAirports);j++){

            if ( strcmp(airports[i].id, airports[j].id) > 0){
                aux = airports[i];
                airports[i] = airports[j];
                airports[j] = aux;
            }
        }
    }
}


/* Gets an airport id and prints its city country and flights */
void printAirportInfo(char id[]){
    int i;
    if ((i = searchAirport(id)) == NONEXISTENT){

        printf(ERROR_NO_SUCH_AIRPORT_ID,id);
        return;
    }
    printf("%s %s %s %d\n", id, airports[i].city, airports[i].country,
     airports[i].airportFlights);
}


/* Makes an airport structure and inserts it into an airport array */
void makeAirport(char id[], char country[], char city[]){
    Airport airport;
    if ( !(verifyAirport(id)) ){
        return;
    }
    strcpy(airport.id,id);
    strcpy(airport.country,country);
    strcpy(airport.city,city);
    airport.airportFlights = 0;
    airports[numAirports] = airport;
    printf(AIRPORTPRINT,id);
    numAirports +=1;
    return;
}


/*Gets an index and returns a copy of the flight
on that index on the flight array */
DestFlight copyDestFlight(int i){
    DestFlight destflight;
    FullDate fulldate;

    strcpy(destflight.departId,flights[i].departId);
    strcpy(destflight.flightCode,flights[i].flightCode);

    fulldate = 
    sumTimeDate(flights[i].date, flights[i].time,flights[i].duration);

    destflight.date = fulldate.date;
    destflight.time = fulldate.time;
    return destflight;
}


/* Gets a date and two times and adds the sum of the times to the date
returns the new date and time */
FullDate sumTimeDate(Date date1, Time time1, Time time2){
    FullDate fulldate;
    int sum = sumMinutes(time1,time2);
    
    if (sum >= MINUTESINDAY){
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
int sumMinutes(Time time1, Time time2){
    int min1,min2,sum;

    min2 = time2.hour*MINUTESINHOUR + time2.minute;
    min1 = time1.hour*MINUTESINHOUR + time1.minute;
    sum = min1 +min2;
    return sum;
}


/* Verifies if the Month or year will change after
 the addition of the time to the date */
Date changesMonthYear(Date date1){

        if  (date1.month == FEB && date1.day == (MONTH28)){
            date1.day = 0;
            date1.month +=1;
        }

        else if (is30Days(date1.month) && (date1.day == MONTH30)){
            date1.day = 0;
            date1.month +=1;
        }

        else if (!(is30Days(date1.month))  && (date1.day == MONTH31)){
            date1.day = 0;

            if (date1.month == DEC){
                date1.year +=1;
                date1.month = 0;
            }

            date1.month +=1;
        }

        date1.day +=1;

    return date1;
}


/*Verifies if a month has 30 days*/
int is30Days(int month){
    int i;
    const int months[MOTHS30DAYSNUM] = {APR,JUN,SEP,NOV};
    
    for (i=0; i < MOTHS30DAYSNUM; i++){

        if (month == months[i]){
            return TRUE;
        }
    }
    return FALSE;
}


/* Gets a month a calculates how many days have passed
since the start of the year */
int monthsToDays(int month){
    int days = 0;
    
    const int monthsdays[MONTHSINYEAR] = 
    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };


    days = monthsdays[month];
    
    return days;
}

ResList* insertRes(ResList* res,Flight* flight){
    ResList *temp;
    temp = flight->reservation;
    /*             Insert When List Is Empty          */
    if (!temp){
        flight->reservation = res;
        return res;
    }
     /*             Insert at Top         */
    if (strcmp(res->rCode,temp->rCode) < 0){
        res->next = temp;
        res->prev = NULL;
        temp->prev = res;
        flight->reservation = res;
        return res;
    }

    while ( temp->next  && strcmp(res->rCode,temp->next->rCode) > 0){
        temp = temp->next;
    }
    res->next = temp->next;

    if ( temp->next)
        res->next->prev = res;

    temp->next = res;
    res->prev = temp;
        
    return res;
}

void printReservations(int i){
    ResList* temp;
    temp = flights[i].reservation;
    while (temp != NULL){
        printf("%s %d\n",temp->rCode, temp->passengers);
        temp = temp->next;
    }

}


int verifyResNum(int p , int i){
    if ( (flights[i].resNum + p) > flights[i].capacity){
        return FALSE;
    }
    return TRUE;
}






/*-----------------------------------AUXFUNCS-----------------------------*/


/*-------------------------------------------COMMANDSFUNCS-----------------*/




/* Makes an airport with the arguments read */
void addAirport(){
    char id[MAXID],country[MAXCOUNTRY],city[MAXCITY],cityaux[MAXCITY];

    scanf("%3s %30s %s",id,country,city);
    fgets(cityaux,MAXCITY,stdin);
    strcat(city,cityaux);
    city[strcspn(city, "\n")] = 0;
    makeAirport(id,country,city);
}


/*Scans to see if the command "l" has arguments, and either lists all airports
by alphabetical order if it doesnt have arguments
or lists the asked ones by the order asked */
void listAirports(){
    char id[MAXID], c;
    int i;
    if ((c=getchar()) == '\n'){
        sortAirports();
        for ( i = 0; i < numAirports; i++){
            printf("%s %s %s %d\n",airports[i].id, airports[i].city,
            airports[i].country, airports[i].airportFlights);
        }
    }

    else{
        while ( c != '\n'){
            scanf("%s",id);
            c =getchar();

            printAirportInfo(id);
        }
    }
}


/*Scans to see if the command "v" has arguments, if there are arguments
makes a flight if not prints existing flights by creation order */
void addListFlights(){
    int i;
    Flight tempflight;
    /*Prints by creation order */
    if (getchar() == '\n'){
        for (i =0; i<numFlights; i++){

            printf("%s %s %s ", flights[i].flightCode, flights[i].departId
            , flights[i].destid);

            printf("%02d-%02d-%d ",flights[i].date.day,
             flights[i].date.month, flights[i].date.year);

            printf("%02d:%02d\n",flights[i].time.hour,
             flights[i].time.minute);
        }
    }
                
    else{
        /*Makes a new Flight*/
        tempflight = scanFlightInfo(tempflight);
        makeFlight(tempflight);
    }
    return;
}

/*Scans an airport id and prints the flights departing from that airport*/ 
void listFlightsDepart(){
    int airportIndex,airportFlightNum,i,countedflights = 0;
    char id[MAXID];
    Flight temp[MAXFLIGHTS];

    scanf("%s",id);

    airportIndex = searchAirport(id);

    if( airportIndex == NONEXISTENT ){
        printf(ERROR_NO_SUCH_AIRPORT_ID,id);
        return;
    }

    airportFlightNum = airports[airportIndex].airportFlights;

    for (i=0; i < numFlights; i++){

        /* If there are no flights left break */
        if (countedflights == airportFlightNum){
            break;
        }

        if ( strcmp(flights[i].departId,id) == 0 ){
            temp[countedflights] = flights[i];
            countedflights +=1;
        }
    }

    sortFlights(temp,countedflights);

    for(i=0; i < countedflights;i++){
        printDepartFlight(temp,i);
    }
}

/* Scans an airport id and prints the flights departing from that airport */ 
void listFlightsArrival(){
    int i,countedFlights = 0;
    char id[MAXID];
    DestFlight temp[MAXFLIGHTS];

    scanf("%s",id);

    if (!existsAirport(id)){
        return;
    }
    
    for (i=0; i < numFlights; i++){
        if ( strcmp(flights[i].destid,id) == 0 ){
            temp[countedFlights] = copyDestFlight(i);
            countedFlights +=1;
        }
    }

    sortDestFlights(temp,countedFlights);
    for(i=0; i < countedFlights;i++){

        printf("%s %s ",temp[i].flightCode, temp[i].departId);

        printf("%02d-%02d-%d ",temp[i].date.day, temp[i].date.month,
        temp[i].date.year);

        printf("%02d:%02d\n",temp[i].time.hour, temp[i].time.minute);
    }
}



/* Reads new date and updates existing one*/
void changeDate(){
    int d;
    Date tempdate;

    scanf("%d-%d-%d", &tempdate.day, &tempdate.month, &tempdate.year);

    if ( (d =dateDistance(today,tempdate)) <0 || d > DAYSINYEAR){
        printf(ERROR_INVALID_DATE);
        return;
    }
    
    today = tempdate;
    printf("%02d-%02d-%d\n",today.day, today.month, today.year);
    return;
}




void addReservation(){
    char buffer[MAXRCODE], *rCode, flightCode[MAXFLIGHTCODE];
    Date date;
    int passengers;
    ResList* resList;
    ResHash* resHash;
    Flight* temp;
    int i;

    scanf("%s %d-%d-%d", flightCode, &date.day, &date.month, &date.year);
    /*with args*/
    if (getchar() != '\n'){
        scanf("%s %d", buffer,&passengers);
        rCode = (char*)calloc((strlen(buffer)+1),sizeof(char));
        checkMem(rCode);
        strcpy(rCode,buffer);

        if ((i = verifyRes(rCode,passengers,flightCode,date)) == NONEXISTENT){
            free(rCode);
            return;
        }
        temp = &flights[i];

        resList = (ResList*)calloc(1,sizeof(ResList));
        checkMem(resList);

        resHash = (ResHash*)calloc(1,sizeof(ResHash));
        checkMem(resHash);

        temp->resNum += passengers;
        resList->passengers = passengers;
        resList->rCode = rCode;

    
        resHash->resInList = insertRes(resList, temp);
        resHash->flight = temp;

        hashInsert(resHash);
        
    }
    /*no args*/
    else{
        if (( i = verifyDateFlight(flightCode,date)) == NONEXISTENT){
            return;
        }
        printReservations(i);
    }
}






void delReservationsFlights(){
    char buffer[MAXRCODE],*Code;
    size_t len;
    scanf("%s",buffer);
    len = strlen(buffer);
    Code = (char*)calloc((len+1),sizeof(char));
    checkMem(Code);
    strcpy(Code,buffer);

    if (len < MINRCODE){
        removeFlights(Code);
        free(Code);
    }
    else{
        resDelete(Code,NONEXISTENT);
        free(Code);
    }
}

/*-------------------------------------------COMMANDSFUNCS-----------------*/


void checkMem(void *ptr){
    if (ptr) return;
    releaseAllMem();
    printf(ERROR_NO_MEMORY);
    exit(0);
}

void releaseAllMem(){
    free(airports);
    removeAllFlights();
    free(flights);
    free(hashRes);
    return;
}


unsigned int hashFunc(char* key) {
    size_t i = 0, len = strlen(key);
    unsigned int hash = 0;
    while (i != len) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash % TABLESIZE;
}

void hashInsert(ResHash *res){
    int i = hashFunc(res->resInList->rCode);
    res->next = hashRes[i];
    hashRes[i] = res;
}

ResHash *hashSearch(char* rCode){
    unsigned int i = hashFunc(rCode);
    ResHash *temp = hashRes[i];
    while (temp && strcmp(temp->resInList->rCode,rCode) != 0){
        temp = temp->next;
    }
    return temp;
}

void resDelete(char *rCode,int mode){
    int i = hashFunc(rCode);
    ResHash *temp = hashRes[i];
    ResHash *prev = NULL;


    while (temp && strcmp(temp->resInList->rCode,rCode) != 0){
        prev = temp;
        temp = temp->next;
    }

    if (!temp){
        printf(ERROR_NOT_FOUND);
        return;
    }

    if (!prev){
        hashRes[i] = temp->next;
    }

    else{
        prev->next = temp->next;
    }

    if (mode != NONEXISTENT){
        temp->flight = &flights[mode];
    }
    removeResInList(temp->resInList,temp->flight);
    free(temp);
}

void removeResInList(ResList* res, Flight* flight){
    if (!res->prev && !res->next){
        flight->reservation = NULL;
    }

    else if (!res->prev){
        res->next->prev = NULL;
        flight->reservation = res->next;
    }

    else if (!res->next){
        res->prev->next = NULL;
    }

    else{
        res->prev->next = res->next;
        res->next->prev = res->prev;
    }
    
    removeRes(res,flight);
}

void removeFlights(char* Code){
    int i,j,found = FALSE;

    for (i=0; i < numFlights; i++){
        if (strcmp(flights[i].flightCode,Code) == 0){

            found = TRUE;
            removeReservations(i);
            numFlights -= 1;

            for (j=i; j < numFlights; j++){
                flights[j] = flights[j+1];
            }
            i -=1;
        }
    }


    if (!found){
        printf(ERROR_NOT_FOUND);
        return;
    }
}


void removeAllFlights(){
    int i;
    for (i=0; i<numFlights; i++)
        removeReservations(i);   
}

void removeReservations(int i){
    ResList *temp;

    while (flights[i].reservation){
        temp = flights[i].reservation;
        flights[i].reservation = temp->next;
        resDelete(temp->rCode,i);
    }
}


void removeRes(ResList *res,Flight* flight){
    flight->resNum -= res->passengers;
    free(res->rCode);
    free(res);
}