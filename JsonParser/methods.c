#include "methods.h"

void PrintTabs(int numOfTabs) {
    for (int i = 0; i < numOfTabs; i++) {
        printf("\t");
    }
}

void PrintObject(jsonObjectOrArrValue* object, int numOfTabs) {
    if (object != NULL) {
        PrintTabs(numOfTabs);
        if (object->key != NULL) {            
            printf("%s : ", object->key);
        }
        if (object->obj_type == SIMPLE_ELEM) {            
            if (object->smplValue->typeValue == STRING_T) {
                printf("%s", (char*)object->smplValue->singleValue);
            }
            if (object->smplValue->typeValue == INTEGER_T) {
                printf("%d", *((int*)object->smplValue->singleValue));
            }
            if (object->smplValue->typeValue == DOUBLE_T) {
                printf("%g", *((double*)object->smplValue->singleValue));
            }
            if (object->smplValue->typeValue == NULL_T) {
                printf("%s", (char*)object->smplValue->singleValue);
            }
            if (object->smplValue->typeValue == BOOL_T) {
                printf("%d", *((bool*)object->smplValue->singleValue));
            }
            if (object->next != NULL) {
                printf(",");
            }
            printf("\n");
        } else if (object->obj_type == JSON_ELEM) {
            printf("\n");
            PrintTabs(numOfTabs + 1);
            printf("{\n");
            PrintJson(object->jsonChildOrArr, numOfTabs + 1);
            PrintTabs(numOfTabs + 1);
            if (object->next != NULL) {
                printf("},\n");
            } else {
                printf("}\n");
            }
        } else if (object->obj_type == JSON_ARRAY) {
            printf("\n");
            PrintTabs(numOfTabs + 1);
            printf("[\n");
            PrintJson(object->jsonChildOrArr, numOfTabs + 1);
            PrintTabs(numOfTabs + 1);
            if (object->next != NULL) {
                printf("],\n");
            } else {
                printf("]\n");
            }
        }
    } else {
        fprintf(stderr,"Current object is NULL\n");
    }
}

void PrintJson(jsonElementOrArr* elementOrArr, int numOfTabs) {
    if (elementOrArr != NULL) {
        if (!(elementOrArr->isEmpty)) {
            jsonObjectOrArrValue* thisObject = elementOrArr->objectOrArrFirstEl;
            for (int i = 0; i < elementOrArr->size; i++) {
                PrintObject(thisObject, numOfTabs);
                thisObject = thisObject->next;
            }
        }
    } else {
        fprintf(stderr, "Current element or Array is NULL\n");
    }
}

jsonObjectOrArrValue* GetFirstObject(jsonElementOrArr* elementOrArr) {
    if (elementOrArr == NULL) {
        fprintf(stderr,"Current element or array is NULL\n");
        return NULL;
    }
    if (!elementOrArr->isEmpty) {
        return elementOrArr->objectOrArrFirstEl;
    } else {
        fprintf(stderr,"Json element or array is empty\n");
    }
    return NULL;
}

jsonObjectOrArrValue* GetNextObject(jsonObjectOrArrValue* object) {
    if (object != NULL) {
        if (object->next != NULL) {
            return object->next;
        } else {
            fprintf(stderr,"Next is NULL\n");
        }
        return object;
    }
    fprintf(stderr,"Current object is NULL\n");
    return NULL;
}

jsonObjectOrArrValue* GetPrevObject(jsonObjectOrArrValue* object) {
    if (object != NULL) {
        if (object->prev != NULL) {
            return object->prev;
        } else {
            fprintf(stderr,"Prev is NULL\n");
        }
        return object;
    }
    fprintf(stderr,"Current object is NULL\n");
    return NULL;
}

jsonObjectOrArrValue* FindObjectByKey(jsonElementOrArr* elementOrArr, char* key) {
    if (elementOrArr != NULL) {
        if (elementOrArr->isEmpty) {
            fprintf(stderr,"Element or Array is empty\n");
            return NULL;
        }
        jsonObjectOrArrValue* thisObject = elementOrArr->objectOrArrFirstEl;
        if (thisObject->key != NULL) {
        	for (int i = 0; i < elementOrArr->size; i++) {
            	if (!strcmp(key, thisObject->key)) {
                	return thisObject;
            	} else {
                	thisObject = thisObject->next;
            	}
        	}
    	} else {
    		fprintf(stderr, "There are no keys in json array\n");
    		return NULL;
    	}
        fprintf(stderr,"Can not find the object by key %s\n", key);
        return NULL;
    }
    fprintf(stderr,"Current element or array is NULL\n");
    return NULL;
}

void* GetObjectValue(jsonObjectOrArrValue* object, OBJECT_TYPE obj_type, VALUE_TYPE val_type) {
    if (object == NULL) {
        fprintf(stderr,"Current object is NULL\n");
        return NULL;
    }
    if (obj_type == JSON_ARRAY || obj_type == JSON_ELEM) {
        if (object->obj_type == obj_type) {
            return (jsonElementOrArr*)object->jsonChildOrArr;
        } else {
            fprintf(stderr,"Incorrect object type\n");
            return NULL;
        }
    } else if (obj_type == SIMPLE_ELEM) {
        if (object->obj_type == obj_type) {
            if (val_type == STRING_T) {
                if (object->smplValue->typeValue == val_type) {
                    return (char*)object->smplValue->singleValue;
                } else {
                    fprintf(stderr,"Incorrect value type\n");
                    return NULL;
                }
            } else if (val_type == INTEGER_T) {
                if (object->smplValue->typeValue == val_type) {
                    return (int*)object->smplValue->singleValue;
                } else {
                    fprintf(stderr,"Incorrect value type\n");
                    return NULL;
                }
            } else if (val_type == DOUBLE_T) {
                if (object->smplValue->typeValue == val_type) {
                    return (double*)object->smplValue->singleValue;
                } else {
                    fprintf(stderr,"Incorrect value type\n");
                    return NULL;
                }
            } else if (val_type == NULL_T) {
                if (object->smplValue->typeValue == val_type) {
                    return (char*)object->smplValue->singleValue;
                } else {
                    fprintf(stderr,"Incorrect value type\n");
                    return NULL;
                }
            } else if (val_type == BOOL_T) {
                if (object->smplValue->typeValue == val_type) {
                    return (bool*)object->smplValue->singleValue;
                } else {
                    fprintf(stderr,"Incorrect value type\n");
                    return NULL;
                }
            } else {
                fprintf(stderr,"Undefined value type\n");
                return NULL;
            }
        } else {
            fprintf(stderr,"Incorrect object type\n");
            return NULL;
        }
    }
    return NULL;
}
