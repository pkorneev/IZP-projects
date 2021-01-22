#include <string.h>

#include "master.h"

#include "ui.h"

void UI_Start(int argc, char** argv) {
	char* filename;
	char* cmd_sequence;
	char* delim;
	int shear = 0;
	if(argc < 3) {
		return;
	}
	if(!strcmp(argv[1], "-d")) {
		if(argc < 5) {
			return;
		}
		delim = argv[2];
		shear = 2;
	}
	else {
		delim = NULL;
	}
	cmd_sequence = argv[1 + shear];
	filename = argv[2 + shear];
	TMaster* master = Master_Create(filename, cmd_sequence, delim);
	while(Master_ExecuteNext(master));
	IO_Write(master->table_file, master->table, master->delim);
	printf("Maybe...\n");
	Master_Destroy(master);
}