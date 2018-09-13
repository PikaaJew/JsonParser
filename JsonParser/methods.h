#ifndef METHODS_H
#define METHODS_H

#endif // METHODS_H

#include "parse.h"

void PrintObject(jsonObjectOrArrValue* object, int numOfTabs); //Prints all object's content
void PrintJson(jsonElementOrArr* elementOrArr, int numOfTabs); //Prints all json element or array's content

jsonObjectOrArrValue* GetFirstObject(jsonElementOrArr* elementOrArr); //Returns the first object of json element or array
jsonObjectOrArrValue* GetNextObject(jsonObjectOrArrValue* object); //Gets the next object from current
jsonObjectOrArrValue* GetPrevObject(jsonObjectOrArrValue* object); //Gets the previous object from current
jsonObjectOrArrValue* FindObjectByKey(jsonElementOrArr* elementOrArr, char* key); //Finds object buy key and returns it

void* GetObjectValue(jsonObjectOrArrValue* object, OBJECT_TYPE obj_type, VALUE_TYPE val_type); //Returns the value of the object
                                                                        //Can be char*, int*, double*, bool* or jsonElementOrArr*
