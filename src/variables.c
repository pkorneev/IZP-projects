#include <string.h>
#include <malloc.h>

#include "variables.h"

TVariables* Variables_Create() {
	TVariables* this = (TVariables*) malloc(sizeof(TVariables));
	this->_var = Selection_Create();
	for(int i = 0; i < VARS_COUNT; ++i) {
		this->_vars[i] = Cell_Create("");
	}
	return this;
}

void Variables_Destroy(TVariables* this) {
	for(int i = 0; i < VARS_COUNT; ++i) {
		Cell_Destroy(this->_vars[i]);
	}
	Selection_Destroy(this->_var);
	free(this);
}