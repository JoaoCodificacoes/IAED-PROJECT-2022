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
#define MaxID 4
#define MaxCountry 31
#define MaxCity 51
#define MaxAirports 40
#define MaxFlights 30000
#define MaxFLightCode 7
#define MinFlightCode 3
#define MaxFlightCodeNum 9
#define MinFlightCodeNum 0
#define MaxDuration 12
#define MinCapacity 10
#define MinRCode 10
#define MaxRCode 65536
#define True 1
#define False 0
#define NonExistant -1
#define Airport_Print "airport %s\n"


/*     Dates and Time      */
#define DaysInYear 365
#define minutesinHour 60
#define Month31 31
#define Month30 30
#define Month28 28
#define n_Months_30Days 4
#define FirstDay 1
#define FirstMonth 1
#define FirstYear 2022
#define minutesinDay 1440
#define MonthsinYear 12
#define JaninYear 31
#define FebinYear 59
#define MarinYear 90
#define AprinYear 120
#define MayinYear 151
#define JuninYear 181
#define JulinYear 212
#define AuginYear 243
#define SepinYear 273
#define OctinYear 304
#define NovinYear 334
#define Feb 2
#define Apr 4
#define Jun 6
#define Sep 9
#define Nov 11
#define Dec 12


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



/*Data Structures Definition*/
typedef struct{

    char id[MaxID];
    char city[MaxCity];
    char country[MaxCountry];
    int  flights;

}Airport; 


typedef struct{

    int hour;
    int minute;

}Time;

typedef struct{

    int Day;
    int Month;
    int Year;

}Date;

typedef struct{

    Time time;
    Date date;

}FullDate;


typedef struct{

    char flightcode[MaxFLightCode];
    char departid[MaxID];
    Date date;
    Time time;

}DestFlight;

struct res{
    char* rcode;
    int passengers;
    struct res* next;
};

typedef struct res Res;


typedef struct{

    char flightcode[MaxFLightCode];
    char departid[MaxID];
    char destid[MaxID];
    Date date;
    Time time;
    Time duration;
    int capacity;
    Res* reservation;
    int reservation_n;

}Flight;


/*Prototypes*/

/* Commands */

void command_a(void);
void command_l(void);
void command_v(void);
void command_p(void);
void command_t(void);
void command_c(void);
void command_r(void);
void command_e(void);

/* Make Structures */

void make_airport(char id[], char country[], char city[]);
void make_flight(Flight flight);

/*Sorting*/

void sort_flights(Flight farray[], int flight_n);
void sort_airports(void);
void sort_destflights(DestFlight farray[], int flight_n);

/*Auxiliary Functions*/

int is_upper_str(char str[]);
int Months_in_days(int Months);
int sum_minutes(Time time1, Time time2);
Date Changes_MonthYear(Date date1);
DestFlight Copy_destflight_struct(int i);
int Search_Airport_by_ID(char id[]);
int Date_Distance(Date date1, Date date2);
int time_distance(Time time1, Time time2);
void Print_DepartFlight_info(Flight temp[],int i);
void print_airport_info(char id[]);
Flight Scan_Flight_Info(Flight flight);
int is30Days(int Month);
FullDate sumtime_Date(Date date1, Time time1, Time time2);

/*Verification Functions */
int Flight_exists(char code[], Date date);
int verify_airport(char id[]);
int verify_flight(Flight flight);
int Verify_Flight_aux(Flight flight);
int verify_flightcode(char code[]);
int Airport_exists(char id[]);
int verify_reservationCode(char* str);
int verify_reservation(char* rCode,int p,char FlightCode[MaxFLightCode], Date date);
void Insert_Reservation(Res* reservation, int i);
int VerifyFlightCodeandDate(char* FlightCode, Date date);
void Print_Reservations(int i);
int verify_reservation_duplicate(char* Code);
int verify_reservation_n(int p , int i);
void remove_res(Res* res,int i);
void FindandFreeRes(char* Code);
void remove_reservations(int i);
void remove_FlightList(void);



/* List with all airports */
Airport *AirportList;

/* List with all flights */
Flight *FlightList;

/* Number of airports */
int airport_n = 0;

/* Number of Flights */
int flight_n = 0;

/* Current Date */
Date Today = {FirstDay, FirstMonth, FirstYear};



/*Code*/
int main(){
    char command;

    FlightList = (Flight*)malloc(sizeof(Flight)*MaxFlights);
    AirportList = (Airport*)malloc(sizeof(Airport)*MaxAirports);

    while((command = getchar()) != 'q'){
        switch(command){
             /* command = "a" args: [AiportID] [Country] [City] */
            case 'a':
                command_a();
                break;
            /*command = "p" args: Optional, [AiportID]...[AirportID] */
            case 'l': 
                command_l();
                break;
            /*command = "v" args: Optional,[FlightCode] [DepartID] [ArrivalID]
            [Departure Date] [Departure Time] [Duration] [Capacity] */
            case 'v':
                command_v();
                break;
            /*command = "p" args: Optional, [AiportID]*/
            case 'p':
                command_p();
                break;
            /*command = "c" args: Optional, [AiportID]*/
            case 'c':
                command_c();
                break;
            /*command = "t" args: [date] */
            case 't':
                command_t();
                break;
            case 'r':
                command_r();
                break;      
            case 'e':
                command_e();
                break;
        }
    }
    remove_FlightList();
    free(AirportList);
    return 0;
}



/*Gets a flight array and sorts its from the oldest date
and time to the most recent one */
void sort_flights(Flight farray[],int f_n){
    int i,j,days;
    Flight aux;
    for (i=0; i < f_n-1; i++){

        for (j=i+1; j < f_n; j++){

            if ( (days = Date_Distance(farray[i].date,farray[j].date))<0
            || ( (days == 0)
            && (time_distance(farray[i].time,farray[j].time)) < 0) ){
                aux = farray[i];
                farray[i] = farray[j];
                farray[j] = aux;
            }
        }
    }
}



/*Gets a destination flights array and sorts its from
 the oldest date and time to the most recent one */
void sort_destflights(DestFlight farray[], int desflight_n){
    int i,j,days;
    DestFlight aux;

    for (i=0; i< desflight_n-1; i++){

        for (j=i+1; j < desflight_n; j++){

            if ( (days = Date_Distance(farray[i].date,farray[j].date)) < 0
            || (days == 0 && time_distance(farray[i].time,farray[j].time) < 0)){

                aux = farray[i];
                farray[i] = farray[j];
                farray[j] = aux;
            }
        }
    }
}

/*---------------------------------VERFUNCS--------------------------------*/




/* Verifies if the airport is valid */ 
int verify_airport(char id[]){
    if ( !(is_upper_str(id)) ){
        printf(ERROR_INVALID_AIRPORT);
        return False;
    }

    if (airport_n == MaxAirports){
        printf(ERROR_TOO_MANY_AIRPORTS);
        return False;
    }

    if (Search_Airport_by_ID(id) != -1){
        printf(ERROR_DUPLICATE_AIRPORT);
        return False;
    }
    return True;
}


/*Verifies if the entire string is in uppercase */
int is_upper_str(char str[]){
    size_t i;
        for (i=0; i < strlen(str); i++ ){
            if ( !(isupper(str[i])) ){
               return False;
            }
        }
    return True;
}


/* Gets and airport array and a airport id  and verifies if the airport exists*/
int Airport_exists(char id[]){
    if (Search_Airport_by_ID(id) == -1){
        printf(ERROR_NO_SUCH_AIRPORT_ID,id);
        return False;
    }
    return True;
}



/*Gets a temporary flight,and the current date
and verifies if the temporary flight is valid */
int verify_flight(Flight flight){
    if (!(verify_flightcode(flight.flightcode))){
        printf(ERROR_INVALID_FLIGHT_CODE);
        return False;
    }

    if( Flight_exists(flight.flightcode,flight.date) != -1){
        printf(ERROR_FLIGHT_ALREADY_EXISTS);
        return False;
    }

    if (!Airport_exists(flight.departid) || !Airport_exists(flight.destid) ){
        return False;
    }

    if ( !Verify_Flight_aux(flight)){
        return False;
    }

    return True;
}


/* Helps verify_flight to verify all the conditions needed */
int Verify_Flight_aux(Flight flight){
    int days,capacity,duration_hour;

    if (flight_n == MaxFlights){
        printf(ERROR_TOO_MANY_FLIGHTS);
        return False;
    }

    if ( (days = Date_Distance(Today,flight.date)) < 0 || days > DaysInYear){
        printf(ERROR_INVALID_DATE);
        return False;
    }

    if ((duration_hour=flight.duration.hour) > MaxDuration 
    || ((duration_hour == MaxDuration) && flight.duration.minute > 0  )){
        printf(ERROR_INVALID_DURATION);
        return False;
    }

    if ( (capacity = flight.capacity) < MinCapacity){
        printf(ERROR_INVALID_CAPACITY);
        return False;
    }

    return True;
}


/* Gets a flightcode and verifies if the code is valid */
int verify_flightcode(char code[]){
    int i,len;
    len = strlen(code);

    if (len < MinFlightCode){
        return False;
    }

    if ( (!isalpha(code[0])) || (!isalpha(code[1])) 
    || (!isupper(code[0])) || (!isupper(code[1])) ){
        return False;
    }
    
    if ( (code[2] - '0') < 1){
        return False;   
    }

    for (i = 2; i < len; i++){

        if( ((code[i]- '0') < MinFlightCodeNum)
        ||  (code[i]- '0') > MaxFlightCodeNum){
            return False;
        }
    }
    return True;
}



/*Gets a flight code and returns its index on the flight array if it exists
 or -1 if it doesnt */
int Flight_exists(char* code, Date date){
    int i;

    for (i=0; i < flight_n; i++){

        if ( strcmp(code,FlightList[i].flightcode) == 0 &&
         Date_Distance(date,FlightList[i].date) == 0){
            return i;
        }
    }
    return NonExistant;
}




int verify_reservation(char* rCode,int p,char* FlightCode, Date date){
    int i,d;

    if (!verify_reservationCode(rCode)){
        printf(ERROR_INVALID_RESERVATION_CODE);
        return NonExistant;
    }
     
    if ( (i = Flight_exists(FlightCode,date)) == NonExistant){
        printf(ERROR_FLIGHT_DOES_NOT_EXIST,FlightCode);
        return NonExistant;
    }

    if (!verify_reservation_duplicate(rCode)){
        printf(ERROR_RESERVATION_ALREADY_USED,rCode);
        return NonExistant;
    }
    if (!verify_reservation_n(p,i)){
        printf(ERROR_TOO_MANY_RESERVATIONS);
        return NonExistant;
    }
    if ( (d =Date_Distance(Today,date)) <0 || d > DaysInYear){
        printf(ERROR_INVALID_DATE);
        return NonExistant;
    }

    if ( p <= 0){
        printf(ERROR_INVALID_PASSENGER_NUMBER);
        return NonExistant;
    }

    return i;
}


int VerifyFlightCodeandDate(char* FlightCode, Date date){
    int i,d;
    if ( (i = Flight_exists(FlightCode,date)) == NonExistant){
        printf(ERROR_FLIGHT_DOES_NOT_EXIST,FlightCode);
        return NonExistant;
    }


    if ( (d =Date_Distance(Today,date)) <0 || d > DaysInYear){
        printf(ERROR_INVALID_DATE);
        return NonExistant;
    }
    return i;
}




int verify_reservationCode(char* str){
    int i=0;
    while (str[i]){
        if ( !isdigit(str[i])  ){
            if ( !isalpha(str[i]) ){
                return False;
            }
            if ( !isupper(str[i]) ){
                return False;
            }
        }
        i += 1;
    }
    if (i < 10){
        return False;
    }
    return True;
}


/* Gets two dates and calculates how far apart they are in days */
int Date_Distance(Date date1, Date date2){
    int days,days1,days2;

    days1 = (date1.Year)*DaysInYear + date1.Day + Months_in_days(date1.Month-1);
    days2 = (date2.Year)*DaysInYear + date2.Day + Months_in_days(date2.Month-1);
    days = days2-days1;
    return days;
}


/*Gets two times and calculates how far apart they are in minutes */
int time_distance(Time time1, Time time2 ){
    int time;

    time = time2.hour*minutesinHour + time2.minute -
    (time1.hour*minutesinHour + time1.minute);
    return time;
}


/* Gets an array, and an index an prints the flight information
 of its corresponding flight */
void Print_DepartFlight_info(Flight temp[],int i){

    printf("%s %s ",temp[i].flightcode, temp[i].destid);

    printf("%02d-%02d-%d ",temp[i].date.Day, temp[i].date.Month,
    temp[i].date.Year);

    printf("%02d:%02d\n",temp[i].time.hour, temp[i].time.minute);

}




/*---------------------------------VERFUNCS------------------------------------------*/



/*-----------------------------------AUXFUNCS-----------------------------*/




/*Scans information and fills the flight structure with it*/
Flight Scan_Flight_Info(Flight tempflight){
    scanf("%s %s %s", tempflight.flightcode, tempflight.departid,
    tempflight.destid);

    scanf("%d-%d-%d", &tempflight.date.Day, &tempflight.date.Month,
    &tempflight.date.Year);

    scanf("%d:%d", &tempflight.time.hour, &tempflight.time.minute);

    scanf("%d:%d %d", &tempflight.duration.hour, &tempflight.duration.minute,
    &tempflight.capacity);

    return tempflight;
}


/* Gets a temporary flight and the current date
and inserts the flight into the flight array and adds it to the depart airport*/
void make_flight(Flight flight){
    int airport_i;

    if ( !(verify_flight(flight)) ){
        return;
    }
    flight.reservation = NULL;
    flight.reservation_n = 0;
    airport_i = Search_Airport_by_ID(flight.departid);
    AirportList[airport_i].flights += 1;
    FlightList[flight_n] = flight;
    flight_n +=1;
    return ;
}





/*Gets an airport ID and an airport array
and returns its index on the airport array or -1 if it doesnt exist*/

int Search_Airport_by_ID(char id[]){
    int i,aarray_i = -1;
    for (i=0; i < airport_n; i++){
        if (strcmp(id,AirportList[i].id) == 0 ){
            aarray_i = i;
            break;
        }
    }
    return aarray_i;
}



/*Sorts the airport system by ID alphabetically */
void sort_airports(){
    int j,i;
    Airport aux;
    for (i=0; i < airport_n-1; i++){

        for (j= i+1; j < (airport_n);j++){

            if ( strcmp(AirportList[i].id, AirportList[j].id) > 0){
                aux = AirportList[i];
                AirportList[i] = AirportList[j];
                AirportList[j] = aux;
            }
        }
    }
}


/* Gets an airport id and prints its city country and flights */
void print_airport_info(char id[]){
    int i;
    if ((i = Search_Airport_by_ID(id)) == -1){

        printf(ERROR_NO_SUCH_AIRPORT_ID,id);
        return;
    }
    printf("%s %s %s %d\n", id, AirportList[i].city, AirportList[i].country,
     AirportList[i].flights);
}


/* Makes an airport structure and inserts it into an airport array */
void make_airport(char id[], char country[], char city[]){
    Airport airport;
    if ( !(verify_airport(id)) ){
        return;
    }
    strcpy(airport.id,id);
    strcpy(airport.country,country);
    strcpy(airport.city,city);
    airport.flights = 0;
    AirportList[airport_n] = airport;
    printf(Airport_Print,id);
    airport_n +=1;
    return;
}


/*Gets an index and returns a copy of the flight
on that index on the flight array */
DestFlight Copy_destflight_struct(int i){
    DestFlight destflight;
    FullDate fulldate;

    strcpy(destflight.departid,FlightList[i].departid);
    strcpy(destflight.flightcode,FlightList[i].flightcode);

    fulldate = 
    sumtime_Date(FlightList[i].date, FlightList[i].time,FlightList[i].duration);

    destflight.date = fulldate.date;
    destflight.time = fulldate.time;
    return destflight;
}


/* Gets a date and two times and adds the sum of the times to the date
returns the new date and time */
FullDate sumtime_Date(Date date1, Time time1, Time time2){
    FullDate fulldate;
    int sum = sum_minutes(time1,time2);
    
    if (sum >= minutesinDay){
        date1 = Changes_MonthYear(date1);
        sum -= minutesinDay;
    }

    time1.hour = sum / 60;
    time1.minute = sum % 60;
    fulldate.date = date1;
    fulldate.time = time1;
    return fulldate;
}


/* Gets two time structures and returns their sum in minutes */
int sum_minutes(Time time1, Time time2){
    int min1,min2,sum;

    min2 = time2.hour*minutesinHour + time2.minute;
    min1 = time1.hour*minutesinHour + time1.minute;
    sum = min1 +min2;
    return sum;
}


/* Verifies if the Month or year will change after
 the addition of the time to the date */
Date Changes_MonthYear(Date date1){

        if  (date1.Month == Feb && date1.Day == (Month28)){
            date1.Day = 0;
            date1.Month +=1;
        }

        else if (is30Days(date1.Month) && (date1.Day == Month30)){
            date1.Day = 0;
            date1.Month +=1;
        }

        else if (!(is30Days(date1.Month))  && (date1.Day == Month31)){
            date1.Day = 0;

            if (date1.Month == Dec){
                date1.Year +=1;
                date1.Month = 0;
            }

            date1.Month +=1;
        }

        date1.Day +=1;

    return date1;
}


/*Verifies if a month has 30 days*/
int is30Days(int Month){
    int i;
    const int Months[n_Months_30Days] = {Apr,Jun,Sep,Nov};
    
    for (i=0; i < n_Months_30Days; i++){

        if (Month == Months[i]){
            return True;
        }
    }
    return False;
}


/* Gets a month a calculates how many days have passed
since the start of the year */
int Months_in_days(int Months){
    int days = 0;
    
    const int monthsdays[MonthsinYear] = 
    {0,JaninYear,FebinYear,MarinYear,AprinYear,MayinYear,JuninYear
    ,JulinYear,AuginYear,SepinYear,OctinYear,NovinYear};


    days = monthsdays[Months];
    
    return days;
}

void Insert_Reservation(Res* res,int i){
    
    Res* prev = FlightList[i].reservation;
    /*             Insert at Top          */
    if (strcmp(res->rcode,prev->rcode) < 0){
        res->next = prev;
        FlightList[i].reservation = res;
        return;
    }
    while (True){
        if (prev->next == NULL){
            prev->next = res;
            return;
        } 
        if (strcmp(res->rcode,prev->next->rcode) < 0){
            res->next = prev->next;
            prev->next = res;
            return;
        }

        prev = prev->next;
    }
}

void Print_Reservations(int i){
    Res* temp;
    temp = FlightList[i].reservation;
    while (temp != NULL){
        printf("%s %d\n",temp->rcode, temp->passengers);
        temp = temp->next;
    }

}


int verify_reservation_n(int p , int i){
    if ( (FlightList[i].reservation_n + p) > FlightList[i].capacity){
        return False;
    }
    return True;
}


int verify_reservation_duplicate(char* Code){
    int i;
    Res* temp;
    
    for (i=0; i < flight_n ;i++){
        temp = FlightList[i].reservation;
        while (temp != NULL){
            if ( strcmp(temp->rcode,Code) == 0 ){
                return False;
            }
        temp = temp->next;
        }
    }

    return True;
}


void remove_flight(char* Code){
    int i,j,found = False;

    for (i=0; i < flight_n; i++){

        if ( strcmp(FlightList[i].flightcode,Code) == 0){

            found = True;
            remove_reservations(i);
            flight_n -= 1;
            for (j=i; j < flight_n; j++){
                FlightList[j] = FlightList[j+1];
            }
            i -=1;
        }

    }

    if (!found){
        printf(ERROR_NOT_FOUND);
        return;
    }
}


void remove_FlightList(){
    int i;
    for (i=0; i<flight_n; i++)
        remove_reservations(i);
    free(FlightList);    
}

void remove_reservations(int i){
    Res *temp,*prev;


    if ( !(prev = FlightList[i].reservation) ) return;
    temp = prev->next;
    do{
        temp = prev->next;
        remove_res(prev,i);
        prev = temp;

    } while (temp != NULL);

}


void remove_res(Res *res,int i){
    FlightList[i].reservation_n -= res->passengers;
    free(res->rcode);
    free(res);
}

/*-----------------------------------AUXFUNCS-----------------------------*/


/*-------------------------------------------COMMANDSFUNCS-----------------*/




/* Makes an airport with the arguments read */
void command_a(){
    char id[MaxID],country[MaxCountry],city[MaxCity],cityaux[MaxCity];

    scanf("%s %s %s",id,country,city);
    fgets(cityaux,MaxCity,stdin);
    strncat(city,cityaux,MaxCity);
    city[strcspn(city, "\n")] = 0;
    make_airport(id,country,city);
}


/*Scans to see if the command "l" has arguments, and either lists all airports
by alphabetical order if it doesnt have arguments
or lists the asked ones by the order asked */
void command_l(){
    char id[MaxID], c;
    int i;
    if ((c=getchar()) == '\n'){
        sort_airports();
        for ( i = 0; i < airport_n; i++){
            printf("%s %s %s %d\n",AirportList[i].id, AirportList[i].city,
            AirportList[i].country, AirportList[i].flights);
        }
    }

    else{
        while ( c != '\n'){
            scanf("%s",id);
            c =getchar();

            print_airport_info(id);
        }
    }
}


/*Scans to see if the command "v" has arguments, if there are arguments
makes a flight if not prints existing flights by creation order */
void command_v(){
    int i;
    char c;
    Flight tempflight;
    /*Prints by creation order */
    if ((c=getchar()) == '\n'){
        for (i =0; i<flight_n; i++){

            printf("%s %s %s ", FlightList[i].flightcode, FlightList[i].departid
            , FlightList[i].destid);

            printf("%02d-%02d-%d ",FlightList[i].date.Day,
             FlightList[i].date.Month, FlightList[i].date.Year);

            printf("%02d:%02d\n",FlightList[i].time.hour,
             FlightList[i].time.minute);
        }
    }
                
    else{
        /*Makes a new Flight*/
        tempflight = Scan_Flight_Info(tempflight);
        make_flight(tempflight);
    }
    return;
}

/*Scans an airport id and prints the flights departing from that airport*/ 
void command_p(){
    int airport_i,airport_f,i,countedflights = 0;
    char id[MaxID];
    Flight temp[MaxFlights];

    scanf("%s",id);

    airport_i = Search_Airport_by_ID(id);

    if( airport_i == -1 ){
        printf(ERROR_NO_SUCH_AIRPORT_ID,id);
        return;
    }

    airport_f = AirportList[airport_i].flights;

    for (i=0; i < flight_n; i++){

        /* If there are no flights left break */
        if (countedflights == airport_f){
            break;
        }

        if ( strcmp(FlightList[i].departid,id) == 0 ){
            temp[countedflights] = FlightList[i];
            countedflights +=1;
        }
    }
    sort_flights(temp,countedflights);
    for(i=0; i < countedflights;i++){
        Print_DepartFlight_info(temp,i);
    }
}

/* Scans an airport id and prints the flights departing from that airport */ 
void command_c(){
    int i,temp_i = 0;
    char id[MaxID];
    DestFlight temp[MaxFlights];

    scanf("%s",id);

    if (!Airport_exists(id)){
        return;
    }
    
    for (i=0; i < flight_n; i++){
        if ( strcmp(FlightList[i].destid,id) == 0 ){
            temp[temp_i] = Copy_destflight_struct(i);
            temp_i +=1;
        }
    }

    sort_destflights(temp,temp_i);
    for(i=0; i < temp_i;i++){

        printf("%s %s ",temp[i].flightcode, temp[i].departid);

        printf("%02d-%02d-%d ",temp[i].date.Day, temp[i].date.Month,
        temp[i].date.Year);

        printf("%02d:%02d\n",temp[i].time.hour, temp[i].time.minute);
    }
}



/* Reads new date and updates existing one*/
void command_t(){
    int d;
    Date tempdate;

    scanf("%d-%d-%d", &tempdate.Day, &tempdate.Month, &tempdate.Year);

    if ( (d =Date_Distance(Today,tempdate)) <0 || d > DaysInYear){
        printf(ERROR_INVALID_DATE);
        return;
    }
    
    Today = tempdate;
    printf("%02d-%02d-%d\n",Today.Day, Today.Month, Today.Year);
    return;
}




void command_r(){
    char buffer[MaxRCode], *rCode, FlightCode[MaxFLightCode],c;
    Date date;
    int passengers;
    Res* reservation;
    int i;

    scanf("%s %d-%d-%d", FlightCode, &date.Day, &date.Month, &date.Year);
    /*with args*/
    if ((c = getchar()) != '\n'){
        scanf("%s %d", buffer,&passengers);
        rCode = (char*)malloc(sizeof(char)*(strlen(buffer)+1));
        strcpy(rCode,buffer);
        if ((i = verify_reservation(rCode,passengers,FlightCode,date)) == NonExistant){
            free(rCode);
            return;
        } 
        reservation = (Res*)malloc(sizeof(Res));
        reservation->passengers = passengers;
        reservation->rcode = rCode;
        reservation->next = NULL;
        if (FlightList[i].reservation == NULL){
            FlightList[i].reservation = reservation;
        }
        else{
           Insert_Reservation(reservation, i);
        }
        FlightList[i].reservation_n += passengers;
    }
    /*no args*/
    else{
        if (( i = VerifyFlightCodeandDate(FlightCode,date)) == NonExistant){
            return;
        }
        Print_Reservations(i);
    }
}


void command_e(){
    char buffer[MaxRCode],*Code;
    size_t len;
    scanf("%s",buffer);
    len = strlen(buffer);
    Code = (char*)malloc(sizeof(char)*(len+1));
    strcpy(Code,buffer);
    if (len < MinRCode){
        remove_flight(Code);
        free(Code);
    }
    else{
        FindandFreeRes(Code);
        free(Code);
    }
}

/*-------------------------------------------COMMANDSFUNCS-----------------*/


void FindandFreeRes(char* Code){
    int i;
    Res *prev,*temp;
    for (i = 0; i< flight_n ;i++){

        prev = FlightList[i].reservation;

        if (!prev) continue;

        if (strcmp(prev->rcode,Code) == 0){
            FlightList[i].reservation = prev->next;
            remove_res(prev,i);
            return;
        }

        for (temp = prev->next; temp != NULL; temp=temp->next){

            if (strcmp(temp->rcode,Code) == 0){
                prev->next = temp->next;
                remove_res(temp,i);
                return;
            }
            prev = temp;
        }
    }
    printf(ERROR_NOT_FOUND);
}