#include "defaults.h"

char default_Delim[2] = " ";
char default_Cell[1] = "";
char default_Filename[8] = "tab.txt";
char default_CmdDelim[3] = ";\n";
char default_SelDelim[5] = "[, ]";
int  default_DoubleMaxLength = 32;

char const* Default_Delim() {
    return default_Delim;
}

char const* Default_Cell() {
    return default_Cell;
}

char const* Default_Filename() {
	return default_Filename;
}

char const* Default_CmdDelim() {
	return default_CmdDelim;
}

char const* Default_SelDelim() {
	return default_SelDelim;
}

int Default_DoubleMaxLength() {
	return default_DoubleMaxLength;
}