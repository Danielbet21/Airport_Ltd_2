#include <assert.h>
#include <stdio.h>
#include "AirportManager.h"

void test_initManager() {
    printf("test_initManager\n");;
    AirportManager manager;
    assert(initManager(&manager) == 1);
}

void test_addAirport() {
    AirportManager manager;
    initManager(&manager);
    assert(addAirport(&manager) == 1);
}

void test_initAirport() {
    AirportManager manager;
    initManager(&manager);
    Airport airport;
    assert(initAirport(&airport, &manager) == 1);
}

void test_findAirportByCode1() {
    printf("test 1\n");
    AirportManager manager;
    initManager(&manager);
    addAirport(&manager);
    // Assuming the code of the added airport is "AAA"
    assert(findAirportByCode(&manager, "AAA") != NULL);
    assert(findAirportByCode(&manager, "ABC") == NULL);
}

void test_checkUniqeCode() {
    AirportManager manager;
    initManager(&manager);
    addAirport(&manager);
    // Assuming the code of the added airport is "XYZ"
    assert(checkUniqeCode("XYZ", &manager) == 0);
    assert(checkUniqeCode("ABC", &manager) == 1);
}

void test_freeManager() {
    AirportManager manager;
    initManager(&manager);
    addAirport(&manager);
    freeManager(&manager);
	assert(manager.AirportList.head.next == NULL);
}

int main() {
    //test_initManager();
    test_addAirport();
    //test_initAirport();
    //test_findAirportByCode1();
    //test_checkUniqeCode();
    //test_freeManager();

    printf("All tests passed!\n");
    return 0;
}
