#include "methods.h"

int main(int argc, char* argv[]) {
        if (argc < 2) {
            fprintf(stderr, "Wrong number of arguments \n");
            return 0;
        }
        jsonElementOrArr* head = NULL;
        jsonElementOrArr* elemOutput = NULL;
        jsonObjectOrArrValue* objectOutput = NULL;
        char* key = malloc(sizeof(char) * 100);
        if (key == NULL) {
            fprintf(stderr, "not enough memory\n");
            return 0;
        }

        head = ParseFile(argv[1]);
        elemOutput = head;
        int check = -1;
        printf("Enter 1 to print json object \n");
        printf("Enter 2 to print json element or json array\n");
        printf("Enter 3 to get first object in json element or array \n");
        printf("Enter 4 to get next object \n");
        printf("Enter 5 to get prev object \n");
        printf("Enter 6 to find json object by key\n");
        printf("Enter 7 to get the json element or array from object \n");
        printf("Enter 8 to quit\n\n");

        while (check != 8) {
            scanf("%d", &check);
            if (check == 1) {
                PrintObject(objectOutput, 0);
                printf("\n");
            }
            if (check == 2) {
                PrintJson(elemOutput, 0);
                printf("\n");
            }
            if (check == 3) {
                objectOutput = GetFirstObject(elemOutput);
            }
            if (check == 4) {
                objectOutput = GetNextObject(objectOutput);
            }
            if (check == 5) {
                objectOutput = GetPrevObject(objectOutput);
            }
            if (check == 6) {
                printf("Enter the key, please\n");
                scanf("%s", key);
                objectOutput = FindObjectByKey(elemOutput, key);
            }
            if (check == 7) {
                if (objectOutput->obj_type == JSON_ELEM) {
                    elemOutput = GetObjectValue(objectOutput, JSON_ELEM, UNDEF_VALUE);
                    printf("\n");
                } else if (objectOutput->obj_type == JSON_ARRAY) {
                    elemOutput = GetObjectValue(objectOutput, JSON_ARRAY, UNDEF_VALUE);
                    printf("\n");
                } else {
                    fprintf(stderr, "Can not get this Value");
                }
            }
        }

        head = freeJsonElement(head);
        free(key);
        return 0;
}
