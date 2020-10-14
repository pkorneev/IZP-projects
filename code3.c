#include <stdio.h>
#include <stdlib.h>

int main(){
    /*int cis=0;
    int i=0; 
    int lenth = 0;
    int alpha = 0;
    char arr[101];*/
/*printf (" zadej retezec \n");
scanf ("%100s", arr);
while  (arr[i] != '\0'){
    lenth++;
    i++;
}
i = 0;
while (arr[i] != '\0' ) {
    if (arr[i]>= '0' && arr[i]<= '9'){
    cis ++;
    }
    if ((arr[i] >= 'a'&&  arr[i]<= 'z')||(arr[i]>= 'A' && arr[i]<= 'Z')){
        alpha++;

    }
    i++;
    

}*/
char arr[101];
printf ("zadej retezec\n");
scanf("%100s", &arr);


int i=0;
int diff = 'a'-'A';
while (arr[i] != '\0'){
    if (arr[i]>= 'a'&& arr[i] <= 'z'){
    arr[i]= arr[i]- diff;
     }
i++;
}
printf ("Velka pismena %s\n", arr);
/*
printf ("zadal jsi %s\n",arr);
printf ("delka je %d\n", lenth );
printf ("cisel jsou : %d\n", cis);
printf ("pismen jsou : %d\n", alpha);*/
}