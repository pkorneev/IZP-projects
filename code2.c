#include <stdio.h>
#include <stdlib.h>

int main(){
    int num[5];
   printf ("zadej 5 cisel\n");
    int i = 0;
    while (i<5){
        scanf("%d", &num [i]);
        i++;
    }
    /*int prumer = (num[0]+num[1]+num[2]+num[3]+num[4])/5;
    printf("Prumer je %d\n", prumer);*/
    int sum = 0;
    i = 0;
    while (i<5){
        sum +=num[i];
        i++;
    }
    double avg = sum / 5.0 ;
    printf (" prumer je %0.2f\n",avg );

   
    return 0;

}