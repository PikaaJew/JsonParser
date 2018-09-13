#ifndef FREEJSON_H
#define FREEJSON_H

#endif // FREEJSON_H

#include "tree.h"

value* freeValue(value* val); //Frees the value struct
jsonObjectOrArrValue* freeJsonObject(jsonObjectOrArrValue* jsonObj); //Frees the jsonObjectOrArrValue struct and all content
jsonElementOrArr* freeJsonElement(jsonElementOrArr* jsonEl); //Frees the json element or array and all content
