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
#define TABLE_SIZE 20047

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
#define ERROR_NO_MEMORY "no memory\n"



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


struct res_list{
    char* rcode;
    int passengers;
    struct res_list* next;
    struct res_list* prev;
};

typedef struct res_list Res_List;


struct flight_Struct;

typedef struct res_hash{
    Res_List* res_in_list;
    struct flight_Struct* flight;
    struct res_hash* next;
}Res_Hash;




typedef struct f_hash{
    struct flight_Struct* flight_in_list;
    struct f_hash* next;
}Flight_Hash;


typedef struct flight_Struct{

    char flightcode[MaxFLightCode];
    char departid[MaxID];
    char destid[MaxID];
    Date date;
    Time time;
    Time duration;
    int capacity;
    Res_List* reservation;
    int reservation_n;
    int index;
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
int exists_flight(char* code,Date date);

/* Make Structures */

void make_airport(char id[], char country[], char city[]);
void make_flight(Flight* flight);

/*Sorting*/

void sort_flights(Flight **farray, int flight_n);
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
void Print_DepartFlight_info(Flight** temp,int i);
void print_airport_info(char id[]);
Flight* Scan_Flight_Info(void);
int is30Days(int Month);
FullDate sumtime_Date(Date date1, Time time1, Time time2);
void release_all_mem(void);
void check_mem(void *ptr);
void remove_res(Res_List* res,Flight* flight);
void remove_reservations(int i);
void remove_FlightList(void);
void remove_flight_hash(int hash,char* Code);
void flight_hash_insert(Flight_Hash *flight);
Flight_Hash *flight_hash_search(int i, char* flightcode);

/*Verification Functions */
int verify_airport(char id[]);
int verify_flight(Flight flight);
int Verify_Flight_aux(Flight flight);
int verify_flightcode(char* code);
int Airport_exists(char id[]);
int verify_reservationCode(char* str);
int verify_reservation(char* rCode,int p,char FlightCode[MaxFLightCode], Date date);
Res_List *Insert_Reservation(Res_List* res, Flight* flight);
int VerifyFlightCodeandDate(char* FlightCode, Date date);
void Print_Reservations(int i);
int verify_reservation_n(int p , int i);
void remove_res_list(Res_List* res, Flight* flight);
unsigned int hash_func(char* key);
void res_delete(char *rCode);
Res_Hash *hash_search(char* rCode);
void hash_insert(Res_Hash *res);
void remove_flights(char* Code);


Flight_Hash **flight_table;

Res_Hash **hash_table;
/* List with all airports */
Airport *AirportList;

/* List with all flights */
Flight **FlightList;

/* Number of airports */
int airport_n = 0;

/* Number of Flights */
int flight_n = 0;

/* Current Date */
Date Today = {FirstDay, FirstMonth, FirstYear};



/*Code*/
int main(){
    char command;

    FlightList = (Flight**)calloc(MaxFlights,sizeof(Flight));
    AirportList = (Airport*)calloc(MaxAirports,sizeof(Airport));
    hash_table = (Res_Hash**)calloc(TABLE_SIZE,sizeof(Res_Hash*));
    flight_table = (Flight_Hash**)calloc(TABLE_SIZE,sizeof(Flight_Hash*));
    

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
    release_all_mem();
    return 0;
}



/*Gets a flight array and sorts its from the oldest date
and time to the most recent one */
void sort_flights(Flight** farray,int f_n){
    int i,j,days;
    Flight *aux;
    for (i=0; i < f_n-1; i++){

        for (j=i+1; j < f_n; j++){

            if ( (days = Date_Distance(farray[i]->date,farray[j]->date))<0
            || ( (days == 0)
            && (time_distance(farray[i]->time,farray[j]->time)) < 0) ){
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

    if( exists_flight(flight.flightcode,flight.date) != NonExistant ){
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
    int days,duration_hour;

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

    if ( flight.capacity < MinCapacity){
        printf(ERROR_INVALID_CAPACITY);
        return False;
    }

    return True;
}


/* Gets a flightcode and verifies if the code is valid */
int verify_flightcode(char* code){
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



int verify_reservation(char* rCode,int p,char* FlightCode, Date date){
    int d,i;

    if (!verify_reservationCode(rCode)){
        printf(ERROR_INVALID_RESERVATION_CODE);
        return NonExistant;
    }
     
    if ( (i = exists_flight(FlightCode,date)) == NonExistant ){
        printf(ERROR_FLIGHT_DOES_NOT_EXIST,FlightCode);
        return NonExistant;
    }

    if (hash_search(rCode) ){
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
    if ( (i = exists_flight(FlightCode,date)) == NonExistant){
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
void Print_DepartFlight_info(Flight** temp,int i){

    printf("%s %s ",temp[i]->flightcode, temp[i]->destid);

    printf("%02d-%02d-%d ",temp[i]->date.Day, temp[i]->date.Month,
    temp[i]->date.Year);

    printf("%02d:%02d\n",temp[i]->time.hour, temp[i]->time.minute);

}




/*---------------------------------VERFUNCS------------------------------------------*/



/*-----------------------------------AUXFUNCS-----------------------------*/




/*Scans information and fills the flight structure with it*/
Flight* Scan_Flight_Info(){
    Flight *tempflight = (Flight*)calloc(1,sizeof(Flight));

    scanf("%s %s %s", tempflight->flightcode, tempflight->departid,
    tempflight->destid);

    scanf("%d-%d-%d", &tempflight->date.Day, &tempflight->date.Month,
    &tempflight->date.Year);

    scanf("%d:%d", &tempflight->time.hour, &tempflight->time.minute);

    scanf("%d:%d %d", &tempflight->duration.hour, &tempflight->duration.minute,
    &tempflight->capacity);

    return tempflight;
}


/* Gets a temporary flight and the current date
and inserts the flight into the flight array and adds it to the depart airport*/
void make_flight(Flight* tempflight){
    int airport_i;

    Flight_Hash *flighthash = (Flight_Hash*)calloc(1,sizeof(Flight_Hash));


    if ( !(verify_flight(*tempflight)) ){
        return;
    }

    tempflight->reservation = NULL;
    tempflight->reservation_n = 0;
    airport_i = Search_Airport_by_ID(tempflight->departid);
    AirportList[airport_i].flights += 1;
    tempflight->index = flight_n;

    FlightList[flight_n] = tempflight;

    flighthash->flight_in_list = FlightList[flight_n];
    flighthash->next = NULL;
    flight_hash_insert(flighthash);

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

    strcpy(destflight.departid,FlightList[i]->departid);
    strcpy(destflight.flightcode,FlightList[i]->flightcode);

    fulldate = 
    sumtime_Date(FlightList[i]->date, FlightList[i]->time,FlightList[i]->duration);

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

Res_List* Insert_Reservation(Res_List* res,Flight* flight){
    Res_List *temp;
    temp = flight->reservation;
    /*             Insert When List Is Empty          */
    if (!temp){
        flight->reservation = res;
        return res;
    }
     /*             Insert at Top         */
    if (strcmp(res->rcode,temp->rcode) < 0){
        res->next = temp;
        res->prev = NULL;
        temp->prev = res;
        flight->reservation = res;
        return res;
    }

    while ( temp->next  && strcmp(res->rcode,temp->next->rcode) > 0){
        temp = temp->next;
    }
    res->next = temp->next;

    if ( temp->next)
        res->next->prev = res;

    temp->next = res;
    res->prev = temp;
        
    return res;
}

void Print_Reservations(int i){
    Res_List* temp;
    temp = FlightList[i]->reservation;
    while (temp != NULL){
        printf("%s %d\n",temp->rcode, temp->passengers);
        temp = temp->next;
    }

}


int verify_reservation_n(int p , int i){
    if ( (FlightList[i]->reservation_n + p) > FlightList[i]->capacity){
        return False;
    }
    return True;
}






/*-----------------------------------AUXFUNCS-----------------------------*/


/*-------------------------------------------COMMANDSFUNCS-----------------*/




/* Makes an airport with the arguments read */
void command_a(){
    char id[MaxID],country[MaxCountry],city[MaxCity],cityaux[MaxCity];

    scanf("%3s %30s %s",id,country,city);
    fgets(cityaux,MaxCity,stdin);
    strcat(city,cityaux);
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
    /*Prints by creation order */
    if (getchar() == '\n'){
        for (i =0; i<flight_n; i++){

            printf("%s %s %s ", FlightList[i]->flightcode, FlightList[i]->departid
            , FlightList[i]->destid);

            printf("%02d-%02d-%d ",FlightList[i]->date.Day,
             FlightList[i]->date.Month, FlightList[i]->date.Year);

            printf("%02d:%02d\n",FlightList[i]->time.hour,
             FlightList[i]->time.minute);
        }
    }
                
    else{
        /*Makes a new Flight*/
        make_flight(Scan_Flight_Info());
    }
    return;
}

/*Scans an airport id and prints the flights departing from that airport*/ 
void command_p(){
    int airport_i,airport_f,i,countedflights = 0;
    char id[MaxID];
    Flight *temp[MaxFlights];

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

        if ( strcmp(FlightList[i]->departid,id) == 0 ){
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
        if ( strcmp(FlightList[i]->destid,id) == 0 ){
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
    char buffer[MaxRCode], *rCode, FlightCode[MaxFLightCode];
    Date date;
    int passengers;
    Res_List* list_res;
    Res_Hash* hash_res;
    Flight* temp;
    int i;

    scanf("%s %d-%d-%d", FlightCode, &date.Day, &date.Month, &date.Year);
    /*with args*/
    if (getchar() != '\n'){
        scanf("%s %d", buffer,&passengers);
        rCode = (char*)calloc((strlen(buffer)+1),sizeof(char));
        check_mem(rCode);
        strcpy(rCode,buffer);

        if ((i = verify_reservation(rCode,passengers,FlightCode,date)) == NonExistant){
            free(rCode);
            return;
        }
        temp = FlightList[i];

        list_res = (Res_List*)calloc(1,sizeof(Res_List));
        check_mem(list_res);

        hash_res = (Res_Hash*)calloc(1,sizeof(Res_Hash));
        check_mem(hash_res);

        temp->reservation_n += passengers;
        list_res->passengers = passengers;
        list_res->rcode = rCode;

    
        hash_res->res_in_list = Insert_Reservation(list_res, temp);
        hash_res->flight = temp;

        hash_insert(hash_res);
        
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
    Code = (char*)calloc((len+1),sizeof(char));
    check_mem(Code);
    strcpy(Code,buffer);

    if (len < MinRCode){
        remove_flights(Code);
        free(Code);
    }
    else{
        res_delete(Code);
        free(Code);
    }
}

/*-------------------------------------------COMMANDSFUNCS-----------------*/


void check_mem(void *ptr){
    if (ptr) return;
    release_all_mem();
    printf(ERROR_NO_MEMORY);
    exit(0);
}

void release_all_mem(){
    free(AirportList);
    remove_FlightList();
    free(hash_table);
    free(flight_table);
    free(FlightList);
    return;
}


unsigned int hash_func(char* key) {
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
    return hash % TABLE_SIZE;
}

void hash_insert(Res_Hash *res){
    int i = hash_func(res->res_in_list->rcode);
    res->next = hash_table[i];
    hash_table[i] = res;
}

Res_Hash *hash_search(char* rCode){
    unsigned int i = hash_func(rCode);
    Res_Hash *temp = hash_table[i];
    while (temp && strcmp(temp->res_in_list->rcode,rCode) != 0){
        temp = temp->next;
    }
    return temp;
}

void res_delete(char *rCode){
    int i = hash_func(rCode);
    Res_Hash *temp = hash_table[i];
    Res_Hash *prev = NULL;


    while (temp && strcmp(temp->res_in_list->rcode,rCode) != 0){
        prev = temp;
        temp = temp->next;
    }

    if (!temp){
        printf(ERROR_NOT_FOUND);
        return;
    }

    if (!prev){
        hash_table[i] = temp->next;
    }

    else{
        prev->next = temp->next;
    }
    remove_res_list(temp->res_in_list,temp->flight);
    free(temp);
}

void remove_res_list(Res_List* res, Flight* flight){
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
    
    remove_res(res,flight);
}

void remove_flights(char* Code){
    int i,j,found = False;
    int hash = hash_func(Code);
    Flight_Hash* temp;
    
    while ( (temp = flight_hash_search(hash,Code)) ){
        found = True;

        remove_reservations((i=temp->flight_in_list->index));
        remove_flight_hash(hash,Code);

        flight_n -=1;
        for (j=i; j < flight_n; j++){

            FlightList[j+1]->index = j;
            FlightList[j] = FlightList[j+1];
        }

    }

    if (!found){
        printf(ERROR_NOT_FOUND);
        return;
    }

}


void remove_flight_hash(int hash,char* Code){
    Flight_Hash *temp = flight_table[hash];
    Flight_Hash *prev = NULL;


    while (temp && strcmp(temp->flight_in_list->flightcode,Code) != 0){
        prev = temp;
        temp = temp->next;
    }

    if (!temp){
        return;
    }

    if (!prev){
        flight_table[hash] = temp->next;
    }

    else{
        prev->next = temp->next;
    }
    free(temp->flight_in_list);
    free(temp);
}

void remove_FlightList(){
    int i;
    char* code;
    for (i=0; i<flight_n; i++){
        code = FlightList[i]->flightcode;
        remove_reservations(i);
        remove_flight_hash(hash_func(code),code);
    }


}

void remove_reservations(int i){
    Res_List *temp;

    while (FlightList[i]->reservation){
        temp = FlightList[i]->reservation;
        FlightList[i]->reservation = temp->next;
        res_delete(temp->rcode);
    }
}


void remove_res(Res_List *res,Flight* flight){
    flight->reservation_n -= res->passengers;
    free(res->rcode);
    free(res);
}


Flight_Hash *flight_hash_search(int i,char* flightcode){
    Flight_Hash *temp = flight_table[i];
    while (temp && strcmp(temp->flight_in_list->flightcode,flightcode) != 0){
        temp = temp->next;
    }
    return temp;
}

void flight_hash_insert(Flight_Hash *flight){
    int i = hash_func(flight->flight_in_list->flightcode);
    flight->next = flight_table[i];
    flight_table[i] = flight;
}


int  exists_flight(char* code,Date date){
    int i = hash_func(code);
    int j = NonExistant;

    Flight_Hash *temp = flight_table[i];
    while (temp && (strcmp(temp->flight_in_list->flightcode,code) ||
     Date_Distance(date,temp->flight_in_list->date) )){

        temp = temp->next;
    }
    if (temp) j =temp->flight_in_list->index;

    return j;
}