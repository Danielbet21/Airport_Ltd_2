#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "General.h"
#include "Date.h"
#include "Flight.h"

const int DAY_MONTHS[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
#define SPECIAL_TAV '#'

void getCorrectDate(Date* pDate)
{
	char date[MAX_STR_LEN];
	int ok = 1;

	do {
		printf("Enter Flight Date dd%c%cmm%c%cyyyy  minimum year %d\t",
			SPECIAL_TAV, SPECIAL_TAV, SPECIAL_TAV, SPECIAL_TAV, MIN_YEAR);
		myGets(date, MAX_STR_LEN);
		ok = checkDate(date, pDate);
		if (!ok)
			printf("Error try again\n");
	} while (!ok);
}


int	 checkDate(char* date, Date* pDate)
{
	int day, month, year;
	if (strlen(date) != 12)
		return 0;
	if ( (date[2] != SPECIAL_TAV) || (date[3] != SPECIAL_TAV)
		 || (date[6] != SPECIAL_TAV) || (date[7] != SPECIAL_TAV))
		return 0;
	sscanf(date, "%d%*c%*c%d%*c%*c%d", &day, &month, &year);
	if (day < 1 || month < 1 || month > 12 || year < MIN_YEAR)
		return 0;

	if (day > DAY_MONTHS[month - 1])
		return 0;

	pDate->day = day;
	pDate->month = month;
	pDate->year = year;

	return 1;
}

void printDate(const Date* pDate)
{
	printf("Date: %d/%d/%d", pDate->day, pDate->month, pDate->year);
}

int compareDates(const void* pDate1, const void* pDate2) {
	Date* dateA = (Date*)pDate1;
	Date* dateB = (Date*)pDate2;
	if (dateA->year != dateB->year)
		return dateA->year - dateB->year;
	if (dateA->month != dateB->month)
		return dateA->month - dateB->month;
	return dateA->day - dateB->day;
}

int saveDateToBinFile(FILE* file, const Date* pDate) {
	if (file == NULL || pDate == NULL) {
		return 0;
	}

	if (writeGeneralToBinFile(file, pDate, sizeof(Date)) != 1) {
		return 0;
	}
	return 1;
}
Date readDateFromBinFile(FILE* file) {
	Date date;
	date.day = 0;
	date.month = 0;
	date.year = 0;
	if (file == NULL) {
		return date;
	}
	if (readGeneralFromBinFile(file, &date, sizeof(Date)) != 1) {
		return date;
	}

	return date;
}