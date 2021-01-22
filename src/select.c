#include <string.h>
#include <stdlib.h>

#include "defaults.h"

#include "select.h"

TSelection* Selection_Create() {
	TSelection* this = (TSelection*) malloc(sizeof(TSelection));
	this->r1 = this->c1 = this->r2 = this->c2 = 0;
	return this;
}

int Selection_Set(TSelection* this, char const* rv, TTable* table) {
	char* buf = (char*) malloc((strlen(rv) + 1) * sizeof(char));
	strcpy(buf, rv);
	char* token = strtok(buf, Default_SelDelim());
	if(!strcmp(token, "_")) {
		this->r1 = 0;
		this->r2 = Table_Height(table) - 1;
	}
	else {
		this->r1 = this->r2 = atoi(token);
	}
	token = strtok(NULL, Default_SelDelim());
	if(!strcmp(token, "_")) {
		this->c1 = 0;
		this->c2 = Table_Width(table) - 1;
	}
	else {
		this->c1 = this->c2 = atoi(token);
	}
	token = strtok(NULL, Default_SelDelim());
	if(token != NULL) {
		if(!strcmp(token, "-")) {
			this->r2 = Table_Height(table) - 1;
		}
		else {
			this->r2 = atoi(token);
		}
		token = strtok(NULL, Default_SelDelim());
		if(!strcmp(token, "-")) {
			this->c2 = Table_Width(table) - 1;
		}
		else {
			this->c2 = atoi(token);
		}
	}
	free(buf);
	return 0;
}

int Selection_IsLegal(TSelection* this, TTable* rv) {
	int h = Table_Height(rv), w = Table_Width(rv);
	if(this->r1 < 0 || this->r1 >= h) {
		return 0;
	}
	if(this->r2 < 0 || this->r2 >= h) {
		return 0;
	}
	if(this->c1 < 0 || this->c1 >= w) {
		return 0;
	}
	if(this->c2 < 0 || this->c2 >= w) {
		return 0;
	}
	if(this->r1 > this->r2) {
		return 0;
	}
	if(this->c1 > this->c2) {
		return 0;
	}
	return 1;
}

void Selection_Destroy(TSelection* this) {
	free(this);
}