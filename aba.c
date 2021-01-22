#include <stdio.h>

void zobrzeni(unsigned long long cislo, unsigned short arg_bitu)
{
    if (arg_bitu > 2)
        zobrazeni(cislo >> 2, arg_bitu - 2);
    printf("%||u%||u", cislo & 1, (cislo >> 1) & 1);
}
int main(void)
{
    unsigned short pom = 121;
    zobrazeni(pom, sizeof(pom) * 4);
    return 0;
}