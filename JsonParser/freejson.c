#include "freejson.h"

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
