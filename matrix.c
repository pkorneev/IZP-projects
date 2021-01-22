#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ALL_ROWS 0 // pokud uvolnujeme celou matici

// typy matic
#define NORMAL 0   // bezna
#define IDENTITY 1 // jednotkova

typedef struct
{
    unsigned rows; /**< Pocet radku matice */
    unsigned cols; /**< Pocet sloupcu matice */
    float **data;  /**< Prvky matice */
} matrix_t;

// pomocne funkce
matrix_t matrix_ctor(int rows, int cols, unsigned t);
void matrix_init(matrix_t *m, unsigned t);
void matrix_dtor(matrix_t *m, unsigned r);
void matrix_print(matrix_t *m);

// funkce pro praci s maticemi
void matrix_switchRows(matrix_t *m, unsigned a, unsigned b);
bool matrix_identity(matrix_t *m);
void matrix_constMult(matrix_t *m, float cc);
void matrix_add(matrix_t *m1, matrix_t *m2);

// alternativni hlavicky funkci
/*
matrix_t matrix_switchRows(matrix_t* m, unsigned a, unsigned b); // funkce vraci vysledek
matrix_t matrix_constMult(matrix_t* m, float cc); // funkce vraci vysledek
matrix_t matrix_add(matrix_t* m1, matrix_t* m2); // funkce vraci vysledek
*/

// pridejte si dalsi funkce (nasobeni, hlavni diagonala, atd....)

int main()
{
}

/**
    @param rows Pocet radku matice
    @param cols Pocet sloupcu matice
    @param t typ matice (0, NORMAL - normalni; 1, IDENTITY - jednotkova)
    @return Funkce vraci alokovanou ridkou matici.

    Funkce alokuje ridkou matici typu float. Nejdrive alokuje pole ukazatelu (radky), pote alokuje pro kazdy radek sloupce.
    Pri chybe alokace funkce uvolnuje dosud nactenou pamet (pomoci funkce matrix_dtor), v polozce data bude NULL.
    Po uspesne alokaci naplni matici hodnotami podle zvoleneho typu (funkce matrix_init).
*/
matrix_t matrix_ctor(int rows, int cols, unsigned t)
{
    matrix_t m
    {
        .rows = rows;
        .cols = cols;
        .data = data;
    }
    m.data = malloc(rows * sizeof(float));
    if (m.data == NULL)
    {
        return m;
    }
    for (int i = 0; i < rows; i++)
    {
        m.data[i] = malloc(cols * sizeof(float));
        if (m.data[i] == NULL)
        {
            matrix_dtor(&m, i);
            return m;
        }
    }
    matrix_init(&m, t);
    return m;
}

/**
 * @param m matice
 * @param t typ matice (0, NORMAL - normalni; 1, IDENTITY - jednotkova)
 * Funkce ulozi do spravne alokovane hodnoty podle typu:
 * NORMAL (normalni) - hodnoty se budou zvysovat napr.
 * 0 1 2
 * 3 4 5
 * 6 7 8
 * IDENTITY (jednotkova) - funkce vytvori jednotkovou matici
 * */
void matrix_init(matrix_t *m, unsigned t)
{
    float value = 0.0;
    for (unsigned int r = 0; r < m->rows; r++)
    {
        for (unsigned int c = 0; c < m->cols; c++)
        {
            if (t == NORMAL)
            {
                m->data[r][c] = value++
            }
            else
            {
                if (r == c)
                {
                    m->data[r][c] = 1.0;
                }
                else
                {
                    m->data[r][c] = 0;
                }
            }
        }
    }
}

/**
    @param m Matice, kterou uvolnujeme z pameti.
    @param r Pocet radku matice (0 nebo ALL_ROWS - vsechny radky), jinak index radku, od ktereho zaciname dealokovat.
    Funkce dealokuje pamet pro matici. Dealokace probiha bud od posledniho radku (pokud ALL_ROWS), nebo od posledniho radku, ktery se podarilo alokovat.
*/
void matrix_dtor(matrix_t *m, unsigned r)
{
}

/**
 * @param m matice
 * Funkce, která vypise obsah matice. Napr.
  0 1 2
  3 4 5
  6 7 8
  Mezi sloupci bude znak \t
 */
void matrix_print(matrix_t *m)
{
}

/**
 * @param m matice
 * @param a index radku
 * @param b index radku
 * Funkce prohodi radky s indexem a a b  v matici a.
 */
void matrix_switchRows(matrix_t *m, unsigned a, unsigned b)
{
}

/**
 * @param m matice
 * @return true pokud je matice jednotkova, false jindy
 * Funkce overi, zda je matice jednotkova.
 */
bool matrix_identity(matrix_t *m)
{
}

/**
 * @param m matice
 * @param c konstanta, kterou nasobime
 * Funkce provadi m = m*c
 * */
void matrix_constMult(matrix_t *m, float cc)
{
}

/**
 * @param m1 matice
 * @param m2 matice
 * Funkce provadi m1 = m1+m2
 * */
void matrix_add(matrix_t *m1, matrix_t *m2)
{
}
