
#include <stdio.h>

void fce(double *x, double y)
{
    double tmp;
    tmp = *x;
    *x = y;
    y = tmp;
    return;
}
int main(void)
{
    double a = 15.0, b = -16.0;

    fce(&a, b); //xx
    printf("%f    %f\n", a, b);
    return 0;
}
