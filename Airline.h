#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"

typedef enum {
	source, destanation, date, None, eNofSortType
} eSortType;

static const char* SortType[eNofSortType]
= { "source", "destanation","date", "None"};

typedef struct
{
	char*		name;
	int			flightCount;
	Flight**	flightArr;
	int			planeCount;
	Plane*		planeArr;
	eSortType	type;
}Airline;

void	initAirline(Airline* pComp);
int		addFlight(Airline* pComp,const AirportManager* pManager);
int		addPlane(Airline* pComp);
Plane*	FindAPlane(Airline* pComp);
void	printCompany(const Airline* pComp);
void	printFlightArr(Flight** arr, int size);
void	printPlanesArr(Plane* arr,int size);
void	doPrintFlightsWithPlaneType(const Airline* pComp);
void	freeFlightArr(Flight** arr, int size);
void	freePlanes(Plane* arr, int size);
void	freeCompany(Airline* pComp);

void sortFlightsMenu(Airline* air, Flight** arr, int size);
void sortBySourceCode(Airline* pComp);
void sortByDestCode(Airline* pComp);
void sortByDate(Airline* pComp);

int compareFlightByDest(const void* flight1, const void* flight2);
int compareFlightBySource(const void* flight1, const void* flight2);

Flight* searchFlights(Airline air, Flight toSearch);
#endif

