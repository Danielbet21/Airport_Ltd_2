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

int	addAirport(AirportManager* pManager) {
	// create new airport to add the list
	Airport* pPort = (Airport*)malloc(sizeof(Airport));
	if (!pPort) {
		free(pPort);
		return 0;
	}
	// initialize the airport
	if (!(initAirport(pPort, pManager))) {
		freeAirport(pPort);
		return 0;
	}
	// add- node to the Airportlist as required
	insertAirportToList(&pManager->AirportList.head, pPort);
	return 1;
}

void insertAirportToList(AirportManager* pManager, Airport* pPort) {
	NODE* pMan = &pManager->AirportList.head.next;
	while (pMan->next != NULL) {
		Airport* pAirport = (Airport*)pMan->next->key;
		int res = strcmp(pAirport->code, pPort->code);
		//res cant be 0 because the code is unique
		if (res < 0) { // the code need to be before the next node
			L_insert(pMan, pPort);
		}
		else if (res > 0) {
			L_insert(pMan->next, pPort);
		}
		pMan = pMan->next;
	}
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
			return (Airport*)res;
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
