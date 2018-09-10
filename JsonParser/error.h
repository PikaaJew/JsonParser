#ifndef ERROR_H
#define ERROR_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef enum {
    NO_ERR = 0,
    ERR_NOT_ENOUGH_MEMORY,
    ERR_CANT_OPEN_FILE,
    ERR_INVALID_JSON,
    ERR_INCORRECT_KEY,
    ERR_INCORRECT_VALUE,
    ERR_BR_MISTAKE,
    ERR_INCORRECT_SS,
    LAST
}  ERRORS;

void PrintError(ERRORS *errorcode, int cur); //Printing errors function

bool CheckJson(char* str, ERRORS* errorcode); //Checks json for correct use of parentheses

bool CheckBackslash(char* str, int cur); //Checks right using of backslash

bool CheckSpecialSymbols(char* str, int cur, ERRORS* errorcode); //Checks special symbols like \n

bool CheckValue(char* str, int* cur, char valueType, ERRORS* errorcode); // Checks correction of the value

void CheckKey(char* str, int* cur, ERRORS* errorcode); //Checks correction of the key

#endif // ERROR_H
