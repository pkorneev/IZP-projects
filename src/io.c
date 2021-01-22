#include <string.h>
#include <malloc.h>

#include "io.h"

#include "defaults.h"

TIOmodule* IO_Create(char const* filename) {
	if(filename == NULL || strlen(filename) == 0) {
		filename = Default_Filename();
	}
	TIOmodule* this = (TIOmodule*) malloc(sizeof(TIOmodule));
	this->file = NULL;
	this->filename = (char*) malloc((strlen(filename) + 1) * sizeof(char));
	this->state = 0;
	this->buffer = NULL;
	strcpy(this->filename, filename);
	return this;
}

char* IO_Read(TIOmodule* this) {
	this->file = fopen(this->filename, "r");
	if(this->file == NULL) {
		this->state = -1;
		return NULL;
	}
	this->state = 1;
	int file_len;
	for(file_len = 0; !feof(this->file); ++file_len, fgetc(this->file));
	rewind(this->file);
	this->buffer = (char*) malloc(file_len * sizeof(char));
	fread((void*)(this->buffer), sizeof(char), file_len - 1, this->file);
	this->buffer[file_len - 1] = 0;
	return this->buffer;
}

int IO_Write(TIOmodule *this, TTable *table, TDelim *delim)
{
    TCell *cell;
    int L;
    int buffer_size;
    char *buffer;
    this->file = fopen(this->filename, "w");

    for (int i = 0; i < Table_Height(table); i++)
    {

        for (int j = 0; j < Table_Width(table); j++)
        {
            cell = Table_At(table, i, j);
            L = strlen(Cell_DataString(cell));
            buffer_size = 2 * L + 2; // worst-case scenario
            buffer = (char *)malloc(buffer_size * sizeof(char));
            fprintf(this->file, "%s", Cell_Encode(cell, buffer, buffer_size, delim));
            if (j < Table_Width(table) - 1)
                fprintf(this->file, "%c", Delim_First(delim));
            free(buffer);
        }
        fprintf(this->file, "\n");
    }
    fclose(this->file);
    return 0;
}

void IO_Destroy(TIOmodule* this) {
	fclose(this->file);
	free(this->filename);
	free(this->buffer);
	free(this);
}