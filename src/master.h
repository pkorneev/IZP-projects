#ifndef SPS_MASTER_H
#define SPS_MASTER_H

#include "table.h"
#include "io.h"
#include "delim.h"
#include "select.h"
#include "variables.h"

typedef struct {
	TTable* table;
	TIOmodule* table_file;
	TIOmodule* command_file;
	TDelim* delim;
	TSelection* current_selection;
	TVariables* variables;
	char* commands;
	char* current_command;
	int state;
} TMaster;

TMaster* Master_Create(char const*, char const*, char const*);
int Master_ExecuteNext(TMaster*);
void Master_Destroy(TMaster*);

#endif //SPS_MASTER_H
