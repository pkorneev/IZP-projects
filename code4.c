#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
struct Delim // structura delim
{
    char *value;
    int length; // lenght
    /* data */
};

struct Delim delim;
int argc_g;
char **argv_g;
int currentCLArg = 0;

void initDelim() // function (void)
{
    delim.length = 1;
    delim.value = " ";

    //strcmp sravnenie strok return: -1/0/1
    //strlen dlina stroki
    int isFirstArgumentD = strcmp(argv_g[1], "-d");
    if (isFirstArgumentD == 0)
    {
        delim.value = argv_g[2];          // parametr delim value = argv[2]
        delim.length = strlen(argv_g[2]); // length = strlen (dlina) argv[2]
        currentCLArg += 3;                // dobavl9em 3
    }
    else
    {
        currentCLArg++; //dobavl9em 1
    }
}

//проверка что еще можем считать слово из аргументов командной строки
bool isCLFinished()
{
    bool result = currentCLArg < argc_g; // peremennaya result = currentCLArg < argc
    if (result == false)                 // jesli nepravda-to
        printf(" ERROR, %d index %d\n", argc_g, currentCLArg);
    return !result; //return obratniy result
}

int readIntAttribute()
{
    currentCLArg++;
    if (isCLFinished())
        return -1;
    /**
     * Если тут есть символы значит возникла ошибка
     */
    char *stopped; //stroka

    /**
     * Здесь мы записываем в переменную наше число из аргументов комнадной строки. 
     * Если возникла ошибка то в стоппед пишем что-то. 10 - означает десятеричную систему счиления
     */
    int buffer = (int)strtol(argv_g[currentCLArg], &stopped, 10);
    if (*stopped)
    {
        printf("\n Cant read attribute %s, one of possible reason is lack of null-char \n", argv_g[currentCLArg]);
        return false;
    }
    return buffer;
}

void processEditCL() // peremenna9a, kotoraja ni4ego ne vozvrashaet
{
    /**
     * Если закончилась строка с аргументами командной строки - просто выфди из этой функции 
     */
    if (isCLFinished())
        return;

    char *currentCommand = argv_g[currentCLArg]; //stroka currentcmd = massivu argv[currentCLArg]

    if (strcmp(currentCommand, "irow") == 0) //sravnenije strok strcmp
    {
        int row = readIntAttribute(); // peremennaya row
    }
    if (strcmp(currentCommand, "arow") == 0)
    {
    }
    if (strcmp(currentCommand, "drows") == 0)
    {
        int n = readIntAttribute();
        int m = readIntAttribute();
    }
}

// Определение позиции символа в строке, для строки абв символ "в" будет 2
int positionOfCharacterInString(char *haystack, char needle)
{
    int i = 0;
    while (i != strlen(haystack)) // while strlen of arr haystack wont be i, repeat
    {
        if (haystack[i] == needle)
        {
            return i;
        }
        i++;
    }
    return -1;
}

// Удаление повторяющихся элементов из строки
char *uniqCharacters(char *string)
{
    char *output = malloc(strlen(string)); // make a memory for (strlen(string))
    int indexatorOfOutput = 0;

    for (int i = 0; i < strlen(string); i++) // цикл сейчас и = 0, пока и < стрлен , и++
    {
        int positionInOutput = positionOfCharacterInString(output, string[i]);
        if (positionInOutput == -1)
        {
            output[indexatorOfOutput] = string[i];
            indexatorOfOutput++;
        }
    }

    return output;
}

int main(int argc, char *argv[])
{
    argc_g = argc; // novije globalnye peremennye
    argv_g = argv;

    printf("hello world\n");
    initDelim(argc, argv);
    printf("%s\n", delim.value);

    printf("Unique: %s\n", uniqCharacters(delim.value));

    printf("\n%d\n", positionOfCharacterInString("ffdpaabv", 'a'));

    processEditCL();
    return 0;
}
