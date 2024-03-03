#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Flight.h"
#include "General.h"


void	initFlight(Flight* pFlight,Plane* thePlane, const AirportManager* pManager)
{
	Airport* pPortOr = setAiportToFlight(pManager, "Enter code of origin airport:");
	strcpy(pFlight->sourceCode, pPortOr->code);
	int same;
	Airport* pPortDes;
	do {
		pPortDes = setAiportToFlight(pManager, "Enter code of destination airport:");
		same = isSameAirport(pPortOr, pPortDes);
		if (same)
			printf("Same origin and destination airport\n");
	} while (same);
	strcpy(pFlight->destCode, pPortDes->code);
	pFlight->flightPlane = *thePlane;
	getCorrectDate(&pFlight->date);
}

int		isFlightFromSourceAirport(const Flight* pFlight, const char* code)
{
	if (strcmp(pFlight->sourceCode, code) == 0)
		return 1;
		
	return 0;
}


int		isFlightToDestAirport(const Flight* pFlight, const char* code)
{
	if (strcmp(pFlight->destCode, code) == 0)
		return 1;

	return 0;


}

int		isPlaneTypeInFlight(const Flight* pFlight, ePlaneType type)
{
	return (pFlight->flightPlane.type == type);
}


void	printFlight(const Flight* pFlight)
{
	printf("Flight From %s To %s\t",pFlight->sourceCode, pFlight->destCode);
	printDate(&pFlight->date);
	printf("\t");
	printPlane(&pFlight->flightPlane);
}

Airport* setAiportToFlight(const AirportManager* pManager, const char* msg)
{
	char code[MAX_STR_LEN];
	Airport* port;
	do
	{
		printf("%s\t", msg);
		getAirportCode(code);
		port = findAirportByCode(pManager, code);
		if (port == NULL)
			printf("No airport with this code - try again\n");
	} while (port == NULL);

	return port;
}

int    compareFlightByDate(const void* flight1, const void* flight2) {
	Flight* f1 = *(Flight**)flight1;
	Flight* f2 = *(Flight**)flight2;

	return compareDates(&f1->date, &f2->date);

}

int saveFlightToBinFile(FILE* file, const Flight* pFlight) {
	if (file == NULL || pFlight == NULL) {
		return 0;
	}

	// Write source code
	if (fwrite(pFlight->sourceCode, sizeof(char), IATA_LENGTH, file) != IATA_LENGTH) {
		return 0;
	}

	// Write destination code
	if (fwrite(pFlight->destCode, sizeof(char), IATA_LENGTH, file) != IATA_LENGTH) {
		return 0;
	}
	// Write plane Serial Number *ONLY*
	if (!savePlaneSerialNumberBinFile(file, &pFlight->flightPlane)) {
		return 0;
	}
	// Write date
	if (!saveDateToBinFile(file, &pFlight->date)) {
		return 0;
	}
	return 1;
}


Flight* readFlightFromBinFile(FILE* file, Plane* planeArr, int planeCount) {
	// this function allocates memory for the flight and returns the flight *be careful*
	if (file == NULL || planeArr == NULL || planeCount <= 0) {
		return NULL;
	}
	char sourceCode[IATA_LENGTH + 1];
	char destCode[IATA_LENGTH + 1];
	// read source code
	if (fread(sourceCode, sizeof(char), IATA_LENGTH, file) != IATA_LENGTH) {
		return NULL;
	}
	sourceCode[IATA_LENGTH] = '\0'; // Add Null-terminate

	// read destination code
	if (fread(destCode, sizeof(char), IATA_LENGTH, file) != IATA_LENGTH) {
		return NULL;
	}
	destCode[IATA_LENGTH] = '\0'; // Add Null-terminate

	// read plane (use function that reads only serial number and return the plane)
	Plane restoredPlane = readPlaneSerialNumberBinFile(file, planeArr, planeCount);
	if (restoredPlane.serialNum == -1) {
		return NULL;
	}

	// read date
	Date restoredDate = readDateFromBinFile(file);
	if (restoredDate.year == 0) {
		return NULL;
	}

	Flight* pFlight = (Flight*)malloc(sizeof(Flight));
	if (!pFlight) {
		return NULL;
	}

	strcpy(pFlight->sourceCode, sourceCode);
	strcpy(pFlight->destCode, destCode);
	pFlight->flightPlane = restoredPlane;
	pFlight->date = restoredDate;

	return pFlight;
}


int saveFlightArrToBinFile(FILE* file, const Flight** flightArr, int flightCount) {
	if (file == NULL || flightArr == NULL || flightCount <= 0) {
		return 0;
	}
	// Write number of flights
	if (!writeGeneralToBinFile(file, &flightCount, sizeof(int))) {
		return 0;
	}
	// Write each flight
	for (int i = 0; i < flightCount; i++)
	{
		if (!saveFlightToBinFile(file, flightArr[i])) {
			return 0;
		}
	}
	return 1;
}


Flight** readFlightArrFromBinFile(FILE* file, const Plane* planeArr, int planeCount, int* restoredCount) {
	// this function allocates memory for the flights and returns the flights *be careful*
	if (file == NULL || planeArr == NULL || planeCount <= 0) {
		return NULL;
	}
	// Read number of flights
	int tempCount;
	if (!readGeneralFromBinFile(file, &tempCount, sizeof(int))) {
		return NULL;
	}
	// allocate memory for the flights
	Flight** flightArr = (Flight**)malloc(tempCount * sizeof(Flight*));

	// add each flight to the array
	for (int i = 0; i < tempCount; i++) {
		Flight* tempFlight = readFlightFromBinFile(file, planeArr, planeCount);
		// if reading falied free all the flights that were read and return NULL
		if (tempFlight == NULL) {
			// free all the flights that were read
			for (int j = 0; j < i; j++) {
				free(flightArr[j]);
			}
			*restoredCount = -1;
			return NULL;
		}
		flightArr[i] = tempFlight;
	}

	*restoredCount = tempCount;
	return flightArr;
}