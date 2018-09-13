#ifndef PARSE_H
#define PARSE_H

#include "error.h"
#include "freejson.h"

char* ReadJson(char* filename, ERRORS* errorcode); //Reads the json from filename and makes a string

char* MakeRoot(char* str, int* cur, bool* rootIsArray, ERRORS* errorcode); //Makes a root json element from json without root
int ToTheNearestQuote(char* str, int cur, ERRORS* errorcode, bool* quoteOpened); //Skips all chars before quote
OBJECT_TYPE DefineObject(char* str, int* cur); //Defines next object type

char* GetKey(char* str, int* cur, ERRORS* errorcode); //Gets a key from the string
void* GetValue(char* str, int* cur, VALUE_TYPE* typeValue, ERRORS* errorcode); //Gets a value from the string. Returns char*, int*, double* or bool*
value* GetValueStruc(char* str, int* cur, ERRORS* errorcode); //Runs GetValue, makes a struct and returns it

jsonElementOrArr* ParseElementOrArr(char* str, int* cur, OBJECT_TYPE prevObjType, ERRORS* errorcode); //Main function that does all parsing
jsonElementOrArr* JsonParse(char* str, int cur, bool* rootIsArray, ERRORS* errorcode);
jsonElementOrArr* ParseFile(char *filename); //Runs the reader from filename, runs the parser, returns the head of the tree

#endif // RUN_H
