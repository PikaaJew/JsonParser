#include "run.h"

char* ReadJson(char* filename, ERRORS* errorcode) {
    FILE *f;
    char *str;
    char *memorytest;
    int i = 0;
    char c;

    f = fopen(filename, "r");
    if (NULL == f) {
        *errorcode = ERR_CANT_OPEN_FILE;
        return NULL;
    }
    str = malloc(sizeof(char) * 100);
    if (str == NULL) {
        free(str);
        *errorcode = ERR_NOT_ENOUGH_MEMORY;
        return NULL;
    }

    while ((c = (char)fgetc(f)) != EOF) { //We can't use fgets because we don't know the maximum size of the string
        str[i] = c;
        if (i % 100 == 99) {
            memorytest = realloc(str, sizeof(char) * ((unsigned long)i + 102));
            if (memorytest == NULL) {
                free(str);
                *errorcode = ERR_NOT_ENOUGH_MEMORY;
                fclose(f);
                return NULL;
            }
            str = memorytest;
        }
        i++;
    }
    fclose(f);
    str[i] = '\0';
    return(str);
}

jsonElementOrArr* ParseFile(char *filename) {
    char* str = NULL;
    ERRORS errorcode = NO_ERR;
    jsonElementOrArr* head = NULL;

    str = ReadJson(filename, &errorcode);
    if (errorcode != NO_ERR) {
        PrintError(&errorcode, 0);
        return NULL;
    }

    bool* rootIsArray = malloc(sizeof(bool));
    int cur = 0;
    str = MakeRoot(str, &cur, rootIsArray, &errorcode);

    head = JsonParse(str, cur, rootIsArray, &errorcode);

    free(str);
    free(rootIsArray);
    return head;
}

value* freeValue(value* val) {
    free(val->singleValue);
    free(val);
    val = NULL;
    return val;
}

jsonObjectOrArrValue* freeJsonObject(jsonObjectOrArrValue* jsonObj) {
    if (jsonObj == NULL) {
        return NULL;
    }
    while (jsonObj->next != NULL) {
        jsonObj = jsonObj->next;
    }
    while (jsonObj->prev != NULL) {
        if (jsonObj->key != NULL) {
            free(jsonObj->key);
        }
        if (jsonObj->obj_type != UNDEF_OBJ) {
            if (jsonObj->obj_type == SIMPLE_ELEM && jsonObj->smplValue != NULL) {
                jsonObj->smplValue = freeValue(jsonObj->smplValue);
                jsonObj->smplValue = NULL;
            }
            else if ((jsonObj->obj_type == JSON_ELEM || jsonObj->obj_type == JSON_ARRAY) && jsonObj->jsonChildOrArr != NULL) {
                jsonObj->jsonChildOrArr = freeJsonElement(jsonObj->jsonChildOrArr);
                jsonObj->jsonChildOrArr = NULL;
            }
        }
        jsonObjectOrArrValue *tmp = jsonObj;
        jsonObj = jsonObj->prev;
        tmp->prev = NULL;
        jsonObj->next = NULL;
        free(tmp);
        tmp = NULL;
    }

    if (jsonObj->key != NULL) {
        free(jsonObj->key);
    }
    if (jsonObj->obj_type != UNDEF_OBJ) {
        if (jsonObj->obj_type == SIMPLE_ELEM && jsonObj->smplValue != NULL) {
            jsonObj->smplValue = freeValue(jsonObj->smplValue);
            jsonObj->smplValue = NULL;
        }
        else if ((jsonObj->obj_type == JSON_ELEM || jsonObj->obj_type == JSON_ARRAY) && jsonObj->jsonChildOrArr != NULL) {
            jsonObj->jsonChildOrArr = freeJsonElement(jsonObj->jsonChildOrArr);
            jsonObj->jsonChildOrArr = NULL;
        }
    }

    free(jsonObj);
    jsonObj = NULL;
    return jsonObj;
}

jsonElementOrArr* freeJsonElement(jsonElementOrArr* jsonEl) {
    if (jsonEl != NULL) {
        if (jsonEl->objectOrArrFirstEl != NULL) {
            jsonEl->objectOrArrFirstEl = freeJsonObject(jsonEl->objectOrArrFirstEl);
        }
        free(jsonEl);
        jsonEl = NULL;
        return jsonEl;
    }
    return NULL;
}

int GetSize(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

char* MakeRoot(char* str, int* cur, bool* rootIsArray, ERRORS* errorcode) {
    int i = 0, j;
    int quoteOpened = -1;
    *rootIsArray = false;
    while (str[i] != '\0' && isspace(str[i])) {
        i++;
    }    
    if (str[i] == '\0') {
        *errorcode = ERR_INVALID_JSON;
        return str;
    }

    j = i;
    while (str[j] != '\0') {
        if (str[j] == '"') {
            quoteOpened = - quoteOpened;
        }
        if (str[j] == '[' && quoteOpened == -1) {
            *rootIsArray = true;
            *cur = j + 1;
            return str;
        }

        if (str[j] == '{' && quoteOpened == -1) {
            *cur = j;
            return str;
        }
        j++;
    }

    str = strcat(str, "}");
    int size = GetSize(str);
    char* tmp = malloc(sizeof(char) * (size + 10));
    tmp[0] = '{';
    tmp[1] = '"';
    tmp[2] = 'r';
    tmp[3] = 'o';
    tmp[4] = 'o';
    tmp[5] = 't';
    tmp[6] = '"';
    tmp[7] = ':';
    tmp[8] = '\0';
    tmp = strcat(tmp, str);
    free(str);
    return tmp;
}

int ToTheNearestQuote(char* str, int cur, ERRORS* errorcode, bool* quoteOpened) {
    if (str[cur] != '"') {
        cur++;
    }
    while (str[cur] != '"') {
        if (!isspace(str[cur]) && str[cur] != '"' && str[cur] != ',') {
            *errorcode = ERR_INCORRECT_KEY;
            return cur;
        }
        cur++;
    }
    *quoteOpened = true;
    return cur;
}

bool CheckEmpty(char* str, int* cur) {
    bool isEmpty = false;
    int i = *cur;
    if (str[i] == '{') {
        i++;
        while (isspace(str[i])) {
            i++;
        }
        if (str[i] == '}') {
            isEmpty = true;
            i++;
        }
        while (isspace(str[i])) {
            i++;
        }
    } else if (i > 0 && str[i - 1] == '[') {
        while (isspace(str[i])) {
            i++;
        }
        if (str[i] == ']') {
            isEmpty = true;
            i++;
        }
        while (isspace(str[i])) {
            i++;
        }
    }
    if (isEmpty) {
        *cur = i;
    }
    return isEmpty;
}

OBJECT_TYPE DefineObject(char* str, int* cur) {
    OBJECT_TYPE obj_type = UNDEF_OBJ;
    int i = *cur;
    while (isspace(str[i])) {
        i++;
    }
    if (str[i] == '{') {
        obj_type = JSON_ELEM;
        (*cur) = i;
        return obj_type;
    }
    if (str[i] == '[') {
        obj_type = JSON_ARRAY;
        i++;
        (*cur) = i;
        return obj_type;
    }
    obj_type = SIMPLE_ELEM;
    return obj_type;
}

char* GetKey(char* str, int* cur, ERRORS* errorcode) {
    bool quoteOpened = false;
    char* buffer = malloc(sizeof(char) * 101);
    char* memtest;
    int i = 0;
    *cur = ToTheNearestQuote(str, *cur, errorcode, &quoteOpened);
    if (*errorcode != NO_ERR) {
        free(buffer);
        return NULL;
    }
    while (quoteOpened == true) { //After while we are on closing quote
        (*cur)++;

        if (!CheckSpecialSymbols(str, *cur, errorcode)) {
            free(buffer);
            return NULL;
        }
        if (str[*cur] == '"' && str[*cur - 1] != '\\') {
            buffer[i] = '\0';
            i = 0;
            quoteOpened = false;
        } else {
            if (i % 100 == 99) {
                memtest = realloc(buffer, sizeof(char) * ((unsigned long)i + 101));
                if (memtest == NULL) {
                    *errorcode = ERR_NOT_ENOUGH_MEMORY;
                    free(buffer);
                    return NULL;
                }
                buffer = memtest;
            }
            buffer[i] = str[*cur];
            i++;
        }
    }
    CheckKey(str, cur, errorcode);
    return buffer;
}

void* GetValue(char* str, int* cur, VALUE_TYPE* typeValue, ERRORS* errorcode) {
    int i = 0, numberBeg, valueEnd;
    double* numValue;
    char* buffer, *memtest;
    bool isDouble = false, *bool_value;

    while (isspace(str[*cur])) {
        (*cur)++;
    }
    if (str[*cur] == '"') {
        buffer = malloc(sizeof(char) * 101);
        //bool gotValue = false;
        (*cur)++;
        while (!(str[*cur] == '"' && str[*cur - 1] != '\\')) { //After while we are on closing quote

            if (!CheckSpecialSymbols(str, *cur, errorcode)) {
                free(buffer);
                return NULL;
            }
            buffer[i] = str[*cur];
            i++;
            if (i % 100 == 99) {
                memtest = realloc(buffer, sizeof(char) * ((unsigned long)i + 101));
                if (memtest == NULL) {
                    *errorcode = ERR_NOT_ENOUGH_MEMORY;
                    free(buffer);
                    return NULL;
                }
                buffer = memtest;
            }
            (*cur)++;
        }
        buffer[i] = '\0';
        CheckValue(str, cur, 's', errorcode); //After CheckValue we are on coma or closing bracer
        (*typeValue) = STRING_T;
        return buffer;
    } else {
        if (str[*cur] == 'n') {
            CheckValue(str, cur, 'n', errorcode);
            (*typeValue) = NULL_T;
            return NULL;
        }
        if (str[*cur] == 't') {
            CheckValue(str, cur, 't', errorcode);
            (*typeValue) = BOOL_T;
            bool_value = malloc(sizeof(bool));
            *bool_value = true;
            return bool_value;
        }
        if (str[*cur] == 'f') {
            CheckValue(str, cur, 'f', errorcode);
            (*typeValue) = BOOL_T;
            bool_value = malloc(sizeof(bool));
            *bool_value = false;
            return bool_value;
        }
        if (isdigit(str[*cur]) || str[*cur] == '.' || str[*cur] == '-') {
            numValue = malloc(sizeof(double));
            numberBeg = (*cur);
            isDouble = CheckValue(str, cur, 'd', errorcode);
            valueEnd = *cur;

            if (*errorcode != NO_ERR) {
                return NULL;
            }
            (*cur) = numberBeg;

            buffer = malloc(sizeof(char) * 50);
            while (isdigit(str[*cur]) || str[*cur] == '.' || str[*cur] == 'e' || str[*cur] == 'E' || str[*cur] == '-' || str[*cur] == '+') {
                buffer[i] = str[*cur];
                i++;
                (*cur)++;
            }
            *cur = valueEnd;
            if (isDouble) {
                sscanf(buffer, "%lf", numValue);
                free(buffer);
                (*typeValue) = DOUBLE_T;
                return numValue;
            } else {
                sscanf(buffer, "%d", (int *)numValue);
                free(buffer);
                (*typeValue) = INTEGER_T;
                return numValue;
            }
        }
    }
    *errorcode = ERR_INCORRECT_VALUE;
    return NULL;
}

value* GetValueStruc(char* str, int* cur, ERRORS* errorcode) {
    value* head = malloc(sizeof(value));

    head->singleValue = GetValue(str, cur, &(head->typeValue), errorcode);
    if (*errorcode != NO_ERR) {
        head = freeValue(head);
        return NULL;
    }
    return head;
}

jsonElementOrArr* ParseElementOrArr(char* str, int* cur, OBJECT_TYPE prevObjType, ERRORS* errorcode) { //starting with opening braces

    jsonElementOrArr* newElement = malloc(sizeof(jsonElementOrArr));
    newElement->objectOrArrFirstEl = NULL;
    newElement->size = 0;

    if (CheckEmpty(str, cur)) {
        newElement->isEmpty = true;
        return newElement;
    }
    newElement->isEmpty = false;

    jsonObjectOrArrValue* headObject = NULL;
    jsonObjectOrArrValue* thisObject = NULL;
    bool isParsed = false;

    while (!isParsed) {
        jsonObjectOrArrValue* newObject = malloc(sizeof(jsonObjectOrArrValue));
        newObject->next = NULL;
        newObject->prev = NULL;
        newObject->obj_type = UNDEF_OBJ;
        newObject->key = NULL;

        if (prevObjType == JSON_ELEM) {
            newObject->key = GetKey(str, cur, errorcode); //after GetKey we are on element after colon
            if (*errorcode != NO_ERR) {
                newElement = freeJsonElement(newElement);
                if (headObject != NULL) {
                    headObject = freeJsonObject(headObject);
                }
                newObject = freeJsonObject(newObject);
                return NULL;
            }
        }


        newObject->obj_type = DefineObject(str, cur);

        if (newObject->obj_type == SIMPLE_ELEM) {
            newObject->smplValue = GetValueStruc(str, cur, errorcode);
        }
        else if (newObject->obj_type == JSON_ELEM) {
            newObject->jsonChildOrArr = ParseElementOrArr(str, cur, JSON_ELEM, errorcode);            
        }
        else if (newObject->obj_type == JSON_ARRAY) {
            newObject->jsonChildOrArr = ParseElementOrArr(str, cur, JSON_ARRAY, errorcode);            
        }
        if (*errorcode != NO_ERR) {
            newElement = freeJsonElement(newElement);
            if (headObject != NULL) {
                headObject = freeJsonObject(headObject);
            }
            newObject = freeJsonObject(newObject);
            return NULL;
        }

        if (headObject == NULL) {
            headObject = newObject;
            headObject->next = NULL;
            headObject->prev = NULL;
        } else {
            if (thisObject == NULL) {
                thisObject = newObject;
                headObject->next = thisObject;
                thisObject->prev = headObject;
                thisObject->next = NULL;
            } else {
                thisObject->next = newObject;
                newObject->prev = thisObject;
                thisObject = thisObject->next;
                thisObject->next = NULL;
            }
        }

        if ((prevObjType == JSON_ELEM && str[*cur] == '}') || (prevObjType == JSON_ARRAY && str[*cur] == ']')) {
            (*cur)++;
            isParsed = true;
            while (str[*cur] != '\0' && isspace(str[*cur])) {
                (*cur)++;
            }
        } else if (str[*cur] == ',') {
            (*cur)++;
        }

        newElement->size++;
    }

    newElement->objectOrArrFirstEl = headObject;

    return newElement;
}

jsonElementOrArr* JsonParse(char* str, int cur, bool* rootIsArray, ERRORS* errorcode) {


    CheckJson(str, errorcode);
    if (*errorcode != NO_ERR) {
        PrintError(errorcode, cur);
        return NULL;
    }

    jsonElementOrArr* head = NULL;
    if (*rootIsArray) {
        head = ParseElementOrArr(str, &cur, JSON_ARRAY, errorcode);
    } else {
        head = ParseElementOrArr(str, &cur, JSON_ELEM, errorcode);
    }
    if (*errorcode != NO_ERR) {
        PrintError(errorcode, cur);
        freeJsonElement(head);
        return NULL;
    }
    return head;
}
