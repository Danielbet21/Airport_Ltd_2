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

int	addAirport(AirportManager* pManager) {
	// create new airport to add the list
	Airport* pPort = (Airport*)malloc(sizeof(Airport));
	if (!pPort) {
		return 0;
	}
	// initialize the airport
	if (!(initAirport(pPort, pManager))) {
		freeAirport(pPort);
		return 0;
	}
	// add- node to the Airportlist as required 
	// TODO: make sure the list is sorted by code
	NODE* dumi = &pManager->AirportList.head;
	dumi = L_insertAirportSorted(&pManager->AirportList, pPort, compareByCode);
		if (dumi == NULL) {
			free(dumi);
			freeAirport(pPort);
		}
	return 1;
}

int lengthList(NODE* head) {
	NODE* temp = head;
	int count = 0;
	while (temp != NULL) {
		count++;
		temp = temp->next;
	}
	return count;
}

int compareByCode(const void* port1, const void* port2) {
	if (!port1 || !port2)
		return 0;
	const Airport* pPort1 = (const Airport*)port1;
	const Airport* pPort2 = (const Airport*)port2;

	return strcmp(((Airport*)pPort1)->code, ((Airport*)pPort2)->code);
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
		Airport* pAir = malloc(sizeof(Airport));
		if (pAir == NULL) {
			return NULL;
		}
		strcpy(pAir->code, code); //TODO: free pPort and check if this works

		NODE* res = L_find(pManager->AirportList.head.next, pAir, compareByCode);
		if (res == NULL) {
			return NULL;
		}
		else {
			return (Airport*)res->key;
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

void freeManager(AirportManager* pManager){
	L_free(&pManager->AirportList, freeAirport);
}
