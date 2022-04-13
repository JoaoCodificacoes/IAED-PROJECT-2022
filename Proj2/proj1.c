/*-------------------IAED PROJECT------------------*/
/*             João David Natário Ferreira         */
/*                     103680                      */
/* ------------------------------------------------*/


/*Libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "proj1.h"


/*Code*/
int main(){
    char command;

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
        }
    }
    return 0;
}


/* Makes an airport with the arguments read */
void command_a(){
    char id[MaxID],country[MaxCountry],city[MaxCity],cityaux[MaxCity];

    scanf("%s %s %s",id,country,city);
    fgets(cityaux,MaxCity,stdin);
    strncat(city,cityaux,MaxCity);
    city[strcspn(city, "\n")] = 0;
    make_airport(id,country,city);
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


/* Gets and airport array and a airport id  and verifies if the airport exists*/
int Airport_exists(char id[]){
    if (Search_Airport_by_ID(id) == -1){
        printf(ERROR_NO_SUCH_AIRPORT_ID,id);
        return False;
    }
    return True;
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
and inserts the flight into the flight array and adds itsto the depart airport*/
void make_flight(Flight flight){
    int airport_i;

    if ( !(verify_flight(flight)) ){
        return;
    }

    airport_i = Search_Airport_by_ID(flight.departid);
    AirportList[airport_i].flights += 1;
    FlightList[flight_n] = flight;
    flight_n +=1;
    return ;
}


/*Gets a temporary flight,and the current date
and verifies if the temporary flight is valid */
int verify_flight(Flight flight){
    int i;
    if (!(verify_flightcode(flight.flightcode))){
        printf(ERROR_INVALID_FLIGHT_CODE);
        return False;
    }

    if(( i= Search_Flight_by_Code(flight.flightcode)) != -1
    && FlightList[i].date.Day ==flight.date.Day){
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


/*Gets a flight code and returns its index on the flight array
 or -1 if it doesnt exist */
int Search_Flight_by_Code(char code[]){
    int i,farray_i = -1;

    for (i=0; i < flight_n; i++){

        if (strcmp(code,FlightList[i].flightcode) == 0 ){
            farray_i = i;
            break;
        }
    }
    return farray_i;
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

    if ( (capacity = flight.capacity) < MinCapacity || capacity > MaxCapacity){
        printf(ERROR_INVALID_CAPACITY);
        return False;
    }

    return True;
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