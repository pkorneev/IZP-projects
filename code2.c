#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_sum1(int arr1[], int length)
{
    int x = 0;
    int s = 0;
    int sum_arr = 0;
    while (x < 5)
    {
        s += arr1[x];
        x++;
    }
    return s;
}
int get_sum2(int arr2[], int length)
{
    int x = 0;
    int s = 0;
    int sum_arr = 0;
    while (x < 5)
    {
        s += arr2[x];
        x++;
    }
    return s;
}

int main()
{
    int arr1[5];
    int arr2[5];
    printf("zadej 5 cisel\n");

    int i = 0;
    while (i < 5)
    {
        scanf("%d", &arr1[i]);
        i++;
    }
    printf("zadej jeste 5 cisel\n");

    int x = 0;
    while (x < 5)
    {
        scanf("%d", &arr2[x]);
        x++;
    }
    int sum1 = get_sum1(arr1, 5);
    int sum2 = get_sum2(arr2, 5);
    int suma = sum1 + sum2;
    printf("soucet je %d\n", suma);

    return 0;
}