#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define max_rows_count 1003
#define max_columns_count 1003
#define max_symbols_in_string 100

// Whether the function "beginswith" is present? Initially not, but in the function "get_selector" here will be put true, if is present.

bool if_begins_with = false;
int begins_with_col;
char *begins_with_str;

// Whether the function "contains" is present? Initially not, but in the function "get_selector" here will be put true, if is present.
bool if_contains = false;
int contains_col;
char *contains_str;

char table[max_rows_count][max_columns_count][max_symbols_in_string];

int left_side = 0; // Variables to work with the commands, where are 2 argumetns(n,m).
int right_side = 0;

int current_rows_count;
int current_columns_count;

int argc_g;
char **argv_g;

char *delim = " "; // The given delimiter.

int pos_in_cl = 0; // Starting position in command line.

bool last_pos() // Definition of the last position of the argument in the command line.
{
    bool result = (argc_g - 1) <= pos_in_cl; // If length oh the given string is 1 less, than the last position in command line.
    return result;
}

char *strsep(char **stringp, const char *delim) // The function for scanning for empty strings as well as multiple strings.
{
    char *begin, *end;
    begin = *stringp;
    if (begin == NULL)
        return NULL;
    //Find the end of the token.
    end = begin + strcspn(begin, delim); // To "end" adds value of "begin" without delimiter.
    if (*end)                            // If "end" != NULL.
    {
        // Terminate the token and set *STRINGP past NUL character.
        *end++ = '\0';
        *stringp = end;
    }
    else
        //No more delimiters; this is the last token.
        *stringp = NULL;
    return begin;
}

void remove_char_from_string(char *src, char *dest, char to_remove) // The function for removing a sign from string (parameters: source, destination).
{
    int index_to_add = 0;

    for (int i = 0; i < (int)strlen(src); i++) // While a sign under the current index is less than the length of string, index is incremented by 1.
    {
        if (src[i] != to_remove) // If a value under current index is not equal to a value of variable to_remove, then equate.
        {
            dest[index_to_add] = src[i];
            index_to_add++;
        }
    }
}

int get_int_com_arg(char *if_error_message) //Gets argument of command in the command line.
{
    if (last_pos()) //If is true, then :
    {
        printf("%s", if_error_message); //Notifies of errors.
        return 0;
    }
    pos_in_cl++;
    char *com_arg = argv_g[pos_in_cl]; // Variable is an argument of the table operation command.
    if (strcmp(com_arg, "-") == 0)     // For checking if argument of "rows" command is "-"
        return current_rows_count;
    char *errors;                               // String, where will be written errors by type coercion.
    int output = (int)strtod(com_arg, &errors); // Writes the result of type coercion in variable.
    if (strlen(errors))                         // Checks on presence of errors.
    {
        printf("Wrong command line argument! Expected: number, recieved : %s\n", com_arg);
        printf("%s", if_error_message);
    }
    return output;
}

char *get_string_com_arg() // Gets an introductory string.
{
    if (last_pos())
    {
        printf("%s", "Сan't read string argument from command line");
        return 0;
    }
    pos_in_cl++;
    return argv_g[pos_in_cl];
}

void irow(int r) // Inserts a row before a row number "r".
{
    current_rows_count++;

    for (int i = current_rows_count; i >= r; i--) // Goes through the rows of the table, starting from the end.
    {
        for (int j = 0; j < current_columns_count; j++) // Goes though the columns, starting from the beginning.
        {
            strcpy(table[i][j], table[i - 1][j]);
        }
    }

    for (int j = 0; j < current_columns_count; j++)
    {
        strcpy(table[r - 1][j], "");
    }
}

void drow(int r) // Removes row number "r".
{
    for (int i = r; i < current_rows_count; i++)
    {
        for (int j = 0; j < current_columns_count; j++)
        {
            strcpy(table[i - 1][j], table[i][j]);
        }
    }
    current_rows_count--;
}

void drows(int n, int m) // Removes rows from row's number "n" to "m".
{
    m--;
    n--;
    int to_delete = m - n + 1; // The quantity of rows, how many have to be removed.

    for (int i = n; i < current_rows_count - to_delete; i++)
    {
        for (int j = 0; j < current_columns_count; j++)
        {
            strcpy(table[i][j], table[i + to_delete][j]);
        }
    }
    current_rows_count -= to_delete;
}

void arow() // Adds an empty row to the end of the table.
{
    current_rows_count += 1;
}

void acol() // Adds an empty column to the end of the table.
{
    current_columns_count += 1;
}

void dcol(int c) // Removes column number "c".
{
    for (int i = 0; i < current_rows_count; i++)
    {
        for (int j = c; j < current_columns_count; j++)
        {
            strcpy(table[i][j - 1], table[i][j]);
        }
    }

    current_columns_count--;
}

void icol(int c) // Inserts an empty column before column number "c".
{
    for (int i = 0; i < current_rows_count; i++)
    {
        for (int j = current_columns_count; j >= c; j--) // Goes through the columns, starting from the end, ending in the column number "c".
        {
            strcpy(table[i][j], table[i][j - 1]);
        }
    }

    for (int i = 0; i < current_rows_count; i++)
    {
        strcpy(table[i][c - 1], "");
    }
    current_columns_count++;
}

void dcols(int n, int m) // Removes columns from the column "n" to the column "m".
{
    m--;
    n--;
    int to_delete = m - n + 1; //The quantity of columns, that will be removed.

    for (int i = 0; i < current_rows_count; i++)
    {
        for (int j = n; j < current_columns_count; j++)
        {
            strcpy(table[i][j], table[i][j + to_delete]);
        }
    }
    current_columns_count -= to_delete;
}

void to_lower(int c) // Converts value in column C to lowercase.
{
    c--;
    for (int i = left_side; i < right_side; i++)
    { // Checks if the input contains "beginswith", then checks whether the string in the specified column starts with the specified character. Same way with the "contains".
        if ((!if_begins_with || strstr(table[i][begins_with_col], begins_with_str) == table[i][begins_with_col]) && (!if_contains || strstr(table[i][contains_col], contains_str) != NULL))
        {

            for (int j = 0; j < 100; j++)
            {
                table[i][c][j] = tolower(table[i][c][j]);
            }
        }
    }
}

void to_upper(int c) // Converts value in the column to the uppercase.
{
    c--;
    for (int i = left_side; i < right_side; i++)
    {
        if ((!if_begins_with || strstr(table[i][begins_with_col], begins_with_str) == table[i][begins_with_col]) && (!if_contains || strstr(table[i][contains_col], contains_str) != NULL))
        {

            for (int j = 0; j < 100; j++)
            {
                table[i][c][j] = toupper(table[i][c][j]);
            }
        }
    }
}

void cset(int c, char *str) // The string will be set to the cell in the column "c".
{
    c--;
    for (int i = left_side; i < right_side; i++)
    {
        if ((!if_begins_with || strstr(table[i][begins_with_col], begins_with_str) == table[i][begins_with_col]) && ((!if_contains || strstr(table[i][contains_col], contains_str) != NULL)))
        {
            strcpy(table[i][c], str);
        }
    }
}

void to_round(int c) // Rounds the cell values in the specified column.
{
    c--;

    for (int i = left_side; i < right_side; i++)
    {
        if ((!if_begins_with || strstr(table[i][begins_with_col], begins_with_str) == table[i][begins_with_col]) &&
            (!if_contains || strstr(table[i][contains_col], contains_str) != NULL))
        {
            char *endPTR;
            double chislo_in_cell = strtod(table[i][c], &endPTR);

            if (strcmp(endPTR, "") == 0)
            {
                if ((int)chislo_in_cell + 1 - chislo_in_cell <= 0.5)
                {
                    sprintf(table[i][c], "%d", (int)chislo_in_cell + 1);
                }
                else
                {
                    sprintf(table[i][c], "%d", (int)chislo_in_cell);
                }
            }
        }
    }
}
void to_swap(int n, int m) // Swaps columns "n" and "m".
{
    n--;
    m--;
    char buffer[100];

    for (int i = left_side; i < right_side; i++)
    {
        if ((!if_begins_with || strstr(table[i][begins_with_col], begins_with_str) == table[i][begins_with_col]) && (!if_contains || strstr(table[i][contains_col], contains_str) != NULL)) //если на вводе есть beginswith, тогда проверяем, что строка в заданной колонке начинается с заданного символа
        {
            strcpy(buffer, table[i][n]);
            strcpy(table[i][n], table[i][m]);
            strcpy(table[i][m], buffer);
        }
    }
}

void to_copy(int n, int m) // Copies values from cells of column number "n" to the column "m".
{
    n--;
    m--;

    for (int i = left_side; i < right_side; i++)
    {
        if ((!if_begins_with || strstr(table[i][begins_with_col], begins_with_str) == table[i][begins_with_col]) && (!if_contains || strstr(table[i][contains_col], contains_str) != NULL))
        {
            if (n > m)
            {
                strcpy(table[i][m], table[i][n]);
            }
            else
                strcpy(table[i][n], table[i][m]);
        }
    }
}

void to_move(int n, int m) // Moves column "n" before the column "m".
{
    for (int i = left_side; i < right_side; i++)
    { // If the input has "beginswith", then checks, if the string in the specified column begins with a specified sign. Same way with the "contains".
        if ((!if_begins_with || strstr(table[i][begins_with_col], begins_with_str) == table[i][begins_with_col]) &&
            (!if_contains || strstr(table[i][contains_col], contains_str) != NULL))
        {
            char buffer[100];
            strcpy(buffer, table[i][n - 1]);
            if (n <= m)
            {
                for (int j = n - 1; j < m - 1; j++)
                {
                    strcpy(table[i][j], table[i][j + 1]);
                }
                strcpy(table[i][m - 2], buffer);
            }
            else
            {
                for (int j = n - 1; j > m - 1; j--)
                {
                    strcpy(table[i][j], table[i][j - 1]);
                }
                strcpy(table[i][m - 1], buffer);
            }
        }
    }
}

void to_int(int c) // Deletes the decimal part of a number in the column "c".
{
    c--;

    for (int i = left_side; i < right_side; i++) // Left_side is starting string, from which starts processing. If "rows" is specified - then it is the 1st number after command "rows". Otherwise is 0.
    {
        if (
            // If the string starts with the argument "beginswith" OR "beginswith" is NOT required at all then :
            (!if_begins_with || strstr(table[i][begins_with_col], begins_with_str) == table[i][begins_with_col]) &&
            // If the string contains the argument "contains" OR "contains" is NOT required at all, then :
            (!if_contains || strstr(table[i][contains_col], contains_str) != NULL)) // If the input has "beginswith, then checks, if string in the specified column begins with a specified sign.
        {
            char *fragment = strtok(table[i][c], ".");
            char *end_ptr;
            strtol(fragment, &end_ptr, 10);
            if (fragment && strcmp(end_ptr, "") == 0)
            {
                strcpy(table[i][c], fragment);
            }
        }
    }
}

void get_cells_changes() // The function, which operates with cells in the table.
{
    if (last_pos())
        return;

    pos_in_cl++;

    char *name = argv_g[pos_in_cl]; // Creates an array, in which will be put the command, written by user.

    if (strcmp("cset", name) == 0) // Compares entered command wuth the command, that exists.
    {
        int c = get_int_com_arg("Expected number of line to put line before\n");
        char *str = get_string_com_arg();
        cset(c, str);
    }
    if (strcmp("tolower", name) == 0)
    {
        int c = get_int_com_arg("Expected number of column to lower value\n");
        to_lower(c);
    }
    if (strcmp("toupper", name) == 0)
    {

        int c = get_int_com_arg("Expected number of column to upper value\n");
        to_upper(c);
    }
    if (strcmp("round", name) == 0)
    {
        int c = get_int_com_arg("Expected number of column to round value\n");

        to_round(c);
    }

    if (strcmp("swap", name) == 0)
    {
        int n = get_int_com_arg("Expected number of column to swap with\n");
        int m = get_int_com_arg("Expected number of column to swap with\n");
        to_swap(n, m);
    }
    if (strcmp("copy", name) == 0)
    {
        int n = get_int_com_arg("Expected number of column to copy value from.\n");
        int m = get_int_com_arg("Expected number of column to copy value to.\n");
        to_copy(n, m);
    }
    if (strcmp("move", name) == 0)
    {
        int n = get_int_com_arg("Expected number of column to move.\n");
        int m = get_int_com_arg("Expected number of column to move before.\n");
        to_move(n, m);
    }
    if (strcmp("int", name) == 0)
    {
        int c = get_int_com_arg("Expected number of column to int\n");
        to_int(c);
    }
}

void get_selector() // The function, that detects the selector.
{

    if (last_pos())
        return;

    bool selector_is_found = false;
    pos_in_cl++;

    char *name = argv_g[pos_in_cl];

    if (strcmp("rows", name) == 0)
    {
        int n = get_int_com_arg("Expected number of line.\n");
        int m = get_int_com_arg("Expected number of line.\n");
        if (n > m)
        {
            printf("Wrong input of rows numbers!\n Left side should be less or equal.");
        }
        else
        {
            left_side = n - 1;
            right_side = m;
        }
        selector_is_found = true;
    }

    if (strcmp("beginswith", name) == 0)
    {
        if_begins_with = true;
        begins_with_col = get_int_com_arg("") - 1;
        begins_with_str = get_string_com_arg();
        selector_is_found = true;
    }
    if (strcmp("contains", name) == 0)
    {
        if_contains = true;
        contains_col = get_int_com_arg("") - 1;
        contains_str = get_string_com_arg();
        selector_is_found = true;
    }

    if (!selector_is_found)
        pos_in_cl--;

    get_cells_changes();
}

bool get_table_structure_changes() // The function, that operates with the table changes (changes rows and columns).
{
    if (last_pos())
        return false;

    pos_in_cl++;

    char *name = argv_g[pos_in_cl]; // Gets the string from the command line.

    if (strcmp("irow", name) == 0)
    {

        int r = get_int_com_arg("Expected number of line to put line before.\n");
        irow(r);
        return true;
    }
    if (strcmp("arow", name) == 0)
    {
        arow();
        return true;
    }
    if (strcmp("drow", name) == 0)
    {
        int r = get_int_com_arg("Expected number of line to remove.\n");
        drow(r);
        return true;
    }
    if (strcmp("icol", name) == 0)
    {
        int c = get_int_com_arg("Expected number of line to put column before.\n");
        icol(c);
        return true;
    }

    if (strcmp("drows", name) == 0)
    {
        int n = get_int_com_arg("Expected number of rows to remove.\n");
        int m = get_int_com_arg("Expected number of rows to remove.\n");
        if (n <= m)
        {
            drows(n, m);
        }
        else
        {
            return false;
        }
        return true;
    }

    if (strcmp("icol", name) == 0)
    {
        int c = get_int_com_arg("Expected number of column to insert a new one before.\n");
        icol(c);
        return true;
    }

    if (strcmp("acol", name) == 0)
    {
        acol();
        return true;
    }
    if (strcmp("dcol", name) == 0)
    {
        int c = get_int_com_arg("Expected number of column to remove.\n");
        dcol(c);
        return true;
    }
    if (strcmp("dcols", name) == 0)
    {
        int n = get_int_com_arg("Expected number of columns to remove.\n");
        int m = get_int_com_arg("Expected number of columns to remove.\n");
        if (n <= m)
        {
            dcols(n, m);
            return true;
        }
        else
            return false;
    }
    pos_in_cl--;

    return false;
}

int char_index(char *haystack, char needle) // Determining the index of a character in the string.
{
    for (int i = 0; i < (int)strlen(haystack); i++) // Goes through the string, beginning with the index 0.
    {
        if (
        int nPos = strlen(row) - 1;
        //это использовать для убирания символов, вместо \n  проверяем на  \, ", ", ', ' и если найдены, заменяем на пустой символ (а не на \0 как тут). пустой символ это ''
        if (strstr(row, "\n") != NULL)
        {haystack[i] == needle) // If in the string has what we "need, then :     (For example, "abv" - need "b" -> index [1])
            return i;
    }

    return -1; // In case, it did not find the desired one.
}

void only_unique_chars(char *string, char *unique_string) // Converts a given string to the unique one -> removing duplicate characters.
{
    int length = strlen(string);
    int iterator_for_add = 0; // The index for adding characters to the resulting string is needed because it already has some characters present, and if user uses "1" to specify the position of the character in the resulting line, then there will be spaces in the string.

    for (int i = 0; i < length; i++)
    {
        char current_char = string[i];                                    // Writes a letter to a variable, which will check for uniqueness.
        int position_in_unique = char_index(unique_string, current_char); // Assign the function value to a variable.

        if (position_in_unique == -1)
        {
            unique_string[iterator_for_add] = current_char;
            // If this letter is added to the resulting string, then the index for the next one to be added is increased by 1. In this case, all the letters will be added in a row without spaces.
            iterator_for_add++;
        }
    }
}

bool get_delim() // Checks for availability of the delimiter.
{
    if (argc_g < 3) // If is less than 3 signs in the given string, then :
        return false;

    bool exists = strcmp(argv_g[1], "-d") == 0; // The function checks for availibility of the "-d" (delimiter) in the string.
    if (exists)
    {
        pos_in_cl += 2;
        delim = argv_g[2]; // Assigns the delimiter to the 2. place in the array.
    }
    return exists;
}

void read_table() // The function for the input of the table.
{
    char buffer[max_symbols_in_string * max_columns_count];
    int i = 0;
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) // Reads the string from the buffer where the source file is located, until comes across the end of the string.
    {
        int j = 0;
        char *cell;
        char *pointer_of_buffer = buffer;
        while ((cell = strsep(&pointer_of_buffer, delim)) != NULL) // The function "strsep" splits the source string (buffer) to the cells, separating then usind the delimiter.
        {
            // Copies to the table the cell value extracted from the row, that was split with the delimiter above in "strsep".
            char safe_string[max_symbols_in_string] = "";     // Creates an array, that has an empty value and where will put "\n".
            remove_char_from_string(cell, safe_string, '\n'); // The funciton deletes sign "\n" from the cell.
            strcpy(table[i][j], safe_string);
            j++;
        } // ----

        if (i == 0)
        {
            current_columns_count = j;
        }
        i++;
    }

    current_rows_count = i;
    right_side = current_rows_count;
}

void write_table() // The function for outputting the table.
{
    for (int i = 0; i < current_rows_count; i++)
    {
        for (int j = 0; j < current_columns_count; j++)
        {
            printf("%s", table[i][j]);
            if (j < current_columns_count - 1)
            {
                printf("%c", delim[0]);
            }
        }

        if (i < current_rows_count - 1)
            printf("\n");
    }
}

int main(int argc, char *argv[]) // The main function.
{
    argc_g = argc;
    argv_g = argv;
    get_delim(); // Assigning the function value to the identifier to determine whether there is a delimiter.

    char buffer_delim[100]; // Creates an array, in which will be put the delimiter after its processing; when it contains only unique characters.
    if (strcmp(delim, " ") != 0)
    {
        only_unique_chars(delim, buffer_delim);
        strcpy(delim, buffer_delim);
    }

    read_table();

    bool if_structure_changed = get_table_structure_changes();
    while (if_structure_changed)
        if_structure_changed = get_table_structure_changes();

    get_selector();
    write_table();
}