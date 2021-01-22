#ifndef SPS_SELECT_H
#define SPS_SELECT_H

#include "table.h"

typedef struct {
	int r1, c1, r2, c2;
} TSelection;

TSelection* Selection_Create();
int Selection_Set(TSelection*, char const*, TTable*);
int Selection_IsLegal(TSelection*, TTable*);
void Selection_Destroy(TSelection*);

#endif //SPS_SELECT_H