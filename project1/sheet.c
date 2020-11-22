#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define MAX_ROWS 1003
#define MAX_COLUMNS 1003
#define MAX_STRINGLENGTH 100

struct Delim // struct for delim
{
    char *value;
    int length; // lenght
    /* data */
} delim;

int argc_g;
char **argv_g;
int currentCLArg = 0;

char table[MAX_ROWS][MAX_COLUMNS][MAX_STRINGLENGTH]; //max possible size of table
int rows, columns;

void initDelim() // function for delim initialization
{
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

//if it possible to read word from comand line
bool isCLFinished()
{
    bool result = currentCLArg < argc_g;
    // if (result == false)
    //     printf(" ERROR, %d index %d\n", argc_g, currentCLArg);
    return !result; //return
}

int readIntAttribute() // possibility to read argument (int type)
{
    currentCLArg++;
    if (isCLFinished())
        return -1;
    /**
     * 
     */
    char *stopped; //

    /**
     * whrite number from cl to buffer  
     * if error -> whrite smth to *stopped. 10 count system (Decade)
     */
    int buffer = (int)strtol(argv_g[currentCLArg], &stopped, 10);
    if (*stopped)
    {
        printf("\n Cant read attribute %s, one of possible reason is lack of null-char \n", argv_g[currentCLArg]);
        return 0;
    }
    return buffer;
}

char *readStrAttribute() // possibility to read argument from cl (string)
{
    currentCLArg++;
    if (isCLFinished())
    {
        printf("Cant read next word\n");
        return NULL;
    }

    return argv_g[currentCLArg];
}
// functions for "uprava tabulky"
void irow(int r) // add string before string r>0
{
    rows += 1;
    for (int i = rows; i >= r; i--) //running rows
    {
        for (int j = 0; j < columns; j++) //running collumns
        {
            strcpy(table[i + 1][j], table[i][j]); //copy to .....from
        }
    }

    for (int i = 0; i < columns; i++)
    {
        strcpy(table[r][i], "");
    }
}
void drow(int r) //delete string r>0
{
    rows -= 1;
    for (int i = r; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            strcpy(table[i][j], table[i + 1][j]);
        }
    }
}
void drows(int n, int m) //delete strings from n to m
{

    int k = m + 1;

    for (int i = 0; k < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            strcpy(table[i][j], table[k][j]);
        }
        k++;
    }
    rows = rows - (m - n + 1);
}

void icol(int c) //add column before column c
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = columns - 1; j >= c; j--)
        {
            strcpy(table[i][j + 1], table[i][j]);
        }
    }
    for (int i = 0; i < rows; i++)
    {
        strcpy(table[i][c], "");
    }
    columns += 1;
}

void dcol(int c) //delete columns c
{

    for (int i = 0; i < rows; i++)
    {
        for (int j = c; j < columns; j++)
        {
            strcpy(table[i][j - 1], table[i][j]);
        }
    }
    columns--;
}

/**
 * This function removes range of columns
 */
void dcols(int n, int m) //delete columns form n to m
{
    for (int i = 0; i < rows; i++)
    {
        int k = m + 1;

        for (int j = n; k < columns; j++)
        {
            strcpy(table[i][j], table[i][k]);
            k++;
        }
    }
    columns = columns - (m - n + 1);
}

void processEditCL() //
{
    bool nothingToRead = isCLFinished();
    bool changesApplied = false;
    /**
     * if string with cl arguments is over -> go away from this function
     */
    while (!nothingToRead && !changesApplied)
    {
        char *currentCommand = argv_g[currentCLArg]; //

        if (strcmp(currentCommand, "irow") == 0) //if current CL command is irow - do function irow etc...
        {                                        //
            int row = readIntAttribute();        //
            irow(row - 1);
            changesApplied = true;
        }
        if (strcmp(currentCommand, "arow") == 0) //  ĐĐ´ĐľŃŃ ĐżŃĐžĐ¸ŃŃĐžĐ´Đ¸Ń Đ˛ŃŃĐ°Đ˛ĐşĐ° ŃŃĐ´Đ° Đ˛ ĐşĐžĐ˝ĐľŃ
        {
            rows += 1;
            changesApplied = true;
        }
        if (strcmp(currentCommand, "drow") == 0) // ĐĄŃĐ°Đ˛Đ˝Đ¸Đ˛Đ°ĐľĐź ŃĐľĐşŃŃŃŃ ĐşĐžĐźĐ°Đ˝Đ´Ń Ń ĐžĐśĐ¸Đ´Đ°ĐľĐźĐžĐš, Đ˛ ŃĐťŃŃĐ°Đľ ŃĐ°Đ˛ĐľĐ˝ŃŃĐ˛Đ° - Đ˛ŃĐˇŃĐ˛Đ°ĐľĐź ĐžĐąŃĐ°ĐąĐžŃŃĐ¸Đş
        {
            int row = readIntAttribute();
            drow(row - 1);
            changesApplied = true;
        }
        if (strcmp(currentCommand, "drows") == 0)
        {
            int n = readIntAttribute();
            int m = readIntAttribute();
            if (m < n)
            {
                printf("Druhe cislo musi byt >= prvniho \n");
            }
            drows(n - 1, m - 1);
            changesApplied = true;
        }
        if (strcmp(currentCommand, "icol") == 0)
        {
            int c = readIntAttribute();
            icol(c - 1);
            changesApplied = true;
        }
        if (strcmp(currentCommand, "acol") == 0)
        {
            columns += 1;
            changesApplied = true;
        }
        if (strcmp(currentCommand, "dcol") == 0)
        {
            int row = readIntAttribute();
            dcol(row);
            changesApplied = true;
        }
        if (strcmp(currentCommand, "dcols") == 0)
        {
            int n = readIntAttribute();
            int m = readIntAttribute();
            if (m < n)
            {
                printf("Druhe cislo musi byt >= prvniho \n");
                return;
            }
            dcols(n - 1, m - 1);
            changesApplied = true;
        }

        if (!changesApplied)
            break; //

        changesApplied = false;
        currentCLArg++;
        nothingToRead = isCLFinished();
    }
}
//functions for "zpracovani dat"
double stringToDouble(char *str)
{
    char *errors;
    double output = strtod(str, &errors);
    return output;
}

void cset(int i, int c, char *str) //do buĹky ve sloupci C bude nastaven ĹetÄzec STR.
{
    strcpy(table[i][c], str);
}

void tableRound(int i, int c) //ve sloupci C zaokrouhlĂ­ ÄĂ­slo na celĂŠ
{
    char *endPTR;
    double cell = strtod(table[i][c], &endPTR);

    int cellTruncated = (int)cell;
    if (strcmp(endPTR, "") == 0)
    {
        if (cell - cellTruncated >= 0.5)
        {
            sprintf(table[i][c], "%d", cellTruncated + 1);
        }
        else
        {
            sprintf(table[i][c], "%d", cellTruncated);
        }
    }
}

void tableTolower(int i, int c) //ĹetÄzec ve sloupci C bude pĹeveden na malĂĄ pĂ­smena
{
    for (int j = 0; j < MAX_STRINGLENGTH; j++)
    {
        table[i][c][j] = tolower(table[i][c][j]);
    }
}

void tableToupper(int i, int c) //ĹetÄzec ve sloupce C bude pĹeveden na velkĂĄ pĂ­smena
{
    for (int j = 0; j < MAX_STRINGLENGTH; j++)
    {
        table[i][c][j] = toupper(table[i][c][j]); //idk if i should to use asci table , but we ve got this function in ctype.h
    }
}

void tableCopy(int i, int n, int m) //pĹepĂ­ĹĄe obsah bunÄk ve sloupci M hodnotami ze sloupce N
{
    strcpy(table[i][m], table[i][n]);
}

void tableInt(int i, int c) //odstranĂ­ desetinnou ÄĂĄst ÄĂ­sla ve sloupci C
{

    char *endPTR;

    double buffer = strtod(table[i][c], &endPTR);
    if (strcmp(endPTR, "") == 0)
    {
        sprintf(table[i][c], "%d", (int)buffer);
    }
}
void tableMove(int i, int n, int m) //pĹesune sloupec N pĹed sloupec M.
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

void tableSwap(int i, int n, int m) //zamÄnĂ­ hodnoty bunÄk ve sloupcĂ­ch N a M.
{
    char buffer[MAX_STRINGLENGTH];

    strcpy(buffer, table[i][n]);
    strcpy(table[i][n], table[i][m]);
    strcpy(table[i][m], buffer);
}

bool processDataCL()
{

    if (isCLFinished())
        return false;

    char *currentCommand = argv_g[currentCLArg];
    // Proccess if we have `rows` or `beginswith` or `contains`

    int startIndex = 0, endIndex = rows;
    int beginC = -1;
    char *beginSTR;

    int containsC = -1;
    char *containsSTR;
    // functions for "selekce radku"
    if (strcmp(currentCommand, "rows") == 0)
    {
        if (currentCLArg + 1 < argc_g - 1 && strcmp(argv_g[currentCLArg + 1], "-") == 0)
        {
            startIndex = rows;

            currentCLArg++;
        }
        else
        {
            startIndex = readIntAttribute();
        }
        if (currentCLArg + 1 < argc_g - 1 && strcmp(argv_g[currentCLArg + 1], "-") == 0)
        {
            endIndex = rows;
            currentCLArg++;
        }
        else
        {
            endIndex = readIntAttribute();
        }
        startIndex--;
        currentCLArg++;
        currentCommand = argv_g[currentCLArg];
    }

    if (strcmp(currentCommand, "beginswith") == 0)
    {
        beginC = readIntAttribute() - 1;
        beginSTR = readStrAttribute();

        currentCLArg++;
        currentCommand = argv_g[currentCLArg];
    }

    if (strcmp(currentCommand, "contains") == 0)
    {
        containsC = readIntAttribute() - 1;
        containsSTR = readStrAttribute();

        currentCLArg++;
        currentCommand = argv_g[currentCLArg];
    }

    // Here the commands start
    int c, n, m;
    char *str;

    bool firstRead = true;

    for (int i = startIndex; i < endIndex; i++)
    {

        if (beginC != -1)
        {
            // Here you get the position number for the substring in the string. if in positionOfBEginStr goes 0 - than work with this table string
            // else skip the loop(cyklus) iteration
            char positionOfBeginStr = strstr(table[i][beginC], beginSTR) - table[i][beginC];

            if (positionOfBeginStr != 0)
                continue;
        }

        if (containsC != -1)
        {
            char *exists = strstr(table[i][containsC], containsSTR);
            if (exists == NULL)
                continue;
        }

        if (strcmp(currentCommand, "cset") == 0)
        {
            if (firstRead)
            {
                c = readIntAttribute();
                str = readStrAttribute();
            }

            cset(i, c - 1, str);
            firstRead = false;
        }

        if (strcmp(currentCommand, "tolower") == 0)
        {
            if (firstRead)
                c = readIntAttribute();

            tableTolower(i, c - 1);
            firstRead = false;
        }

        if (strcmp(currentCommand, "toupper") == 0)
        {
            if (firstRead)
                c = readIntAttribute();

            tableToupper(i, c - 1);
            firstRead = false;
        }

        if (strcmp(currentCommand, "round") == 0)
        {
            if (firstRead)
                c = readIntAttribute();

            tableRound(i, c - 1);
            firstRead = false;
        }

        if (strcmp(currentCommand, "swap") == 0)
        {
            if (firstRead)
            {
                n = readIntAttribute();
                m = readIntAttribute();
            }
            tableSwap(i, n - 1, m - 1);
            firstRead = false;
        }
        if (strcmp(currentCommand, "move") == 0)
        {
            if (firstRead)
            {
                n = readIntAttribute();
                m = readIntAttribute();
            }
            tableMove(i, n, m);
            firstRead = false;
        }

        if (strcmp(currentCommand, "copy") == 0)
        {
            if (firstRead)
            {
                n = readIntAttribute();
                m = readIntAttribute();
            }
            tableCopy(i, n - 1, m - 1);
            firstRead = false;
        }
        if (strcmp(currentCommand, "int") == 0)
        {
            if (firstRead)
                c = readIntAttribute();

            tableInt(i, c - 1);
            firstRead = false;
        }
    }

    return false;
}

// Determining the position of the character in the string, for the string ABC, the character " b " will be 2
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

void readTable()
{
    char row[10240];

    int i;
    while (fgets(row, 10240, stdin))
    {
        int nPos = strlen(row) - 1;
        if (strstr(row, "\n") != NULL)
        {
            row[nPos] = '\0';
        }

        char *bufferString;
        char *bufferStringPointer = row;

        while ((bufferString = strsep(&bufferStringPointer, delim.value)) != NULL)
        {
            if (rows == 0)
            {
                columns++;
            }

            strcpy(table[rows][i], bufferString);
            i++;
        }
        rows++;

        i = 0;
    }
}

void writeTable() //write sells to the table
{
    int i;
    for (i = 0; i < rows; i++)
    {
        int j = 0;
        for (; j < columns; j++)
        {
            printf("%s", table[i][j]);
            if (j < columns - 1)
                printf("%c", delim.value[0]);
        }
        if (i < rows - 1)
            printf("\n");
    }
}

int main(int argc, char *argv[])
{
    argc_g = argc; // pro zjednoduseni funkci,kde to potrebujeme
    argv_g = argv;

    initDelim(argc, argv);
    char uniqueDelim[MAX_STRINGLENGTH] = "";
    if (delim.length > 1)
    {
        uniqCharacters(delim.value, uniqueDelim);
        strcpy(delim.value, uniqueDelim);
    }

    readTable();

    processEditCL();
    bool isPossibleFurther = processDataCL();

    while (isPossibleFurther)
    {
        isPossibleFurther = processDataCL();
    }
    writeTable();
    return 0;
}