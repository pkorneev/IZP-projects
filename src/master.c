#include <string.h>
#include <malloc.h>

#include "defaults.h"
#include "cell.h"

#include "master.h"

TMaster* Master_Create(char const* filename_table, char const* cmd_sequence, char const* delim_str) {
	if(cmd_sequence == NULL) {
		return NULL;
	}
	TMaster* this = (TMaster*) malloc(sizeof(TMaster));
	this->table_file = IO_Create(filename_table);
	this->delim = Delim_Create(delim_str);
	this->current_selection = Selection_Create();
	this->variables = Variables_Create();
	this->table = Table_Create(IO_Read(this->table_file), this->delim);
	if(strstr(cmd_sequence, "-c") == cmd_sequence) {
		this->command_file = IO_Create(this->commands + 2);
		this->commands = IO_Read(this->command_file);
	}
	else {
		this->command_file = NULL;
		this->commands = (char*) malloc((strlen(cmd_sequence) + 1) * sizeof(char));
		strcpy(this->commands, cmd_sequence);
	}
	if(strpbrk(this->commands + strlen(this->commands) - 1, Default_CmdDelim()) == NULL) {
		this->commands = (char*) realloc(this->commands, strlen(this->commands) + 2);
		strcat(this->commands, ";");
	}
	this->current_command = this->commands;
	this->state = 1;
	return this;
}

int Master_Execute(TMaster* this) {
	char* cmd = this->current_command;
	char* temp_str;
	TCell* temp_cell;
	TSelection* temp_sel;
	double temp_num;
	int temp_count;
	if(cmd == NULL) {
		return 0;
	}
	else if(!strcmp(cmd, "[min]")) {
		temp_sel = Selection_Create();
		temp_count = 0;
		for(int i = this->current_selection->r1; i <= this->current_selection->r2; ++i) {
			for(int j = this->current_selection->c1; j <= this->current_selection->c2; ++j) {
				temp_cell = Table_At(this->table, i, j);
				if(Cell_IsNumber(temp_cell)) {
					if(!temp_count || Cell_DataDouble(temp_cell) < temp_num) {
						temp_num = Cell_DataDouble(temp_cell);
						temp_sel->r1 = temp_sel->r2 = i;
						temp_sel->c1 = temp_sel->c2 = j;
						temp_count = 1;
					}
				}
			}
		}
		memcpy((void*) this->current_selection, (void*) temp_sel, sizeof(TSelection));
		Selection_Destroy(temp_sel);
	}
	else if(!strcmp(cmd, "[max]")) {
		temp_sel = Selection_Create();
		temp_count = 0;
		for(int i = this->current_selection->r1; i <= this->current_selection->r2; ++i) {
			for(int j = this->current_selection->c1; j <= this->current_selection->c2; ++j) {
				temp_cell = Table_At(this->table, i, j);
				if(Cell_IsNumber(temp_cell)) {
					if(!temp_count || Cell_DataDouble(temp_cell) > temp_num) {
						temp_num = Cell_DataDouble(temp_cell);
						temp_sel->r1 = temp_sel->r2 = i;
						temp_sel->c1 = temp_sel->c2 = j;
						temp_count = 1;
					}
				}
			}
		}
		memcpy((void*) this->current_selection, (void*) temp_sel, sizeof(TSelection));
		Selection_Destroy(temp_sel);
	}
	else if(!strcmp(cmd, "[_]")) {
		memcpy((void*) this->current_selection, (void*) this->variables->_var, sizeof(TSelection));
	}
	else if(!strcmp(cmd, "irow")) {
		Table_InsertRow(this->table, this->current_selection->r1);
	}
	else if(!strcmp(cmd, "arow")) {
		Table_InsertRow(this->table, this->current_selection->r2 + 1);
	}
	else if(!strcmp(cmd, "icol")) {
		Table_InsertCol(this->table, this->current_selection->c1);
	}
	else if(!strcmp(cmd, "acol")) {
		Table_InsertCol(this->table, this->current_selection->c2 + 1);
	}
	else if(!strcmp(cmd, "drow")) {
		for(int i = this->current_selection->r1, j = i; i <= this->current_selection->r2; ++i) {
			Table_RemoveRow(this->table, j);
		}
		this->current_selection->r2 = this->current_selection->r1;
		if(!Selection_IsLegal(this->current_selection, this->table)) {
			this->current_selection->r1--;
			this->current_selection->r2--;
		}
	}
	else if(!strcmp(cmd, "dcol")) {
		for(int i = this->current_selection->c1, j = i; i <= this->current_selection->c2; ++i) {
			Table_RemoveCol(this->table, j);
		}
		this->current_selection->c2 = this->current_selection->c1;
		if(!Selection_IsLegal(this->current_selection, this->table)) {
			this->current_selection->c1--;
			this->current_selection->c2--;
		}
	}
	else if(!strcmp(cmd, "clear")) {
		for(int i = this->current_selection->r1; i <= this->current_selection->r2; ++i) {
			for(int j = this->current_selection->c1; j <= this->current_selection->c2; ++j) {
				Cell_Clear(Table_At(this->table, i, j));
			}
		}
	}
	else if(!strcmp(cmd, "[set]")) {
		memcpy((void*) this->variables->_var, (void*) this->current_selection, sizeof(TSelection));
	}
	else if(strstr(cmd, "set") == cmd) {
		temp_cell = Cell_Create(strstr(cmd, " ") + 1);
		for(int i = this->current_selection->r1; i <= this->current_selection->r2; ++i) {
			for(int j = this->current_selection->c1; j <= this->current_selection->c2; ++j) {
				Cell_SetData(Table_At(this->table, i, j), Cell_DataString(temp_cell));
			}
		}
		Cell_Destroy(temp_cell);
	}
	else if(strstr(cmd, "swap") == cmd) {
		if(this->current_selection->r1 ==
		   this->current_selection->r2 &&
		   this->current_selection->c1 ==
		   this->current_selection->c2) {
			temp_sel = Selection_Create();
			Selection_Set(temp_sel, strchr(cmd, '['), this->table);
			Cell_Swap(Table_At(this->table, temp_sel->r1, temp_sel->c1), 
			          Table_At(this->table, this->current_selection->r1, this->current_selection->c1));
			Selection_Destroy(temp_sel);
		}
	}
	else if(strstr(cmd, "sum") == cmd) {
		temp_sel = Selection_Create();
		Selection_Set(temp_sel, strchr(cmd, '['), this->table);
		temp_num = 0;
		for(int i = this->current_selection->r1; i <= this->current_selection->r2; ++i) {
			for(int j = this->current_selection->c1; j <= this->current_selection->c2; ++j) {
				temp_cell = Table_At(this->table, i, j);
				if(Cell_IsNumber(temp_cell)) {
					temp_num += Cell_DataDouble(temp_cell);
				}
			}
		}
		Cell_SetDataDouble(Table_At(this->table, temp_sel->r1, temp_sel->c1), temp_num);
		Selection_Destroy(temp_sel);
	}
	else if(strstr(cmd, "avg") == cmd) {
		temp_sel = Selection_Create();
		Selection_Set(temp_sel, strchr(cmd, '['), this->table);
		temp_num = 0;
		temp_count = 0;
		for(int i = this->current_selection->r1; i <= this->current_selection->r2; ++i) {
			for(int j = this->current_selection->c1; j <= this->current_selection->c2; ++j) {
				temp_cell = Table_At(this->table, i, j);
				if(Cell_IsNumber(temp_cell)) {
					temp_num += Cell_DataDouble(temp_cell);
					temp_count++;
				}
			}
		}
		Cell_SetDataDouble(Table_At(this->table, temp_sel->r1, temp_sel->c1), temp_num / temp_count);
		Selection_Destroy(temp_sel);
	}
	else if(strstr(cmd, "count") == cmd) {
		temp_sel = Selection_Create();
		Selection_Set(temp_sel, strchr(cmd, '['), this->table);
		temp_count = 0;
		for(int i = this->current_selection->r1; i <= this->current_selection->r2; ++i) {
			for(int j = this->current_selection->c1; j <= this->current_selection->c2; ++j) {
				temp_cell = Table_At(this->table, i, j);
				if(strlen(Cell_DataString(temp_cell)) > 0) {
					temp_count++;
				}
			}
		}
		Cell_SetDataDouble(Table_At(this->table, temp_sel->r1, temp_sel->c1), (double) temp_count);
		Selection_Destroy(temp_sel);
	}
	else if(strstr(cmd, "len") == cmd) {
		temp_sel = Selection_Create();
		Selection_Set(temp_sel, strchr(cmd, '['), this->table);
		Cell_SetDataDouble(Table_At(this->table, temp_sel->r1, temp_sel->c1), 
			(double) strlen(Cell_DataString(Table_At(this->table, 
			this->current_selection->r1, this->current_selection->c1))));
		Selection_Destroy(temp_sel);
	}
	else if(strstr(cmd, "def") == cmd) {
		for(int i = this->current_selection->r1; i <= this->current_selection->r2; ++i) {
			for(int j = this->current_selection->c1; j <= this->current_selection->c2; ++j) {
				Cell_Copy(Table_At(this->table, i, j), this->variables->_vars[strchr(cmd, '_')[1] - '0']);
			}
		}
	}
	else if(strstr(cmd, "use") == cmd) {
		Cell_Copy(this->variables->_vars[strchr(cmd, '_')[1] - '0'], Table_At(this->table, this->current_selection->r1, this->current_selection->c1));
	}
	else if(strstr(cmd, "inc") == cmd) {
		temp_cell = this->variables->_vars[strchr(cmd, '_')[1] - '0'];
		if(Cell_IsNumber(temp_cell)) {
			temp_num = Cell_DataDouble(temp_cell) + 1;
		}
		else {
			temp_num = 1;
		}
		Cell_SetDataDouble(temp_cell, temp_num);
	}
	else if(strstr(cmd, "[find") == cmd) {
		temp_str = (char*) malloc(strlen(cmd) * sizeof(char));
		strncpy(temp_str, strchr(cmd, ' ') + 1, strchr(cmd, ']') - strchr(cmd, ' '));
		temp_cell = Cell_Create(temp_str);
		free(temp_str);
		temp_count = 0;
		for(int i = this->current_selection->r1; i <= this->current_selection->r2; ++i) {
			for(int j = this->current_selection->c1; j <= this->current_selection->c2; ++j) {
				if(!strcmp(Cell_DataString(Table_At(this->table, i, j)), Cell_DataString(temp_cell))) {
					this->current_selection->r1 =
					this->current_selection->r2 = i;
					this->current_selection->c1 =
					this->current_selection->c2 = j;
					temp_count = 1;
					break;
				}
			}
			if(temp_count) {
				break;
			}
		}
		Cell_Destroy(temp_cell);
	}
	else if(strstr(cmd, "[") == cmd) {
		Selection_Set(this->current_selection, cmd, this->table);
	}
	return 0;
}

int Master_ExecuteNext(TMaster* this) {
	char* temp;
	if(this->state == 1) {
		temp = this->commands;
	}
	else {
		temp = NULL;
	}
	this->current_command = strtok(temp, Default_CmdDelim());
	this->state = 2;
	if(this->current_command == NULL) {
		this->state = 0;
	}
	else if(Master_Execute(this)) {
		this->state = 0;
	}
	return this->state;
}

void Master_Destroy(TMaster* this) {
	if(this->command_file == NULL) {
		free(this->commands);
	}
	else {
		IO_Destroy(this->command_file);
	}
	Table_Destroy(this->table);
	Variables_Destroy(this->variables);
	Selection_Destroy(this->current_selection);
	Delim_Destroy(this->delim);
	IO_Destroy(this->table_file);
	free(this);
}