#ifndef SPS_CELL_H
#define SPS_CELL_H

#include "delim.h"

typedef struct {
    char* data;
} TCell;

TCell* Cell_Create(char const*);
int Cell_SetData(TCell*, char const*);
int Cell_SetDataDouble(TCell*, double);
int Cell_Copy(TCell*, TCell const*);
int Cell_Clear(TCell*);
int Cell_Swap(TCell*, TCell*);
char const* Cell_DataString(TCell*);
int Cell_IsNumber(TCell*);
double Cell_DataDouble(TCell*);
char* Cell_Encode(TCell*, char*, int, TDelim*);
void Cell_Destroy(TCell*);

#endif //SPS_CELL_H
