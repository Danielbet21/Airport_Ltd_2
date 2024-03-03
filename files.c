#include <stdio.h>
#include "Airline.h"

void writeToTXTFile(char* fileName, Airline air) {
	FILE* file = fopen(fileName, "w");
	if (file == NULL) {
		printf("Error opening file\n");
		return;
	}
	fprintf(file, "%s\n", air.name);
	// planes
	for (int i = 0; i < air.planeCount; i++) {
		fprintf(file, "%d\n", air.planeArr[i].serialNum);
		fprintf(file, "%d\n", air.planeArr[i].type);
	}
	// flights
	for (int i = 0; i < air.flightCount; i++) {
		fprintf(file, "%s\n", air.flightArr[i]->sourceCode);
		fprintf(file, "%s\n", air.flightArr[i]->destCode);
		fprintf(file, "%d\n", air.flightArr[i]->flightPlane.serialNum);
		fprintf(file, "%d\n", air.flightArr[i]->date.year);
		fprintf(file, "%d\n", air.flightArr[i]->date.month);
		fprintf(file, "%d\n", air.flightArr[i]->date.day);
	}
	fclose(file);

}
//TODO: add read from file to the initAirline and initManager

void writeToBinFile(char* fileName, Airline air) {
	FILE* file = fopen(fileName, "w");
	if (file == NULL) {
		printf("Error opening file\n");
		return;
	}
	//TODO: write to file
	fclose(file);

}