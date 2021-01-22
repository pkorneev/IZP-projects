#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defaults.h"

#include "cell.h"

int Cell_Parse(char* lv, char const* rv) {
    int result = 0;
    strcpy(lv, rv);
    unsigned len = strlen(rv);
    for(unsigned i = 0, j = 0; i < len + 1; ++i) {
        if (rv[i] == '"') {
            if (i == 0 || i == len - 1) {
                result ^= 1;
                continue;
            }
            else {
                result = 2;
                break;
            }
        }
        if(rv[i] == '\\') {
            ++i;
        }
        lv[j] = rv[i];
    }
    return result;
}

TCell* Cell_Create(char const* rv) {
    if(rv == NULL) {
        rv = Default_Cell();
    }
    TCell* this = (TCell*) malloc(sizeof(TCell));
    this->data = (char*) malloc((strlen(rv) + 1) * sizeof(char));
    Cell_Parse(this->data, rv);
    this->data = (char*) realloc(this->data, (strlen(this->data) + 1) * sizeof(char));
    return this;
}

int Cell_SetData(TCell* this, char const* rv)
{
    free(this->data);
    this->data = (char*) malloc((strlen(rv) + 1) * sizeof(char));
    if(this->data == NULL) {
        return -1;
    }
    strcpy(this->data, rv);
    return 0;
}

int Cell_SetDataDouble(TCell* this, double rv) {
    free(this->data);
    this->data = (char*) malloc(Default_DoubleMaxLength() * sizeof(char));
    if(snprintf(this->data, Default_DoubleMaxLength(), "%g", rv) >= Default_DoubleMaxLength()) {
        return -1;
    }
    this->data = (char*) realloc(this->data, (strlen(this->data) + 1) * sizeof(char));
    return 0;
}

int Cell_Clear(TCell* this) {
    return Cell_SetData(this, "");
}

int Cell_Copy(TCell* this, TCell const* rv) {
    free(this->data);
    this->data = (char*) malloc((strlen(rv->data) + 1) * sizeof(char));
    strcpy(this->data, rv->data);
    return 0;
}

int Cell_Swap(TCell* lv, TCell* rv) {
    TCell* temp = Cell_Create(NULL);
    Cell_Copy(temp, lv);
    Cell_Copy(lv, rv);
    Cell_Copy(rv, temp);
    Cell_Destroy(temp);
    return 0;
}

char const* Cell_DataString(TCell* this) {
    return this->data;
}

int Cell_IsNumber(TCell* this) {
    char* ptr;
    strtod(this->data, &ptr);
    return strchr(this->data, '\0') == ptr;
}

double Cell_DataDouble(TCell* this) {
    return strtod(this->data, NULL);
}

char* Cell_Encode(TCell* this, char* destination, int dest_size, TDelim* delim) {
    char const* source = Cell_DataString(this);
    int add_quotes = 0;
    if(strpbrk(source, Delim_Data(delim)) != NULL) {
        add_quotes = 1;
    }
    int escape_symbols, i, j;
    char const* s = source;
    for(i = 0; s[i]; (s[i] == '"' ? ++i : *++s));
    escape_symbols = i;
    int source_size = strlen(source);
    int result_size = source_size + 2 * add_quotes + escape_symbols + 1;
    if(result_size > dest_size) {
        return NULL;
    }
    strcpy(destination, "");
    for(i = 0, j = 0; i < source_size; ++i) {
        if(add_quotes && (i == 0 || i == source_size - 1)) {
            strcat(destination + j++, "\"");
        }
        if(source[i] == '"') {
            strcat(destination + j++, "\\");
        }
        strncat(destination + j++, source, 1);
    }
    return destination;
}

void Cell_Destroy(TCell* this) {
    if(this == NULL) {
        return;
    }
    free(this->data);
    free(this);
}