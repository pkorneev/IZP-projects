//Xkorni03 IZP second project//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VARS_COUNT 10

typedef struct //  struct for delim
{
    char *data;
} TDelim;

typedef struct //  struct for cell(data type)
{
    char *data;
} TCell;

typedef struct //  struct for row(data type)
{
    TCell **data;
    int len;
} TRow;

typedef struct
{
    TRow **data;
    int len;
} TTable;

typedef struct // selection
{
    int r1, c1, r2, c2;
} TSelection;

typedef struct
{
    TSelection *_var;
    TCell *_vars[10];
} TVariables;

typedef struct // input and output
{
    FILE *file;
    char *filename;
    int state;
    char *buffer;
} TIOmodule;

typedef struct
{
    TTable *table;
    TIOmodule *table_file;
    TIOmodule *command_file;
    TDelim *delim;
    TSelection *current_selection;
    TVariables *variables;
    char *commands;
    char *current_command;
    int state;
} TMaster;

char default_Delim[2] = " "; // default data
char default_Cell[1] = "";
char default_Filename[8] = "tab.txt";
char default_CmdDelim[3] = ";\n";
char default_SelDelim[5] = "[, ]";
int default_DoubleMaxLength = 32;

char const *Default_Delim()
{
    return default_Delim;
}

char const *Default_Cell()
{
    return default_Cell;
}

char const *Default_Filename()
{
    return default_Filename;
}

char const *Default_CmdDelim()
{
    return default_CmdDelim;
}

char const *Default_SelDelim()
{
    return default_SelDelim;
}

int Default_DoubleMaxLength()
{
    return default_DoubleMaxLength;
}

int Delim_MakeUnique(char *lv, char const *rv) //remove duplicate symbols
{
    strcpy(lv, "");
    unsigned len = strlen(rv);
    for (unsigned i = 0, j = 0; i < len; ++i)
    {
        if (strchr(lv, rv[i]) == NULL)
        {
            strncat(lv + (j++), rv + i, 1);
        }
    }
    return 0;
}

TDelim *Delim_Create(char const *rv) // create delim
{
    if (rv == NULL || strlen(rv) == 0)
    {
        rv = Default_Delim();
    }
    TDelim *this = (TDelim *)malloc(sizeof(TDelim));
    this->data = (char *)malloc((strlen(rv) + 1) * sizeof(char));
    Delim_MakeUnique(this->data, rv);
    this->data = (char *)realloc(this->data, strlen(this->data) + 1);
    return this;
}

char const *Delim_Data(TDelim *this) // delim data (string)
{
    return this->data;
}

char Delim_First(TDelim *this) // first symbol of delim
{
    return *Delim_Data(this);
}

void Delim_Destroy(TDelim *this) // clear memmory (delim)
{
    free(this->data);
    free(this);
}

int Cell_Parse(char *lv, char const *rv) // parsing data in cells
{
    int result = 0;
    strcpy(lv, rv);
    unsigned len = strlen(rv);
    for (unsigned i = 0, j = 0; i < len + 1; ++i)
    {
        if (rv[i] == '"')
        {
            if (i == 0 || i == len - 1)
            {
                result ^= 1;
                continue;
            }
            else
            {
                result = 2;
                break;
            }
        }
        if (rv[i] == '\\')
        {
            ++i;
        }
        lv[j] = rv[i];
    }
    return result;
}

TCell *Cell_Create(char const *rv) // create cell
{
    if (rv == NULL)
    {
        rv = Default_Cell(); // if nothing to read -> create default cell
    }
    TCell *this = (TCell *)malloc(sizeof(TCell));
    this->data = (char *)malloc((strlen(rv) + 1) * sizeof(char));
    Cell_Parse(this->data, rv);
    this->data = (char *)realloc(this->data, (strlen(this->data) + 1) * sizeof(char));
    return this;
}

int Cell_SetData(TCell *this, char const *rv) //set data from rv to cell
{
    free(this->data);
    this->data = (char *)malloc((strlen(rv) + 1) * sizeof(char));
    if (this->data == NULL)
    {
        return -1;
    }
    strcpy(this->data, rv);
    return 0;
}

int Cell_SetDataDouble(TCell *this, double rv) //
{
    free(this->data);
    this->data = (char *)malloc(Default_DoubleMaxLength() * sizeof(char));
    if (snprintf(this->data, Default_DoubleMaxLength(), "%g", rv) >= Default_DoubleMaxLength())
    {
        return -1;
    }
    this->data = (char *)realloc(this->data, (strlen(this->data) + 1) * sizeof(char));
    return 0;
}

int Cell_Clear(TCell *this) // clear data in cell
{
    return Cell_SetData(this, "");
}

int Cell_Copy(TCell *this, TCell const *rv) // copy data from rv sell to current sell
{
    free(this->data);
    this->data = (char *)malloc((strlen(rv->data) + 1) * sizeof(char));
    strcpy(this->data, rv->data);
    return 0;
}

void Cell_Destroy(TCell *this) // clear memmory(cell)
{
    if (this == NULL)
    {
        return;
    }
    free(this->data);
    free(this);
}

int Cell_Swap(TCell *lv, TCell *rv) // swap lv and rv cells
{
    TCell *temp = Cell_Create(NULL);
    Cell_Copy(temp, lv);
    Cell_Copy(lv, rv);
    Cell_Copy(rv, temp);
    Cell_Destroy(temp);
    return 0;
}

char const *Cell_DataString(TCell *this) // data in current cell
{
    return this->data;
}

int Cell_IsNumber(TCell *this) //
{
    char *ptr;
    strtod(this->data, &ptr);
    return strchr(this->data, '\0') == ptr;
}

double Cell_DataDouble(TCell *this)
{
    return strtod(this->data, NULL);
}

char *Cell_Encode(TCell *this, char *destination, int dest_size, TDelim *delim) //
{
    char const *source = Cell_DataString(this);
    int add_quotes = 0;
    if (strpbrk(source, Delim_Data(delim)) != NULL)
    {
        add_quotes = 1;
    }
    int escape_symbols, i, j;
    char const *s = source;
    for (i = 0; s[i]; (s[i] == '"' ? ++i : *++s))
        ;
    escape_symbols = i;
    int source_size = strlen(source);
    int result_size = source_size + 2 * add_quotes + escape_symbols + 1;
    if (result_size > dest_size)
    {
        return NULL;
    }
    strcpy(destination, "");
    for (i = 0, j = 0; i < source_size; ++i)
    {
        if (add_quotes && (i == 0 || i == source_size - 1))
        {
            strcat(destination + j++, "\"");
        }
        if (source[i] == '"')
        {
            strcat(destination + j++, "\\");
        }
        strncat(destination + j++, source, 1);
    }
    return destination;
}

int Row_ParseReturnCount(char *rv, TDelim *delim) //parsing row
{
    int result = 0; // quantity of сells in the row ( or columns)
    char *token;
    while (1)
    {
        if (rv[0] == '"')
        {
            while (1)
            {
                rv = strchr(rv, '"');
                if (rv == NULL)
                {
                    return -1;
                }
                if (rv[-1] != '\\')
                {
                    break;
                }
                rv++;
            }
        }
        token = strtok(rv, Delim_Data(delim));
        result++;
        if (token == NULL)
        {
            break;
        }
        rv += strlen(token) + 1;
    }
    return result;
}

// "i\"am here"\0123\0"hello"
// rv[-1] === *(rv - 1)
// a[b] === *(a + b) === *(b + a) === b[a]
TRow *Row_Create(char const *rv, TDelim *delim) // create the row
{
    if (rv == NULL) // if nothing to read -> create default row (1 default cell)
    {
        TRow *this = (TRow *)malloc(sizeof(TRow));
        this->len = 1;
        this->data = (TCell **)malloc(this->len * sizeof(TCell *));
        this->data[0] = Cell_Create(NULL);
        return this;
    }
    char *buf = (char *)malloc((strlen(rv) + 1) * sizeof(char));
    strcpy(buf, rv);
    TRow *this = (TRow *)malloc(sizeof(TRow));
    this->len = Row_ParseReturnCount(buf, delim);
    this->data = (TCell **)malloc(this->len * sizeof(TCell *));
    char *temp = buf;
    for (int i = 0; i < this->len; ++i)
    {
        this->data[i] = Cell_Create(temp); // create cells by delim
        temp += strlen(temp) + 1;
    }
    free(buf);
    return this;
}

int Row_Length(TRow *this)
{
    return this->len;
}

TCell *Row_At(TRow *this, int c)
{
    if (c < 0 || c >= this->len)
    {
        return NULL;
    }
    return this->data[c];
}

// a . b c d
// a[this->len - 1] = a[this->len - 2]

int Row_Insert(TRow *this, int index, TCell *rv) //
{
    if (index < 0 || index > this->len)
    {
        return -1;
    }
    if (rv == NULL)
    {
        rv = Cell_Create(NULL);
    }
    this->len++;
    this->data = (TCell **)realloc(this->data, this->len * sizeof(TCell *));
    for (int i = this->len - 1; i > index; --i)
    {
        this->data[i] = this->data[i - 1];
    }
    this->data[index] = rv;
    return 0;
}

// a c d d

int Row_Remove(TRow *this, int index) // remove row from the teble(delete)
{
    if (this->len == 1)
    {
        return 1;
    }
    if (index < 0 || index >= this->len)
    {
        return -1;
    }
    Cell_Destroy(this->data[index]);
    for (int i = index; i < this->len - 1; ++i)
    {
        this->data[i] = this->data[i + 1];
    }
    this->len--;
    this->data = (TCell **)realloc(this->data, this->len * sizeof(TCell *));
    return 0;
}

void Row_Destroy(TRow *this) // clear memmory (row)
{
    for (int i = 0; i < this->len; ++i)
    {
        Cell_Destroy(this->data[i]);
    }
    free(this->data);
    free(this);
}

int Table_ParseReturnCount(char *rv) //parse table to rows
{
    int result = 0;
    strtok(rv, "\n");
    do
    {
        result++;
    } while (strtok(NULL, "\n") != NULL);
    return result;
}

TTable *Table_Create(char const *rv, TDelim *delim) // create table  ( rows and cells in rows)
{
    TTable *this;
    if (rv == NULL) // if nothing to read -> create default...
    {
        this = (TTable *)malloc(sizeof(TTable));
        this->len = 1;
        this->data = (TRow **)malloc(this->len * sizeof(TRow *));
        this->data[0] = Row_Create(NULL, delim);
        return this;
    }
    int buf_len = strlen(rv);
    char *buf = (char *)malloc((buf_len + 1) * sizeof(char));
    strcpy(buf, rv);
    if (buf[buf_len - 1] != '\n')
    {
        strcat(buf + buf_len, "\n");
    }
    this = (TTable *)malloc(sizeof(TTable));
    this->len = Table_ParseReturnCount(buf);
    this->data = (TRow **)malloc(this->len * sizeof(TRow *));
    char *temp = buf;
    int max_len = 0;
    for (int i = 0; i < this->len; ++i)
    {
        this->data[i] = Row_Create(temp, delim);
        if (Row_Length(this->data[i]) > max_len)
        {
            max_len = Row_Length(this->data[i]);
        }
        temp += strlen(temp) + 1;
    }
    for (int i = 0; i < this->len; ++i)
    {
        int starting_len = Row_Length(this->data[i]);
        for (int j = 0; j < max_len - starting_len; ++j)
        {
            Row_Insert(this->data[i], starting_len + j, NULL);
        }
    }
    free(buf);
    return this;
}

int Table_Height(TTable *this) // rows count
{
    return this->len;
}

int Table_Width(TTable *this) // columns count or cells in the row
{
    if (Table_Height(this) == 0)
    {
        return 0;
    }
    return Row_Length(this->data[0]);
}

TCell *Table_At(TTable *this, int r, int c)
{
    if (r < 0 || r >= this->len)
    {
        return NULL;
    }
    return Row_At(this->data[r], c);
}

int Table_InsertRow(TTable *this, int index)
{
    if (index < 0 || index > Table_Height(this))
    {
        return -1;
    }
    TRow *rv = Row_Create(NULL, NULL);
    this->len++;
    this->data = (TRow **)realloc(this->data, this->len * sizeof(TRow *));
    for (int i = this->len - 1; i > index; --i)
    {
        this->data[i] = this->data[i - 1];
    }
    this->data[index] = rv;
    return 0;
}

int Table_InsertCol(TTable *this, int index)
{
    if (index < 0 || index > Table_Width(this))
    {
        return -1;
    }
    for (int i = 0; i < Table_Height(this); ++i)
    {
        Row_Insert(this->data[i], index, NULL);
    }
    return 0;
}

int Table_RemoveRow(TTable *this, int index) // remove row dfrom the table
{
    if (Table_Height(this) == 1)
    {
        return 1;
    }
    if (index < 0 || index > Table_Height(this))
    {
        return -1;
    }
    Row_Destroy(this->data[index]);
    int temp_width = Row_Length(this->data[(index ? 0 : 1)]);
    for (int i = index; i < temp_width - 1; ++i)
    {
        this->data[i] = this->data[i + 1];
    }
    this->len--;
    this->data = (TRow **)realloc(this->data, this->len * sizeof(TRow *));
    return 0;
}

int Table_RemoveCol(TTable *this, int index) //remove column from the table
{
    if (Table_Width(this) == 1)
    {
        return 1;
    }
    if (index < 0 || index >= Table_Width(this))
    {
        return -1;
    }
    for (int i = 0; i < Table_Height(this); ++i)
    {
        Row_Remove(this->data[i], index);
    }
    return 0;
}

void Table_Destroy(TTable *this) // clear memmory (table)
{
    for (int i = 0; i < this->len; i++)
    {
        Row_Destroy(this->data[i]);
    }
    free(this->data);
    free(this);
}

TSelection *Selection_Create() // create selection of cells
{
    TSelection *this = (TSelection *)malloc(sizeof(TSelection));
    this->r1 = this->c1 = this->r2 = this->c2 = 0;
    return this;
}

int Selection_Set(TSelection *this, char const *rv, TTable *table) // set data in selected cells
{
    char *buf = (char *)malloc((strlen(rv) + 1) * sizeof(char));
    strcpy(buf, rv);
    char *token = strtok(buf, Default_SelDelim());
    if (!strcmp(token, "_"))
    {
        this->r1 = 0;
        this->r2 = Table_Height(table) - 1;
    }
    else
    {
        this->r1 = this->r2 = atoi(token);
    }
    token = strtok(NULL, Default_SelDelim());
    if (!strcmp(token, "_"))
    {
        this->c1 = 0;
        this->c2 = Table_Width(table) - 1;
    }
    else
    {
        this->c1 = this->c2 = atoi(token);
    }
    token = strtok(NULL, Default_SelDelim());
    if (token != NULL)
    {
        if (!strcmp(token, "-"))
        {
            this->r2 = Table_Height(table) - 1;
        }
        else
        {
            this->r2 = atoi(token);
        }
        token = strtok(NULL, Default_SelDelim());
        if (!strcmp(token, "-"))
        {
            this->c2 = Table_Width(table) - 1;
        }
        else
        {
            this->c2 = atoi(token);
        }
    }
    free(buf);
    return 0;
}

int Selection_IsLegal(TSelection *this, TTable *rv) // is it possible to sellect these cells
{
    int h = Table_Height(rv), w = Table_Width(rv);
    if (this->r1 < 0 || this->r1 >= h)
    {
        return 0;
    }
    if (this->r2 < 0 || this->r2 >= h)
    {
        return 0;
    }
    if (this->c1 < 0 || this->c1 >= w)
    {
        return 0;
    }
    if (this->c2 < 0 || this->c2 >= w)
    {
        return 0;
    }
    if (this->r1 > this->r2)
    {
        return 0;
    }
    if (this->c1 > this->c2)
    {
        return 0;
    }
    return 1;
}

void Selection_Destroy(TSelection *this) // clear memmory(selection)
{
    free(this);
}

TVariables *Variables_Create() // create variables
{
    TVariables *this = (TVariables *)malloc(sizeof(TVariables));
    this->_var = Selection_Create();
    for (int i = 0; i < VARS_COUNT; ++i)
    {
        this->_vars[i] = Cell_Create(""); //create cells in selection
    }
    return this;
}

void Variables_Destroy(TVariables *this) // clear memmory ( variables)
{
    for (int i = 0; i < VARS_COUNT; ++i)
    {
        Cell_Destroy(this->_vars[i]);
    }
    Selection_Destroy(this->_var);
    free(this);
}

TIOmodule *IO_Create(char const *filename) // input output
{
    if (filename == NULL || strlen(filename) == 0)
    {
        filename = Default_Filename(); // tab.txt
    }
    TIOmodule *this = (TIOmodule *)malloc(sizeof(TIOmodule));
    this->file = NULL;
    this->filename = (char *)malloc((strlen(filename) + 1) * sizeof(char));
    this->state = 0;
    this->buffer = NULL;
    strcpy(this->filename, filename);
    return this;
}

char *IO_Read(TIOmodule *this) //read data from file
{
    this->file = fopen(this->filename, "r");
    if (this->file == NULL)
    {
        this->state = -1;
        return NULL;
    }
    this->state = 1;
    int file_len;
    for (file_len = 0; !feof(this->file); ++file_len, fgetc(this->file))
        ;
    rewind(this->file);
    this->buffer = (char *)malloc(file_len * sizeof(char));
    fread((void *)(this->buffer), sizeof(char), file_len - 1, this->file);
    this->buffer[file_len - 1] = 0;
    return this->buffer;
}

int IO_Write(TIOmodule *this, TTable *table, TDelim *delim) //whrite data to file
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

void IO_Destroy(TIOmodule *this) //clear data input output
{
    fclose(this->file);
    free(this->filename);
    free(this->buffer);
    free(this);
}

TMaster *Master_Create(char const *filename_table, char const *cmd_sequence, char const *delim_str) // here the commands start
{
    if (cmd_sequence == NULL)
    {
        return NULL;
    }
    TMaster *this = (TMaster *)malloc(sizeof(TMaster));
    this->table_file = IO_Create(filename_table);
    this->delim = Delim_Create(delim_str);
    this->current_selection = Selection_Create();
    this->variables = Variables_Create();
    this->table = Table_Create(IO_Read(this->table_file), this->delim);
    if (strstr(cmd_sequence, "-c") == cmd_sequence)
    {
        this->command_file = IO_Create(this->commands + 2);
        this->commands = IO_Read(this->command_file);
    }
    else
    {
        this->command_file = NULL;
        this->commands = (char *)malloc((strlen(cmd_sequence) + 1) * sizeof(char));
        strcpy(this->commands, cmd_sequence);
    }
    if (strpbrk(this->commands + strlen(this->commands) - 1, Default_CmdDelim()) == NULL)
    {
        this->commands = (char *)realloc(this->commands, strlen(this->commands) + 2);
        strcat(this->commands, ";");
    }
    this->current_command = this->commands;
    this->state = 1;
    return this;
}

int Master_Execute(TMaster *this)
{
    char *cmd = this->current_command;
    char *temp_str;
    TCell *temp_cell;
    TSelection *temp_sel;
    double temp_num;
    int temp_count;
    if (cmd == NULL) // if no commands -> return 0
    {
        return 0;
    }
    else if (!strcmp(cmd, "[min]"))
    {
        temp_sel = Selection_Create();
        temp_count = 0;
        for (int i = this->current_selection->r1; i <= this->current_selection->r2; ++i)
        {
            for (int j = this->current_selection->c1; j <= this->current_selection->c2; ++j)
            {
                temp_cell = Table_At(this->table, i, j);
                if (Cell_IsNumber(temp_cell))
                {
                    if (!temp_count || Cell_DataDouble(temp_cell) < temp_num)
                    {
                        temp_num = Cell_DataDouble(temp_cell);
                        temp_sel->r1 = temp_sel->r2 = i;
                        temp_sel->c1 = temp_sel->c2 = j;
                        temp_count = 1;
                    }
                }
            }
        }
        memcpy((void *)this->current_selection, (void *)temp_sel, sizeof(TSelection));
        Selection_Destroy(temp_sel);
    }
    else if (!strcmp(cmd, "[max]"))
    {
        temp_sel = Selection_Create();
        temp_count = 0;
        for (int i = this->current_selection->r1; i <= this->current_selection->r2; ++i)
        {
            for (int j = this->current_selection->c1; j <= this->current_selection->c2; ++j)
            {
                temp_cell = Table_At(this->table, i, j);
                if (Cell_IsNumber(temp_cell))
                {
                    if (!temp_count || Cell_DataDouble(temp_cell) > temp_num)
                    {
                        temp_num = Cell_DataDouble(temp_cell);
                        temp_sel->r1 = temp_sel->r2 = i;
                        temp_sel->c1 = temp_sel->c2 = j;
                        temp_count = 1;
                    }
                }
            }
        }
        memcpy((void *)this->current_selection, (void *)temp_sel, sizeof(TSelection));
        Selection_Destroy(temp_sel);
    }
    else if (!strcmp(cmd, "[_]"))
    {
        memcpy((void *)this->current_selection, (void *)this->variables->_var, sizeof(TSelection));
    }
    else if (!strcmp(cmd, "irow"))
    {
        Table_InsertRow(this->table, this->current_selection->r1);
    }
    else if (!strcmp(cmd, "arow"))
    {
        Table_InsertRow(this->table, this->current_selection->r2 + 1);
    }
    else if (!strcmp(cmd, "icol"))
    {
        Table_InsertCol(this->table, this->current_selection->c1);
    }
    else if (!strcmp(cmd, "acol"))
    {
        Table_InsertCol(this->table, this->current_selection->c2 + 1);
    }
    else if (!strcmp(cmd, "drow"))
    {
        for (int i = this->current_selection->r1, j = i; i <= this->current_selection->r2; ++i)
        {
            Table_RemoveRow(this->table, j);
        }
        this->current_selection->r2 = this->current_selection->r1;
        if (!Selection_IsLegal(this->current_selection, this->table))
        {
            this->current_selection->r1--;
            this->current_selection->r2--;
        }
    }
    else if (!strcmp(cmd, "dcol"))
    {
        for (int i = this->current_selection->c1, j = i; i <= this->current_selection->c2; ++i)
        {
            Table_RemoveCol(this->table, j);
        }
        this->current_selection->c2 = this->current_selection->c1;
        if (!Selection_IsLegal(this->current_selection, this->table))
        {
            this->current_selection->c1--;
            this->current_selection->c2--;
        }
    }
    else if (!strcmp(cmd, "clear"))
    {
        for (int i = this->current_selection->r1; i <= this->current_selection->r2; ++i)
        {
            for (int j = this->current_selection->c1; j <= this->current_selection->c2; ++j)
            {
                Cell_Clear(Table_At(this->table, i, j));
            }
        }
    }
    else if (!strcmp(cmd, "[set]"))
    {
        memcpy((void *)this->variables->_var, (void *)this->current_selection, sizeof(TSelection));
    }
    else if (strstr(cmd, "set") == cmd)
    {
        temp_cell = Cell_Create(strstr(cmd, " ") + 1);
        for (int i = this->current_selection->r1; i <= this->current_selection->r2; ++i)
        {
            for (int j = this->current_selection->c1; j <= this->current_selection->c2; ++j)
            {
                Cell_SetData(Table_At(this->table, i, j), Cell_DataString(temp_cell));
            }
        }
        Cell_Destroy(temp_cell);
    }
    else if (strstr(cmd, "swap") == cmd)
    {
        if (this->current_selection->r1 ==
                this->current_selection->r2 &&
            this->current_selection->c1 ==
                this->current_selection->c2)
        {
            temp_sel = Selection_Create();
            Selection_Set(temp_sel, strchr(cmd, '['), this->table);
            Cell_Swap(Table_At(this->table, temp_sel->r1, temp_sel->c1),
                      Table_At(this->table, this->current_selection->r1, this->current_selection->c1));
            Selection_Destroy(temp_sel);
        }
    }
    else if (strstr(cmd, "sum") == cmd)
    {
        temp_sel = Selection_Create();
        Selection_Set(temp_sel, strchr(cmd, '['), this->table);
        temp_num = 0;
        for (int i = this->current_selection->r1; i <= this->current_selection->r2; ++i)
        {
            for (int j = this->current_selection->c1; j <= this->current_selection->c2; ++j)
            {
                temp_cell = Table_At(this->table, i, j);
                if (Cell_IsNumber(temp_cell))
                {
                    temp_num += Cell_DataDouble(temp_cell);
                }
            }
        }
        Cell_SetDataDouble(Table_At(this->table, temp_sel->r1, temp_sel->c1), temp_num);
        Selection_Destroy(temp_sel);
    }
    else if (strstr(cmd, "avg") == cmd)
    {
        temp_sel = Selection_Create();
        Selection_Set(temp_sel, strchr(cmd, '['), this->table);
        temp_num = 0;
        temp_count = 0;
        for (int i = this->current_selection->r1; i <= this->current_selection->r2; ++i)
        {
            for (int j = this->current_selection->c1; j <= this->current_selection->c2; ++j)
            {
                temp_cell = Table_At(this->table, i, j);
                if (Cell_IsNumber(temp_cell))
                {
                    temp_num += Cell_DataDouble(temp_cell);
                    temp_count++;
                }
            }
        }
        Cell_SetDataDouble(Table_At(this->table, temp_sel->r1, temp_sel->c1), temp_num / temp_count);
        Selection_Destroy(temp_sel);
    }
    else if (strstr(cmd, "count") == cmd)
    {
        temp_sel = Selection_Create();
        Selection_Set(temp_sel, strchr(cmd, '['), this->table);
        temp_count = 0;
        for (int i = this->current_selection->r1; i <= this->current_selection->r2; ++i)
        {
            for (int j = this->current_selection->c1; j <= this->current_selection->c2; ++j)
            {
                temp_cell = Table_At(this->table, i, j);
                if (strlen(Cell_DataString(temp_cell)) > 0)
                {
                    temp_count++;
                }
            }
        }
        Cell_SetDataDouble(Table_At(this->table, temp_sel->r1, temp_sel->c1), (double)temp_count);
        Selection_Destroy(temp_sel);
    }
    else if (strstr(cmd, "len") == cmd)
    {
        temp_sel = Selection_Create();
        Selection_Set(temp_sel, strchr(cmd, '['), this->table);
        Cell_SetDataDouble(Table_At(this->table, temp_sel->r1, temp_sel->c1),
                           (double)strlen(Cell_DataString(Table_At(this->table,
                                                                   this->current_selection->r1, this->current_selection->c1))));
        Selection_Destroy(temp_sel);
    }
    else if (strstr(cmd, "def") == cmd)
    {
        for (int i = this->current_selection->r1; i <= this->current_selection->r2; ++i)
        {
            for (int j = this->current_selection->c1; j <= this->current_selection->c2; ++j)
            {
                Cell_Copy(Table_At(this->table, i, j), this->variables->_vars[strchr(cmd, '_')[1] - '0']);
            }
        }
    }
    else if (strstr(cmd, "use") == cmd)
    {
        Cell_Copy(this->variables->_vars[strchr(cmd, '_')[1] - '0'], Table_At(this->table, this->current_selection->r1, this->current_selection->c1));
    }
    else if (strstr(cmd, "inc") == cmd)
    {
        temp_cell = this->variables->_vars[strchr(cmd, '_')[1] - '0'];
        if (Cell_IsNumber(temp_cell))
        {
            temp_num = Cell_DataDouble(temp_cell) + 1;
        }
        else
        {
            temp_num = 1;
        }
        Cell_SetDataDouble(temp_cell, temp_num);
    }
    else if (strstr(cmd, "[find") == cmd)
    {
        temp_str = (char *)malloc(strlen(cmd) * sizeof(char));
        strncpy(temp_str, strchr(cmd, ' ') + 1, strchr(cmd, ']') - strchr(cmd, ' '));
        temp_cell = Cell_Create(temp_str);
        free(temp_str);
        temp_count = 0;
        for (int i = this->current_selection->r1; i <= this->current_selection->r2; ++i)
        {
            for (int j = this->current_selection->c1; j <= this->current_selection->c2; ++j)
            {
                if (!strcmp(Cell_DataString(Table_At(this->table, i, j)), Cell_DataString(temp_cell)))
                {
                    this->current_selection->r1 =
                        this->current_selection->r2 = i;
                    this->current_selection->c1 =
                        this->current_selection->c2 = j;
                    temp_count = 1;
                    break;
                }
            }
            if (temp_count)
            {
                break;
            }
        }
        Cell_Destroy(temp_cell);
    }
    else if (strstr(cmd, "[") == cmd)
    {
        Selection_Set(this->current_selection, cmd, this->table);
    }
    return 0;
}

int Master_ExecuteNext(TMaster *this)
{
    char *temp;
    if (this->state == 1)
    {
        temp = this->commands;
    }
    else
    {
        temp = NULL;
    }
    this->current_command = strtok(temp, Default_CmdDelim());
    this->state = 2;
    if (this->current_command == NULL)
    {
        this->state = 0;
    }
    else if (Master_Execute(this))
    {
        this->state = 0;
    }
    return this->state;
}

void Master_Destroy(TMaster *this) //clear memmory(master)
{
    if (this->command_file == NULL)
    {
        free(this->commands);
    }
    else
    {
        IO_Destroy(this->command_file);
    }
    Table_Destroy(this->table);
    Variables_Destroy(this->variables);
    Selection_Destroy(this->current_selection);
    Delim_Destroy(this->delim);
    //IO_Destroy(this->table_file);
    free(this);
}

void UI_Start(int argc, char **argv) // user interface
{
    char *filename;
    char *cmd_sequence;
    char *delim;
    int shear = 0;
    if (argc < 3)
    {
        return;
    }
    if (!strcmp(argv[1], "-d"))
    {
        if (argc < 5)
        {
            return;
        }
        delim = argv[2];
        shear = 2;
    }
    else
    {
        delim = NULL;
    }
    cmd_sequence = argv[1 + shear];
    filename = argv[2 + shear];
    TMaster *master = Master_Create(filename, cmd_sequence, delim);
    while (Master_ExecuteNext(master))
        ;
    IO_Write(master->table_file, master->table, master->delim);
    //printf("Maybe...\n");
    Master_Destroy(master);
}

int main(int argc, char *argv[])
{
    UI_Start(argc, argv);
    return 0;
}