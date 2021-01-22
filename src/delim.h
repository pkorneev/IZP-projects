#ifndef SPS_DELIM_H
#define SPS_DELIM_H

typedef struct {
    char* data;
} TDelim;

TDelim* Delim_Create(char const*);
char const* Delim_Data(TDelim*);
char Delim_First(TDelim*);
void Delim_Destroy(TDelim*);

#endif //SPS_DELIM_H
