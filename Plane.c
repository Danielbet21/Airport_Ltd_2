#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "General.h"
#include "Plane.h"
#include "fileHelper.h"



void	initPlane(Plane* pPlane, Plane* planeArr, int planeCount)
{
	pPlane->serialNum = getPlaneSN(planeArr, planeCount);
	pPlane->type = getPlaneType();
}

ePlaneType getPlaneType()
{
	int option;
	printf("\n\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofPlaneTypes; i++)
			printf("%d for %s\n", i, PlaneTypeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofPlaneTypes);
	getchar();
	return (ePlaneType) option;
}

const char*		GetPlaneTypeStr(int type)
{
	if (type < 0 || type >= eNofPlaneTypes)
		return NULL;
	return PlaneTypeStr[type];
}

int getPlaneSN(Plane* planeArr, int planeCount)
{
	int num;
	int unique = 0;
	do {
		printf("Enter plane serial number - between %d to %d\n",MIN_SN,MAX_SN);
		scanf("%d", &num);
		if (num >= MIN_SN && num <= MAX_SN)
			unique = isSerialNumUnique(planeArr, planeCount, num);
	} while (!unique);
	return num;
}

int		isSerialNumUnique(Plane* planeArr, int planeCount, int num)
{
	for (int i = 0; i < planeCount; i++)
	{
		if (planeArr[i].serialNum == num)
			return 0;
	}
	return 1;
}

Plane* findPlaneBySN(Plane* planeArr, int count, int sn)
{
	for (int i = 0; i < count; i++)
		if (planeArr[i].serialNum == sn)
			return &planeArr[i];
	return NULL;
}

void	printPlane(const Plane* pPlane)
{
	printf("Plane: serial number:%d, type %s\n",pPlane->serialNum, PlaneTypeStr[pPlane->type]);

}

void			freePlane(Plane* pPlane) 
{ 
	//nothing to free
}

int savePlaneArrToBinFile(FILE* file, const Plane* planeArr, int planeCount) {
	// save the number of planes
	if (!writeGeneralToBinFile(file, &planeCount, sizeof(int))) {
		return 0;
	}
	// save the planes
	for (int i = 0; i < planeCount; i++) {
		if (!savePlaneToBinFile(file, &planeArr[i])) {
			return 0;
		}
	}
	return 1;
}

int savePlaneToBinFile(FILE* file, const Plane* pPlane) {
	if (!writeGeneralToBinFile(file, pPlane, sizeof(Plane))) {
		return 0;
	}

	return 1;
}

Plane readPlaneFromBinFile(FILE* file) {
	Plane restoredPlane;
	if (!readGeneralFromBinFile(file, &restoredPlane, sizeof(Plane))) {
		restoredPlane.serialNum = -1;
	}
	return restoredPlane;
}


Plane* readPlaneArrFromBinFile(FILE* file, int* restoredCount) {
	if (file == NULL || restoredCount == NULL) {
		return NULL;
	}
	int planeCount;
	if (!readGeneralFromBinFile(file, &planeCount, sizeof(int))) {
		return NULL;
	}
	Plane* planeArr = (Plane*)malloc(planeCount * sizeof(Plane));
	if (!planeArr) {
		return NULL;
	}
	for (int i = 0; i < planeCount; i++) {
		planeArr[i] = readPlaneFromBinFile(file);
		if (planeArr[i].serialNum == -1) {
			free(planeArr);
			return NULL;
		}
	}
	*restoredCount = planeCount;
	return planeArr;
}


int savePlaneSerialNumberBinFile(FILE* file, const Plane* pPlane) {
	if (file == NULL || pPlane == NULL) {
		return 0;
	}
	if (!writeGeneralToBinFile(file, &pPlane->serialNum, sizeof(int))) {
		return 0;
	}

	return 1;
}

Plane readPlaneSerialNumberBinFile(FILE* file, const Plane* planeArr, int planeCount) {
	Plane restoredPlane;
	restoredPlane.serialNum = -1;
	restoredPlane.type = 0;
	if (file == NULL || planeArr == NULL || planeCount <= 0) {
		return restoredPlane;
	}
	int tempSerialNum;
	if (!readGeneralFromBinFile(file, &tempSerialNum, sizeof(int))) {
		return restoredPlane;
	}

	for (int i = 0; i < planeCount; i++)
	{
		if (tempSerialNum == planeArr[i].serialNum) {
			restoredPlane = planeArr[i];
			return restoredPlane;
		}
	}
	return restoredPlane;
}
