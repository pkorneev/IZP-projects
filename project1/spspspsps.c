#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
int argc_g;
char **argv_g;
int currentCLArg = 0;
char ***table;

int rows = 0, columns = 0;

FILE *file;
struct Delim // struct for delim
{
    char *value;
    int length; // lenght
    /* data */
} delim;

int positionOfCharacterInString(char *haystack, char needle)
{

    int i = 0;
    while (i != (int)strlen(haystack)) // while strlen of arr haystack wont be i, repeat
    {
        if (haystack[i] == needle) //searching needle in the haystack :)
        {
            return i;
        }
        i++;
    }
    return -1;
}

// deleting repeting symbols in string
void uniqCharacters(char *string, char *output)
{
    int indexatorOfOutput = 0;

    for (int i = 0; i < (int)strlen(string); i++) //
    {
        int positionInOutput = positionOfCharacterInString(output, string[i]);
        if (positionInOutput == -1)
        {
            output[indexatorOfOutput] = string[i];
            indexatorOfOutput++;
        }
    }
}

void initDelim() // function for delim initialization
{
    // struct Delim *a = &delim;

    // char *u = a->value;

    delim.length = 1;
    delim.value = " ";

    //strcmp str compaier: -1/0/1
    //strlen length of string
    if (argc_g == 1)
        return;
    int isFirstArgumentD = strcmp(argv_g[1], "-d");
    if (isFirstArgumentD == 0)
    {
        delim.value = argv_g[2];          //
        delim.length = strlen(argv_g[2]); //
        currentCLArg += 3;                //
    }
    else
    {
        currentCLArg++; //next comand line argument
    }
}

char *strsep(char **stringp, const char *delim) //function for separating strings to substrings by delim
{
    char *begin, *end;
    begin = *stringp;
    if (begin == NULL)
        return NULL;
    end = begin + strcspn(begin, delim);
    if (*end)
    {
        *end++ = '\0';
        *stringp = end;
    }
    else
        *stringp = NULL;
    return begin;
}

char *replaceChar(char *str, char target, char replace)
{
    char *foundPosition = strchr(str, target);

    while (foundPosition)
    {
        *foundPosition = replace;
        foundPosition = strchr(str, target);
    }
    return str;
}

char *sanitizeDelim(char *delim)
{
    char *slashes = "\\";
    char *output = replaceChar(delim, '\\', '\0');

    output = replaceChar(output, '\"', '\0');
    output = replaceChar(output, '\'', '\0');

    return output;
}
bool isCLFinished()
{
    bool result = currentCLArg < argc_g;

    return !result; //return
}

void readTable(char *delim)
{
    char c = getc(file);
    int mostLong = 0;
    table = malloc(sizeof(char **));

    while (c != EOF)
    {
        int sizeOfCurrentRow = 0;
        if (c == '\n')
        {
            c = getc(file);
        }

        char *bufferrow = malloc(sizeof(char *));

        while (c != '\n' && c != EOF)
        {

            bufferrow = (char *)realloc(bufferrow, sizeOfCurrentRow + 1);
            bufferrow[sizeOfCurrentRow] = c;
            sizeOfCurrentRow++;

            c = getc(file);
        }
        rows++;
        table = realloc(table, rows * sizeof(char **));
        int currentBunka = 0;
        char *token = strsep(&bufferrow, delim);
        while (token != NULL)
        {
            if (rows == 1)
            {
                columns++;
            }
            table[rows - 1] = realloc(table[rows - 1], (currentBunka + 1) * sizeof(char *));
            table[rows - 1][currentBunka] = token;
            token = strsep(&bufferrow, delim);
            currentBunka++;
        }
        if (mostLong < currentBunka)
            mostLong = currentBunka;
        // Выравнивание должно быть тут
        free(bufferrow);
    }
}

void writeTable(char *delim) //write sells to the table
{
    int i;
    for (i = 0; i < rows; i++)
    {
        int j = 0;
        for (; j < columns; j++)
        {
            printf("%s", table[i][j]);
            if (j < columns - 1)
                printf("%c", delim[currentCLArg]);
        }
        if (i < rows - 1)
            printf("\n");
    }
}

int main(int argc, char *argv[])
{
    argc_g = argc;
    argv_g = argv;

    char *delim = sanitizeDelim(argv[2]);

    char *filename = argv[3];
    file = fopen(filename, "r");
    readTable(delim);
    writeTable(delim);

    return 0;
}