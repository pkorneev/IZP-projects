#include <string.h>
#include <malloc.h>

#include "table.h"

int Table_ParseReturnCount(char* rv) {
    int result = 0;
    strtok(rv, "\n");
    do {
        result++;
    } while (strtok(NULL, "\n") != NULL);
    return result;
}

TTable* Table_Create(char const* rv, TDelim* delim) {
    TTable* this;
    if(rv == NULL) {
        this = (TTable*) malloc(sizeof(TTable));
        this->len = 1;
        this->data = (TRow**) malloc(this->len * sizeof(TRow*));
        this->data[0] = Row_Create(NULL, delim);
        return this;
    }
    int buf_len = strlen(rv);
    char* buf = (char*) malloc((buf_len + 1) * sizeof(char));
    strcpy(buf, rv);
    if(buf[buf_len - 1] != '\n') {
        strcat(buf + buf_len, "\n");
    }
    this = (TTable*) malloc(sizeof(TTable));
    this->len = Table_ParseReturnCount(buf);
    this->data = (TRow**) malloc(this->len * sizeof(TRow*));
    char* temp = buf;
    int max_len = 0;
    for(int i = 0; i < this->len; ++i) {
        this->data[i] = Row_Create(temp, delim);
        if(Row_Length(this->data[i]) > max_len) {
            max_len = Row_Length(this->data[i]);
        }
        temp += strlen(temp) + 1;
    }
    for(int i = 0; i < this->len; ++i) {
        int starting_len = Row_Length(this->data[i]);
        for(int j = 0; j < max_len - starting_len; ++j) {
            Row_Insert(this->data[i], starting_len + j, NULL);
        }
    }
    free(buf);
    return this;
}

int Table_Height(TTable* this) {
	return this->len;
}

int Table_Width(TTable* this) {
	if(Table_Height(this) == 0) {
		return 0;
	}
	return Row_Length(this->data[0]);
}

TCell* Table_At(TTable* this, int r, int c) {
	if(r < 0 || r >= this->len) {
		return NULL;
	}
	return Row_At(this->data[r], c);
}

int Table_InsertRow(TTable* this, int index) {
    if(index < 0 || index > Table_Height(this)) {
        return -1;
    }
    TRow* rv = Row_Create(NULL, NULL);
    this->len++;
    this->data = (TRow**) realloc(this->data, this->len * sizeof(TRow*));
    for(int i = this->len - 1; i > index; --i) {
        this->data[i] = this->data[i - 1];
    }
    this->data[index] = rv;
    return 0;
}

int Table_InsertCol(TTable* this, int index) {
    if(index < 0 || index > Table_Width(this)) {
        return -1;
    }
    for(int i = 0; i < Table_Height(this); ++i) {
        Row_Insert(this->data[i], index, NULL);
    }
    return 0;
}

int Table_RemoveRow(TTable* this, int index) {
    if(Table_Height(this) == 1) {
        return 1;
    }
    if(index < 0 || index > Table_Height(this)) {
        return -1;
    }
    Row_Destroy(this->data[index]);
    int temp_width = Row_Length(this->data[(index ? 0 : 1)]);
    for(int i = index; i < temp_width - 1; ++i) {
        this->data[i] = this->data[i + 1];
    }
    this->len--;
    this->data = (TRow**) realloc(this->data, this->len * sizeof(TRow*));
    return 0;
}

int Table_RemoveCol(TTable* this, int index) {
    if(Table_Width(this) == 1) {
        return 1;
    }
    if(index < 0 || index >= Table_Width(this)) {
        return -1;
    }
    for(int i = 0; i < Table_Height(this); ++i) {
        Row_Remove(this->data[i], index);
    }
    return 0;
}

void Table_Destroy(TTable* this) {
    for(int i = 0; i < this->len; i++) {
        Row_Destroy(this->data[i]);
    }
    free(this->data);
    free(this);
}