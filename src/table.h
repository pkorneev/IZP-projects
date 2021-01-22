#ifndef SPS_TABLE_H
#define SPS_TABLE_H

#include "row.h"
#include "delim.h"

typedef struct {
    TRow** data;
    int len;
} TTable;

TTable* Table_Create(char const*, TDelim*);
int Table_Height(TTable*);
int Table_Width(TTable*);
TCell* Table_At(TTable*, int, int);
int Table_InsertRow(TTable*, int);
int Table_InsertCol(TTable*, int);
int Table_RemoveRow(TTable*, int);
int Table_RemoveCol(TTable*, int);
void Table_Destroy(TTable*);

#endif //SPS_TABLE_H
