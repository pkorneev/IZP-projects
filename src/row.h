#ifndef SPS_ROW_H
#define SPS_ROW_H

#include "cell.h"
#include "delim.h"

typedef struct {
    TCell** data;
    int len;
} TRow;

/*
 * data
 * |
 * V
 * ptr0 ptr1 ptr2 ... ptr[len-1]
 * |    |    |        |
 * V    V    V        V
 * c0   c1   c2 ...
 */

TRow* Row_Create(char const*, TDelim*);
int Row_Length(TRow*);
TCell* Row_At(TRow*, int);
int Row_Insert(TRow*, int, TCell*);
int Row_Remove(TRow*, int);
void Row_Destroy(TRow*);

#endif //SPS_ROW_H
