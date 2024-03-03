#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Airline.h"
#include "Date.h"
#include "Airport.h"
#include "General.h"
#include "fileHelper.h"

void	initAirline(Airline* pComp)
{
	//printf("-----------  Init Airline\n");
	pComp->name = getStrExactName("Enter Airline name");
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
	pComp->planeArr = NULL;
	pComp->planeCount = 0;
	pComp->type = None;
}

int saveAirlineToFile(const Airline* pComp, const char* fileName) {
	FILE* file = fopen(fileName, "wb");
	if (!file) {
		return 0;
	}

	// Write the name
	if (!writeStringTobinFile(file, pComp->name)) {
		fclose(file);
		return 0;
	}
	// Write the planes
	if (!savePlaneArrToBinFile(file, pComp->planeArr, pComp->planeCount)) {
		fclose(file);
		return 0;
	}
	// Write the flights
	if (!saveFlightArrToBinFile(file, pComp->flightArr, pComp->flightCount)) {
		fclose(file);
		return 0;
	}

	fclose(file);
	return 1;
}


int initAirlineFromFile(Airline* pComp, AirportManager* pManager, const char* fileName) {
	FILE* file = fopen(fileName, "rb");
	if (!file) {
		// TDDO : add error message
		return 0;
	}
	// Read the name
	char* name = readStringFromBinFile(file);
	if (!name) {
		fclose(file);
		return 0;
	}
	// Read the planes
	Plane* tempPlaneArr = NULL;
	int tempPlaneCount = 0;
	tempPlaneArr = readPlaneArrFromBinFile(file, &tempPlaneCount);
	if (!tempPlaneArr) {
		fclose(file);
		return 0;
	}
	// Read the flights
	Flight** restoredFlightArr = NULL;
	int tempFlightCount = 0;
	restoredFlightArr = readFlightArrFromBinFile(file, tempPlaneArr, tempPlaneCount, &tempFlightCount);
	if (!restoredFlightArr) {
		fclose(file);
		return 0;
	}

	// Check if the read was successful and set the values
	pComp->name = name;
	pComp->planeCount = tempPlaneCount;
	pComp->planeArr = tempPlaneArr;
	pComp->flightCount = tempFlightCount;
	pComp->flightArr = restoredFlightArr;
	pComp->type = None;
	fclose(file);
	return 1;
}

int	addFlight(Airline* pComp, const AirportManager* pManager) {
	int count = 0;
	if (!hasXorMoreAirports(pManager, 2)) {
		printf("There are not enough airport to set a flight\n");
		return 0;
	}
	if (pComp->planeCount == 0)
	{
		printf("There is no plane in company\n");
		return 0;
	}

	Flight* pFlight = (Flight*)calloc(1, sizeof(Flight));
	if (!pFlight)
		return 0;

	Plane* thePlane = FindAPlane(pComp);
	printAirports(pManager);
	initFlight(pFlight, thePlane, pManager);

	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount + 1) * sizeof(Flight*));
	if (!pComp->flightArr)
	{
		free(pFlight);
		return 0;
	}
	pComp->flightArr[pComp->flightCount] = pFlight;
	pComp->flightCount++;
	return 1;
}

int		addPlane(Airline* pComp)
{
	pComp->planeArr = (Plane*)realloc(pComp->planeArr, (pComp->planeCount + 1) * sizeof(Plane));
	if (!pComp->planeArr)
		return 0;
	initPlane(&pComp->planeArr[pComp->planeCount], pComp->planeArr, pComp->planeCount);
	pComp->planeCount++;
	return 1;
}

Plane* FindAPlane(Airline* pComp)
{
	printf("Choose a plane from list, type its serial Number\n");
	printPlanesArr(pComp->planeArr, pComp->planeCount);
	int sn;
	Plane* temp = NULL;
	do {
		scanf("%d", &sn);
		temp = findPlaneBySN(pComp->planeArr, pComp->planeCount, sn);
		if (!temp)
			printf("No plane with that serial number! Try again!\n");
	} while (temp == NULL);

	return temp;
}


void printCompany(const Airline* pComp)
{
	printf("Airline %s\n", pComp->name);
	printf("\n -------- Has %d planes\n", pComp->planeCount);
	printPlanesArr(pComp->planeArr, pComp->planeCount);
	printf("\n\n -------- Has %d flights\n", pComp->flightCount);
	printFlightArr(pComp->flightArr, pComp->flightCount);
}

void	printFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
		printFlight(arr[i]);
}

void	printPlanesArr(Plane* arr, int size)
{
	for (int i = 0; i < size; i++)
		printPlane(&arr[i]);
}

void	doPrintFlightsWithPlaneType(const Airline* pComp)
{
	ePlaneType type = getPlaneType();
	int count = 0;
	printf("Flights with plane type %s:\n", GetPlaneTypeStr(type));
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isPlaneTypeInFlight(pComp->flightArr[i], type))
		{
			printFlight(pComp->flightArr[i]);
			count++;
		}
	}
	if (count == 0)
		printf("Sorry - could not find a flight with plane type %s:\n", GetPlaneTypeStr(type));
	printf("\n");
}

void	freeFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
		free(arr[i]);
}

void	freePlanes(Plane* arr, int size)
{
	for (int i = 0; i < size; i++)
		freePlane(&arr[i]);
}

void	freeCompany(Airline* pComp)
{
	freeFlightArr(pComp->flightArr, pComp->flightCount);
	free(pComp->flightArr);
	free(pComp->planeArr);
	free(pComp->name);
}

// compare types
int compareFlightBySource(const void* flight1, const void* flight2) {
	if (!flight1 || !flight2)
		return 0;
	const Flight* pFlight1 = *(const Flight**)flight1;
	const Flight* pFlight2 = *(const Flight**)flight2;

	return strcmp(pFlight1->sourceCode, pFlight2->sourceCode);
}

int compareFlightByDest(const void* flight1, const void* flight2) {
	if (!flight1 || !flight2)
		return 0;
	const Flight* pFlight1 = *(const Flight**)flight1;
	const Flight* pFlight2 = *(const Flight**)flight2;

	return strcmp(pFlight1->destCode, pFlight2->destCode);

}

// sorting types
void sortBySourceCode(Airline* pComp) {
	if (pComp == NULL || pComp->flightArr == NULL || pComp->flightCount == 0)
		return;
	qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareFlightBySource);
}
void sortByDestCode(Airline* pComp) {
	if (pComp == NULL || pComp->flightArr == NULL || pComp->flightCount == 0)
		return;
	qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareFlightByDest);
}
void sortByDate(Airline* pComp) {
	if (pComp == NULL || pComp->flightArr == NULL || pComp->flightCount == 0)
		return;
	qsort(pComp->flightArr, pComp->flightCount, sizeof(Flight*), compareFlightByDate);
}


void sortFlight(Airline* air) {
	int op = 0;
	while (1) {
		printf("Base on what field do you want to sort ?\n"
			"Enter 1 for Source Code\n"
			"Enter 2 for Dest Code\n"
			"Enter 3 for Date\n");
		op = scanf("%d", &op);
		switch (op) {
		case 1:
			air->type = source;
			sortBySourceCode(air);
			break;

		case 2:
			air->type = destanation;
			sortByDestCode(air);
			break;

		case 3:
			air->type = date;
			sortByDate(air);
			break;
		}
	}
}

// search
Flight* findFlight(Airline *air) {
	if (air == NULL || air->flightArr == NULL || air->flightCount == 0) {
		printf("The search cannot be performed, array not sorted\n");
		return NULL;
	}
	if (air->type == None) {
		printf("The search cannot be performed, array not sorted\n");
		return NULL;
	}
	printf("Origin: Enter airport code  - 3 UPPER CASE letters\n");
	Flight * toSearch = (Flight*)calloc(1, sizeof(Flight));
	Flight* founded = NULL;

	if (air->type == source) {
		printf("Origin: ");
		founded= (Flight*)bsearch(&toSearch, air->flightArr, air->flightCount, sizeof(Flight), compareFlightBySource);
	}
	else if (air->type == destanation) {
	printf("Destination: ");
		founded = (Flight*)bsearch(&toSearch, air->flightArr, air->flightCount, sizeof(Flight), compareFlightByDest);
	}
	else if (air->type == date) {
		founded = (Flight*)bsearch(&toSearch, air->flightArr, air->flightCount, sizeof(Flight), compareFlightByDate);
	}

	if (founded) {
		printf("Flight found, ");
		printFlight(founded);
	}
	else {
		printf("Flight was not found\n");
	}
	return founded;
	free(toSearch);
}