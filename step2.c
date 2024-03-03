#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "Airline.h"
#include "AirportManager.h"
#include "step2.h"

Flight* flightArr[3];
Airline airline;
AirportManager manager;

void initsializeAirline() {   
    // Initialize airline, manager
    initAirline(&airline);
    //initManager(&manager);
    // Allocate memory for airline.flightArr
    airline.flightArr = (Flight**)malloc(3 * sizeof(Flight*));
    if (!(airline.flightArr)) {
        return;
    }
    // Add some flights to the airline
    for (int i = 0; i < 3; i++) {
        flightArr[i] = (Flight*)malloc(sizeof(Flight));
        if (!flightArr[i]) {
            return;
        }
        airline.flightArr[i] = flightArr[i];
    }

    strcpy(airline.flightArr[0]->destCode, "TLV");
    strcpy(airline.flightArr[0]->sourceCode, "JFK");
    airline.flightArr[0]->date.day = 01;
    airline.flightArr[0]->date.month = 01;
    airline.flightArr[0]->date.year = 2023;

    strcpy(airline.flightArr[1]->destCode, "TLV");
    strcpy(airline.flightArr[1]->sourceCode, "WWW");
    airline.flightArr[1]->date.day = 02;
    airline.flightArr[1]->date.month = 02;
    airline.flightArr[1]->date.year = 2024;

    strcpy(airline.flightArr[2]->destCode, "URL");
    strcpy(airline.flightArr[2]->sourceCode, "URI");
    airline.flightArr[2]->date.day =03;
    airline.flightArr[2]->date.month = 03;
    airline.flightArr[2]->date.year = 2025;

    airline.flightCount = 3;
}

void cleanAirline() {
    freeCompany(&airline);
}

void test_sortBySourceCode() {
printf("---------sortBySourceCode test---------\n");
    initsializeAirline();

    sortBySourceCode(&airline);

    // Check if the flights are sorted by source code
    for (int i = 0; i < airline.flightCount - 1; i++) {
        assert(strcmp(airline.flightArr[i]->sourceCode, airline.flightArr[i + 1]->sourceCode) <= 0);
    }

    cleanAirline();
}

void test_sortByDestCode() {
printf("---------sortByDestCode test---------\n");
    initsializeAirline();

    sortByDestCode(&airline);

    // Check if the flights are sorted by destination code
    for (int i = 0; i < airline.flightCount - 1; i++) {
        assert(strcmp(airline.flightArr[i]->destCode, airline.flightArr[i + 1]->destCode) <= 0);
    }

    cleanAirline();
}

void test_sortByDate() {
    printf("---------sortByDate test---------\n");

    initsializeAirline();

    sortByDate(&airline);

    // Check if the flights are sorted by date
    for (int i = 0; i < airline.flightCount - 1; i++) {
        printDate(&airline.flightArr[i]->date);
        printf("\n");
        printDate(&airline.flightArr[i + 1]->date);
        printf("\n");
        assert(compareDates(&airline.flightArr[i]->date, &airline.flightArr[i + 1]->date) <= 0);
    }
    cleanAirline();
}
void test_compareByDate() {
    printf("---------compareByDate test---------\n");
    Date date1 = { 01, 01, 2022 };
    Date date2 = { 01, 01, 2023 };
    Date date3 = { 01, 02, 2022 };
    Date date4 = { 02, 01, 2022 };
    Date date5 = { 01, 01, 2022 };

    assert(compareDates(&date1, &date2) < 0);
    assert(compareDates(&date1, &date3) < 0);
    assert(compareDates(&date1, &date4) < 0);
    assert(compareDates(&date1, &date5) == 0);
    assert(compareDates(&date2, &date1) > 0);
}

int menu2() {
    //test_sortBySourceCode();
    //test_sortByDestCode();
    //test_compareByDate();
    test_sortByDate();
    printf("All tests passed!\n");
    return 0;
}
