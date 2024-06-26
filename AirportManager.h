#ifndef __AIR_MANAGER__
#define __AIR_MANAGER__

#include "Airport.h"
#include "listGen.h"

typedef struct
{
    LIST AirportList; 
} AirportManager;

int     initManager(AirportManager* pManager, const char* fileName);
int     loadManagerFromFile(AirportManager* pManager, const char* fileName);
int	    saveManagerToFile(const AirportManager* pManager, const char* fileName);
int		addAirport(AirportManager* pManager);
int		initAirport(Airport* pPort, AirportManager* pManager);
Airport* findAirportByCode(const AirportManager* pManager, const char* code);
int		checkUniqeCode(const char* code, const AirportManager* pManager);
void	printAirports(const AirportManager* pManager);
void	freeManager(AirportManager* pManager);
int 	lengthList(NODE* pNode);
int		compareByCode(const void* port1, const void* port2);
int     hasXorMoreAirports(const AirportManager* pManager, int x);

#endif