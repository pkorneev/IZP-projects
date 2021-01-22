#ifndef SPS_VARIABLES_H
#define SPS_VARIABLES_H

#define VARS_COUNT 10

#include "select.h"
#include "cell.h"

typedef struct {
	TSelection* _var;
	TCell* _vars[10];
} TVariables;

TVariables* Variables_Create();
void Variables_Destroy(TVariables*);

#endif //SPS_VARIABLES_H