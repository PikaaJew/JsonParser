#ifndef TREE_H
#define TREE_H

#endif // TREE_H

#include <stdlib.h>
#include <stdbool.h>

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
