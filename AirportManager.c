#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "AirportManager.h"
#include "General.h"

int initManager(AirportManager* pManager, const char* fileName)
{
	if (fileName && loadManagerFromFile(pManager, fileName)) {
		// success init from file
		return 1;
	}
	if (L_init(&pManager->AirportList)) {
		// success init list menually
		return 2;
	};
	return 0;
}

int	saveManagerToFile(const AirportManager* pManager, const char* fileName) {
	FILE* file = fopen(fileName, "w");
	int res;
	if (!file) {
		printf("Error open file to write\n");
		return 0;
	}
	int count = L_count(&pManager->AirportList);
	NODE* ptr = pManager->AirportList.head.next;
	fprintf(file, "%d\n", count);

	while (ptr) {
		Airport* pPort = (Airport*)ptr->key;
		if (pPort == NULL) {
			fclose(file);
			return 0;
		}
		writeAirportToFile(file, pPort);
		ptr = ptr->next;
	}
	fclose(file);
	return 1;
}

int loadManagerFromFile(AirportManager* pManager, const char* fileName) {
	FILE* file = fopen(fileName, "r");
	if (!file) {
		printf("Error open file to read\n");
		return 0;
	}
	//TODO check if count == 0 need to print 0 
	int count;
	fscanf(file, "%d", &count);
	if (count == 0) {
		fclose(file);
		return 0;
	}
	fgetc(file); // get to the next line
	// init the list
	if (!L_init(&pManager->AirportList)) {
		fclose(file);
		return 0;
	}
	// read the airports
	for (int i = 0; i < count; i++) {
		Airport* pPort = (Airport*)calloc(1, sizeof(Airport));
		if (!pPort) {
			return 0;
		}
		if (!readAirportFromFile(file, pPort)) {
			freeAirport(pPort);
			fclose(file);
			return 0;
		}

		NODE* ptr1 = L_insertAirportSorted(&pManager->AirportList, pPort, AirportCompareCode);
		if (!ptr1) {
			freeAirport(pPort);
			fclose(file);
			return 0;
		}
	}

	fclose(file);
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
		strcpy(pAir->code, code); //TODO: free pPort

		const NODE* res = L_find(pManager->AirportList.head.next, pAir, compareByCode);
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


int hasXorMoreAirports(const AirportManager* pManager, int x) {

	int count = 0;
	NODE* ptr = pManager->AirportList.head.next;
	if (!ptr)
		return 0;

	while (ptr && count < x) {
		count++;
		ptr = ptr->next;
	}

	if (count >= x) {
		return 1;
	}
	return 0;
}