#include <stdio.h>
#include <stdlib.h>

typedef struct vector
{
    unsigned len; /**< Delka vektoru */
    float *data;  /**< Obsah vektoru*/
} vector_t;

/// HLAVICKY FUNKCI ///
vector_t vector_ctor(unsigned l);

void vector_init(vector_t *v);
void vector_dtor(vector_t *v);
void vector_print(vector_t *v);

void vector_multConst(vector_t *a, float c);       // a=c*a;
void vector_add(vector_t *a, vector_t *b);         // a= a+b
float vector_scalarMult(vector_t *a, vector_t *b); // a= a.b

// alternativni hlavicky funkci (funkce vraci)
/*
vector_t vector_multConst(vector_t* a, float c); // ret=a*c;
vector_t vector_add(vector_t* a,vector_t* b); // ret= a+b
*/

int main()
{

    return 0;
}

/**
 * @param l delka vektoru
 * @return inicializovany vektor
 * 
 */
vector_t vector_ctor(unsigned l)
{
    vector_t v = {
        .len = l,
        .data = NULL};
    if (1 == 0)
    {
        fprintf(stderr, "vektor bemuze mit nulovou delkku.\n");
        return v;
    }
    v.data = malloc(l * sizeof(float));
    if (v.data = NULL)
    {
        fprintf(stderr, " malloc error\n");
        return v;
    }
}