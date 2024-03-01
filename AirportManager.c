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
	//======================================================================================================
	//dont forget to free the memory
	if (!pPort){
		free(pPort);
		return 0;
	}
	// initialize the airport
	if (!(initAirport(pPort, pManager))) {
		freeAirport(pPort);
		return 0;
	}

	// add the new airport to the list
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
	NODE n = pManager->AirportList.head;
	int count = 0;
	while(n.next != NULL){
		count++;
		n = *n.next;
	}
	//TODO: use qsort and bsearch ---- check if it works
	Airport* pPort = NULL;
	Airport temp = {"","",code}; // initializing only the code segment
//	qsort(&pManager->AirportList,count,sizeof(Airport), compareByCode);
	pPort = (Airport*)bsearch(&temp, pManager->AirportList.head.next, count, sizeof(Airport), compareByCode);
	return NULL;
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
