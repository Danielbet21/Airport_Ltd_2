#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "AirportManager.h"
#include "General.h"

int	initManager(AirportManager* pManager){
	L_init(&pManager->AirportList);
	return 1;
}

int compareByCode(const void* port1, const void* port2) {
	return strcmp(((Airport*)port1)->code, ((Airport*)port2)->code);
}

int	addAirport(AirportManager* pManager){
	// create new airport to add the list
	Airport* pPort = (Airport*)malloc(sizeof(Airport));
	if (!pPort){
		free(pPort);
		return 0;
	}
	// initialize the airport
	if (!(initAirport(pPort, pManager))) {
		freeAirport(pPort);
		return 0;
	}

	// TODO: add the new airport to the list
	//L_insert_sort_by_code(pManager->AirportList,pPort,(int)compareByCode);
	return 1;
}

int  initAirport(Airport* pPort, AirportManager* pManager){
	while (1){
		getAirportCode(pPort->code);
		if (checkUniqeCode(pPort->code, pManager)){
			break;
		}

		printf("This code already in use - enter a different code\n");
	}

	initAirportNoCode(pPort);
	return 1;
}

Airport* findAirportByCode(const AirportManager* pManager, const char* code){
		Airport pPort = { .code = code };//initialize only the code of the airport

		NODE* res = L_find(pManager->AirportList.head.next, &pPort, compareByCode);
		if (res == NULL) {
			return NULL;
		}
		else {
			return res;
		}
	}

int checkUniqeCode(const char* code,const AirportManager* pManager){
	Airport* port = findAirportByCode(pManager, code);

	if (port != NULL)
		return 0;

	return 1;
}


void printAirports(const AirportManager* pManager){
	L_print(&pManager->AirportList, printAirport);
}

void	freeManager(AirportManager* pManager){
	freeAirportArr(pManager);
}


void	freeAirportArr(AirportManager* pManager){
	//bulid in func in genlist
}
