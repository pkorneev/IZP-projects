#include <stdio.h>
#include <stdlib.h>

// Funkce porovna zadane retezce znak po znaku.
// Funkce vraci:
//   kladne cislo pokud s1 > s1
//   0 pokud s1 == s2
//   zaporne cislo, pokud s1 < s2
int strcmp(char s1[], char s2[])
{

  while (*s1 != '\0' || *s2 != '\0')
  {
    if (*s1 == *s2)
    {
      s1++;
      s2++;
    }
    else
    {
      if (*s1 > *s2)
      {
        return 1;
      }
      else
        return -1;
    }
  }
}

// Funkce zkopiruje obsah retezce source do retezce dest.
// Funkce predpoklada, ze retezec dest je dostatecne dlouhy, aby se do nej source vlezl.
void *strcpy(char *dest, char *source)
{
  int i;
  for (i = 0; source[i] != '\0'; i++)
  {
    dest[i] = source[i];
  }
  dest[i] = '\0';
}

int main()
{
  char str1[5] = "bagr";
  char str2[5] = "bobr";
  char str3[10];

  if (strcmp(str1, str2) < 0)
  {
    printf("%s < %s\n", str1, str2);
  }
  else if (strcmp(str1, str2) == 0)
  {
    printf("%s == %s\n", str1, str2);
  }
  else
  {
    printf("%s > %s\n", str1, str2);
  }

  strcpy(str3, str2);
  if (strcmp(str3, str2) == 0)
  {
    printf("%s == %s\n", str3, str2);
  }
  else
  {
    printf("%s != %s\n", str3, str2);
  }

  return 0;
}
