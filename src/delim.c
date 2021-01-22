#include <string.h>
#include <malloc.h>

#include "defaults.h"

#include "delim.h"

int Delim_MakeUnique(char* lv, char const* rv) {
    strcpy(lv, "");
    unsigned len = strlen(rv);
    for (unsigned i = 0, j = 0; i < len; ++i) {
        if (strchr(lv, rv[i]) == NULL) {
            strncat(lv + (j++), rv + i, 1);
        }
    }
    return 0;
}

TDelim* Delim_Create(char const* rv) {
    if(rv == NULL || strlen(rv) == 0) {
        rv = Default_Delim();
    }
    TDelim* this = (TDelim*) malloc(sizeof(TDelim));
    this->data = (char*) malloc((strlen(rv) + 1) * sizeof(char));
    Delim_MakeUnique(this->data, rv);
    this->data = (char*) realloc(this->data, strlen(this->data) + 1);
    return this;
}

char const* Delim_Data(TDelim* this) {
    return this->data;
}

char Delim_First(TDelim* this) {
    return *Delim_Data(this);
}

void Delim_Destroy(TDelim* this) {
    free(this->data);
    free(this);
}
