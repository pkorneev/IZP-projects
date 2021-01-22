#include <string.h>
#include <malloc.h>

#include "row.h"

int Row_ParseReturnCount(char* rv, TDelim* delim) {
    int result = 0;
    char* token;
    while(1) {
        if(rv[0] == '"') {
            while(1) {
                rv = strchr(rv, '"');
                if(rv == NULL) {
                    return -1;
                }
                if(rv[-1] != '\\') {
                    break;
                }
                rv++;
            }
        }
        token = strtok(rv, Delim_Data(delim));
        result++;
        if(token == NULL) {
            break;
        }
        rv += strlen(token) + 1;
    }
    return result;
}

// "i\"am here"\0123\0"hello"
// rv[-1] === *(rv - 1)
// a[b] === *(a + b) === *(b + a) === b[a]
TRow* Row_Create(char const* rv, TDelim* delim) {
    if(rv == NULL) {
        TRow* this = (TRow*) malloc(sizeof(TRow));
        this->len = 1;
        this->data = (TCell**) malloc(this->len * sizeof(TCell*));
        this->data[0] = Cell_Create(NULL);
        return this;
    }
    char* buf = (char*) malloc((strlen(rv) + 1) * sizeof(char));
    strcpy(buf, rv);
    TRow* this = (TRow*) malloc(sizeof(TRow));
    this->len = Row_ParseReturnCount(buf, delim);
    this->data = (TCell**) malloc(this->len * sizeof(TCell*));
    char* temp = buf;
    for(int i = 0; i < this->len; ++i) {
        this->data[i] = Cell_Create(temp);
        temp += strlen(temp) + 1;
    }
    free(buf);
    return this;
}

int Row_Length(TRow* this) {
    return this->len;
}

TCell* Row_At(TRow* this, int c) {
	if(c < 0 || c >= this->len) {
		return NULL;
	}
	return this->data[c];
}

// a . b c d
// a[this->len - 1] = a[this->len - 2]

int Row_Insert(TRow* this, int index, TCell* rv) {
    if(index < 0 || index > this->len) {
        return -1;
    }
    if(rv == NULL) {
        rv = Cell_Create(NULL);
    }
    this->len++;
    this->data = (TCell**) realloc(this->data, this->len * sizeof(TCell*));
    for(int i = this->len - 1; i > index; --i) {
        this->data[i] = this->data[i - 1];
    }
    this->data[index] = rv;
    return 0;
}

// a c d d

int Row_Remove(TRow* this, int index) {
    if(this->len == 1) {
        return 1;
    }
    if(index < 0 || index >= this->len) {
        return -1;
    }
    Cell_Destroy(this->data[index]);
    for(int i = index; i < this->len - 1; ++i) {
        this->data[i] = this->data[i + 1];
    }
    this->len--;
    this->data = (TCell**) realloc(this->data, this->len * sizeof(TCell*));
    return 0;
}

void Row_Destroy(TRow* this)
{
    for(int i = 0; i < this->len; ++i) {
        Cell_Destroy(this->data[i]);
    }
    free(this->data);
    free(this);
}
