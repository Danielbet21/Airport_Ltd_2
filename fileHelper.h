#ifndef __FILE_HELPER__
#define __FILE_HELPER__

#include <stdio.h>
#include <crtdbg.h> // TODO: remove before release
#include "General.h"
#include <string.h>

#define MAX_BUFFER_SIZE 256

int writeStringToFile(FILE* file, const char* str);
int writeStringTobinFile(FILE* file, const char* str);
char* readStringFromTextFile(FILE* file);
char* readStringFromBinFile(FILE* file);
int writeGeneralToBinFile(FILE* file, void* fileType, size_t sizeOfElement);
int readGeneralFromBinFile(FILE* file, void* readValue, size_t sizeOfElement);




#endif 