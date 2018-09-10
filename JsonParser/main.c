#include "methods.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
        fprintf(stderr, "invalid number of arguments \n");
        return -1;
	}
    jsonElementOrArr* head;
    head = ParseFile(argv[1]);
    PrintJson(head);
    jsonObjectOrArrValue* object;

    object = GetFirstObject(head);

    PrintObject(object);

    object = GetNextObject(object);

    object = FindObjectByKey(head, "web-app");

    jsonElementOrArr* newElement;

    newElement = GetObjectValue(object, JSON_ELEM, UNDEF_VALUE);
    object = GetFirstObject(newElement);
    newElement = GetObjectValue(object, JSON_ARRAY, UNDEF_VALUE);
    object = GetFirstObject(newElement);
    newElement = GetObjectValue(object, JSON_ELEM, UNDEF_VALUE);
    object = GetFirstObject(newElement);


    char* str = NULL;
    str = GetObjectValue(object, SIMPLE_ELEM, STRING_T);
    if (str != NULL) {
        printf("%s \n", str);
    }

    if (str != NULL) {
        free(str);
    }
    freeJsonElement(head);
    return 0;
}
