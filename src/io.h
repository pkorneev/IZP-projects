#ifndef SPS_IO_H
#define SPS_IO_H

#include <stdio.h>

#include "table.h"
#include "delim.h"

/*
 * file -> char*
 * TTable -> file
 * open, save, close
 */

typedef struct {
	FILE* file;
	char* filename;
	int state;
	char* buffer;
} TIOmodule;

TIOmodule* IO_Create(char const*);
char* IO_Read(TIOmodule*);
int IO_Write(TIOmodule*, TTable*, TDelim*);
void IO_Destroy(TIOmodule*);

#endif //SPS_IO_H