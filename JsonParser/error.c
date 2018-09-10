#include "error.h"

void PrintError(ERRORS *errorcode, int cur)
{
    char *ErrorNotification[LAST] =
    {
        "ERROR: Not enough memory",
        "ERROR: Can't open the file",
        "ERROR: Invalid JSON",
        "ERROR: Incorrect key",
        "ERROR: Incorrect value",
        "ERROR: Json bracers or brackets mistake",
        "ERROR: Incorrect special symbols using"
    };
    fprintf(stderr, "%s on symbol %d\n", ErrorNotification[*errorcode - 1], cur);
}



bool CheckJson(char* str, ERRORS* errorcode) {
    int i = 0, j = 0;
    short quoteOpened = -1;
    char* stack = malloc(sizeof(char) * 100), *memtest;
    stack[0] = '0';
    bool inRoot = false;
    while (str[i] != '\0') {
        if (str[i] == '"' && i > 0 && str[i - 1] != '\\') {
            quoteOpened = -quoteOpened;
        }

        if ((str[i] == '{' || str[i] == '}' || str[i] == '[' || str[i] == ']') && quoteOpened == -1) {
            inRoot = true;
            if (j % 100 == 99) {
                memtest = realloc(stack, sizeof(char) * ((unsigned long)j + 101));
                if (memtest == NULL) {
                    *errorcode = ERR_NOT_ENOUGH_MEMORY;
                    free(stack);
                    return false;
                }
                stack = memtest;
            }
            stack[j] = str[i];
            if (j > 0) {
                if ((stack[j - 1] == '{' && stack[j] == '}') || (stack[j - 1] == '[' && stack[j] == ']')) {  //Clean stack
                    stack[j - 1] = '0';
                    stack[j] = '0';
                    j -= 2;
                }
            }
            if (j < 0) {
                inRoot = false;
            }
            j++;
        } else if (!inRoot && !isspace(str[i])) {
            *errorcode = ERR_INVALID_JSON;
            free(stack);
            return false;
        }
        i++;        
    }
    if (stack[0] != '0') {
        *errorcode = ERR_BR_MISTAKE;
        free(stack);
        return false;
    }
    free(stack);
    return true;
}

bool CheckExp(char* str, int* cur) {
    if (str[*cur - 1] == '.') {
        return false;
    }
    (*cur)++;
    if (str[*cur] != '+' && str[*cur] != '-' && !isdigit(str[*cur])) {
        return false;
    }
    return true;
}

bool CheckBackslash(char* str, int cur) {
    if (str[cur - 1] != '\\' && str[cur + 1] != '\\' && str[cur + 1] != 'r' && str[cur + 1] != 't' && str[cur + 1] != 'n' &&
            str[cur + 1] != '"' && str[cur + 1] != 'f' && str[cur + 1] != 'b' && str[cur + 1] != '/' && str[cur + 1] != 'u'
            && str[cur + 1] != 'o' && str[cur + 1] != ':') {
        return false;
    }
    return true;
}

bool CheckSpecialSymbols(char* str, int cur, ERRORS* errorcode) {
    if (str[cur] == '\t' || str[cur] == '\r' || str[cur] == '\n') {
        *errorcode = ERR_INCORRECT_SS;
        return false;
    }
    if (str[cur] == '\\') {
        if (!CheckBackslash(str, cur)) {
            *errorcode = ERR_INCORRECT_SS;
            return false;
        }
    }
    return true;
}

bool CheckValue(char* str, int* cur, char valueType, ERRORS* errorcode) {
    bool isDouble = false, isExp = false;
    switch(valueType) {
    case 's':
        (*cur)++;
        break;
    case 'n':
        if (str[*cur] != 'n' || str[*cur + 1] != 'u' || str[*cur + 2] != 'l' || str[*cur + 3] != 'l') {
            *errorcode = ERR_INCORRECT_VALUE;
            break;
        } else {
            *cur = *cur + 4;
        }
        break;
    case 't':
        if (str[*cur] != 't' || str[*cur + 1] != 'r' || str[*cur + 2] != 'u' || str[*cur + 3] != 'e') {
            *errorcode = ERR_INCORRECT_VALUE;
            break;
        } else {
            *cur = *cur + 4;
        }
        break;
    case 'f':
        if (str[*cur] != 'f' || str[*cur + 1] != 'a' || str[*cur + 2] != 'l' || str[*cur + 3] != 's' || str[*cur + 4] != 'e') {
            *errorcode = ERR_INCORRECT_VALUE;
            break;
        } else {
            *cur = *cur + 5;
            }
        break;
    case 'd':
        if (str[*cur] == '-') {
            (*cur)++;
        }
        if (str[*cur] == '0' && isdigit(str[*cur + 1])) {
            *errorcode = ERR_INCORRECT_VALUE;
            break;
        }
        while (isdigit(str[*cur]) || str[*cur] == '.' || str[*cur] == 'e' || str[*cur] == 'E') {
            if (str[*cur] == '.') {
                if (isDouble != true) {
                    isDouble = true;
                } else {
                    *errorcode = ERR_INCORRECT_VALUE;
                    break;
                }
            }
            if (str[*cur] == 'e' || str[*cur] == 'E') {
                if (isExp != true) {
                    isExp = true;
                    isDouble = true;
                    if (!CheckExp(str, cur)) {
                        *errorcode = ERR_INCORRECT_VALUE;
                        break;
                    }
                } else {
                    *errorcode = ERR_INCORRECT_VALUE;
                    break;
                }
            }
            (*cur)++;
        }
        break;
    }
    if (*errorcode != NO_ERR) {
        return isDouble;
    }
    while (str[*cur] != '}' && str[*cur] != ',' && str[*cur] != ']') {        
        if (!isspace(str[*cur])) {
            *errorcode = ERR_INCORRECT_VALUE;
            break;
        }
        (*cur)++;
    }

    if (str[*cur] == ',') {
        return isDouble;
    }
    while (isspace(str[*cur])) {
        (*cur)++;
        if (str[*cur] == '\0') {
            return isDouble;
        }
    }
    return isDouble;
}

void CheckKey(char* str, int* cur, ERRORS* errorcode) {
    while (str[*cur] != ':') {
        (*cur)++;
        if(!isspace(str[*cur]) && str[*cur] != ':') {
            *errorcode = ERR_INCORRECT_KEY;
        }
    }
    (*cur)++;
}

