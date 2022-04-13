

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
    char destid[MaxID];
    Date date;
    Time time;
    Time duration;
    int capacity;

}Flight;

typedef struct{

    char flightcode[MaxFLightCode];
    char departid[MaxID];
    Date date;
    Time time;

}DestFlight;

/* List with all airports */
Airport AirportList[MaxAirports];

/* List with all flights */
Flight FlightList[MaxFlights];

/* Number of airports */
int airport_n = 0;

/* Number of Flights */
int flight_n = 0;

/* Current Date */
Date Today = {FirstDay, FirstMonth, FirstYear};


/*Prototypes*/

/* Commands */

void command_a(void);
void command_l(void);
void command_v(void);
void command_p(void);
void command_t(void);
void command_c(void);

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
int Flight_exists(char code[], Date date);
int time_distance(Time time1, Time time2);
void Print_DepartFlight_info(Flight temp[],int i);
void print_airport_info(char id[]);
Flight Scan_Flight_Info(Flight flight);
int is30Days(int Month);
FullDate sumtime_Date(Date date1, Time time1, Time time2);
/*Verification Functions */

int verify_airport(char id[]);
int verify_flight(Flight flight);
int Verify_Flight_aux(Flight flight);
int verify_flightcode(char code[]);
int Airport_exists(char id[]);

