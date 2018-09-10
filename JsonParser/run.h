#ifndef RUN_H
#define RUN_H

#include "error.h"

typedef enum {
    UNDEF_VALUE = 0,
    STRING_T,
    NULL_T,
    BOOL_T,
    INTEGER_T,
    DOUBLE_T
} VALUE_TYPE;

typedef enum {
    UNDEF_OBJ = 0,
    SIMPLE_ELEM,
    JSON_ELEM,
    JSON_ARRAY
}  OBJECT_TYPE;


typedef struct jsonElementOrArr{
    bool isEmpty;
    int size;
    struct jsonObjectOrArrValue* objectOrArrFirstEl;
} jsonElementOrArr;

typedef struct jsonObjectOrArrValue {
    struct jsonObjectOrArrValue* next;
    struct jsonObjectOrArrValue* prev;
    char* key;
    OBJECT_TYPE obj_type;
    union {
        struct value* smplValue;
        struct jsonElementOrArr* jsonChildOrArr;
    };
} jsonObjectOrArrValue;

typedef struct value {
    VALUE_TYPE typeValue;
    void* singleValue;
} value;



char* ReadJson(char* filename, ERRORS* errorcode); //Reads the json from filename and makes a string

jsonElementOrArr* ParseFile(char *filename); //Runs the reader from filename, runs the parser, returns the head of the tree

value* freeValue(value* val); //Frees the value struct

jsonObjectOrArrValue* freeJsonObject(jsonObjectOrArrValue* jsonObj); //Frees the jsonObjectOrArrValue struct and all content

jsonElementOrArr* freeJsonElement(jsonElementOrArr* jsonEl); //Frees the json element or array and all content

char* MakeRoot(char* str, int* cur, bool* rootIsArray, ERRORS* errorcode); //Makes a root json element from json without root

int ToTheNearestQuote(char* str, int cur, ERRORS* errorcode, bool* quoteOpened); //Skips all chars before quote

char* GetKey(char* str, int* cur, ERRORS* errorcode); //Gets a key from the string

void* GetValue(char* str, int* cur, VALUE_TYPE* typeValue, ERRORS* errorcode); //Gets a value from the string. Returns char*, int*, double* or bool*

value* GetValueStruc(char* str, int* cur, ERRORS* errorcode); //Runs GetValue, makes a struct and returns it

OBJECT_TYPE DefineObject(char* str, int* cur); //Defines next object type

jsonElementOrArr* ParseElementOrArr(char* str, int* cur, OBJECT_TYPE prevObjType, ERRORS* errorcode); //Main function that does all parsing

jsonElementOrArr* JsonParse(char* str, int cur, bool* rootIsArray, ERRORS* errorcode);

#endif // RUN_H
