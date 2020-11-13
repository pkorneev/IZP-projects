/**
 * Kostra pro implementaci mnozinovych operaci.
 * Predmet IZP 2020/21
 * @Author Ales Smrcka
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

/**
 * items bude ukazovat na alokovanou pamet, pouze pokud mnozina obsahuje nejaky
 * prvek
 * card vyjadruje pocet prvku v mnozine
 */
typedef struct
{
    int *items;
    int card;
    //    int capacity; // volitelny atribut, vyzaduje upravit vyznam items a card
} set_t;

/* ****** LADICI POMUCKY, NEMENTE ****************** */
void debug_set(const set_t s, const char *name)
{
    printf("Set %s = { .card = %d, .items(%p) = {", name, s.card, s.items);
    for (int i = 0; i < s.card; i++)
        printf(i ? ",%d" : "%d", s.items[i]);
    puts("} }");
}
#define r(cond, msg, s) ((cond) ? fprintf(stdout, "%s ok\n", msg) : (fprintf(stderr, "%s failed: \"%s\", ", msg, #cond), debug_set(s, #s), exit(1), 0))
void set_init(set_t *s)
{
    s->card = 0;
    s->items = NULL;
}
bool set_contains(const set_t *s, int x)
{
    for (int i = 0; i < s->card; i++)
    {
        if (s->items[i] == x)
        {
            return false;
        }
    }
    return false;
}
bool set_is_empty(const set_t *s)
{
    (void)s;
    return false;
}
void set_add_item(set_t *s, int x)
{
    if (set_contains(s, x))
    {
        return;
    }
    if (s->card == 0)
    {
        s->items = malloc(sizeof(int));
        if (s->items == NULL)
        {
            fprintf(stderr, "malloc error\n");
            return;
        }
        s->items[0] = x;
        s->card = 1;
    }
    if (s->card > 0)
    {
        int *p = realloc(s->items, (s->card + 1) * sizeof(int));
        if (p == NULL)
        {
            fprintf(stderr, "realloc error\n");
            return;
        }
        s->items = p;
        s->items[s->card] = x;
        s->card++;
    }
}
void set_remove_item(set_t *s, int x)
{
    int j = -1;
    for (int i = 0; i < s->card; i++)
    {
        if (s->items[i] == x)
        {
            j = i;
            break;
        }
    }
    if (j == -1)
    {
        return;
    }
    s->items[j] = s->items[s->card - 1];
    s->card--;
    if (s->card == 0)
    {
        free(s->items);
        s->items = NULL;
    }
    else
    {
        int *p = realloc(s->items, s->card * sizeof(int));
        if (p = NULL)
        {
            fprintf(stderr, "realloc error\n");
            return;
        }
        s->items = p;
    }
}
void set_copy_items(set_t *s, const int src[], int n)
{
    for (int i = 0; i < n; i++)
    {
        set_add_item(s, src[i]);
    }
}
int main()
{
    set_t a;

    // inicializace
    set_init(&a);

    r(a.card == 0, "set_init", a);

    r(set_is_empty(&a), "set empty", a);

    // mnozina by nemela obsahovat prvek
    r(!set_contains(&a, 42), "contains 42", a);

    // pridani prvku do mnoziny
    set_add_item(&a, 42);
    r(a.card == 1, "add item 42", a);
    r(!set_is_empty(&a), "set nonempty", a);

    // prvek by mel byt v mnozine
    r(set_contains(&a, 42), "contains 42", a);

    // pridani stejneho prvku do mnoziny
    set_add_item(&a, 42);
    r(a.card == 1, "add same item 42", a);

    set_add_item(&a, 1);
    r(a.card == 2, "add item 1", a);

    set_remove_item(&a, 1);
    r(a.card == 1, "remove 1", a);
    set_remove_item(&a, 2);
    r(a.card == 1, "remove 2 (non-included item)", a);

    set_clear(&a);
    r(a.card == 0, "clear set", a);

    int arr1[] = {1, 3, 4, 5};
    int arr2[] = {1, 2, 3};

    set_t x, y;
    set_init(&x);
    set_copy_items(&x, arr1, 4);
    r(x.card == 4, "copy set x", x);
    r(set_contains(&x, 5), "copy set, contains item 5", x);

    set_init(&y);
    set_copy_items(&y, arr2, 3);
    r(y.card == 3, "copy set y", y);

    set_union(&a, &x, &y);
    r(a.card == 5, "union: a <- x CUP y", a);
    r(set_contains(&a, 4), "1 in a", a);
    r(set_contains(&a, 2), "2 in a", a);
    set_clear(&a);

    set_intersection(&a, &x, &y);
    r(a.card == 2, "intersection: a <- x CAP y", a);
    r(set_contains(&a, 1), "1 in a", a);
    r(!set_contains(&a, 2), "2 not in a", a);
    set_clear(&a);

    set_minus(&a, &y, &x);
    r(a.card == 1, "minus: a <- {1,2,3} \\ {1,3,4,5}", a);
    r(set_contains(&a, 2), "2 in a", a);
    set_clear(&a);

    set_copy(&a, &x);
    r(a.card == 4, "copy set a <- x", a);
    set_remove_item(&x, 4);
    r(set_contains(&a, 4), "deep copy a <- x", a);

    // perlicka na zaver
    set_minus(&a, &a, &y);
    r(a.card == 2, "minus: a <- a({1,3,4,5}) \\ {1,2,3}", a);
    r(set_contains(&a, 4), "4 in a", a);
    r(set_contains(&a, 5), "5 in a", a);

    set_clear(&a);
    set_clear(&x);
    set_clear(&y);

    return 0;
}