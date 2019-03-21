/** @file
   * Implementacja funkcji pomocniczych na struct PhoneNumbers.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "phone_numbers.h"
#include "list_util.h"
#include "phone_forward.h"

struct PhoneNumbers *emptyPhoneNumber(void) {
    struct PhoneNumbers *empty = malloc(sizeof(struct PhoneNumbers));
    if (!empty) return NULL;
    empty->number = NULL;
    empty->size = 0;

    return empty;
}

//Funkcja tworzy i zwraca strukturę PhoneNumbers na podstawie napisu num, który jest kopiowany.
struct PhoneNumbers *newNumberFromString(const char *num) {
    assert(num);

    //Allokujemy strukturę.
    struct PhoneNumbers *new = malloc(sizeof(struct PhoneNumbers));
    char **newTab = malloc(sizeof(char *));

    //Przypadek nieudanej allokacji.
    if (!new || !newTab) {
        if (new) free(new);
        if (newTab) free(newTab);
        return NULL;
    }

    //Allokujemy miejsce na stringa do *(new->numbers)
    *newTab = malloc(sizeof(char) * (strlen(num) + 1));

    //Przypadek niudanej allokacji.
    if (!(*newTab)) {
        free(new);
        free(newTab);
        return NULL;
    }
    //Kopiujemy stringa.
    strcpy(*newTab, num);

    //Podpinamy go do struktury.
    new->number = newTab;
    new->size = 1;

    return new;
}

//Funkcja usuwająca strukturę PhoneNumbers.
void phnumDelete(const struct PhoneNumbers *pnum) {

    //Usuwamy wszystkie napisy w del->number i ustawiamy je na NULLa.
    if (pnum) {
        for (size_t i = 0; i < pnum->size; i++) {
            assert((pnum->number)[i]);
            free((pnum->number)[i]);
        }
        free(pnum->number);
        free((void*)pnum);
    }
}

//Funkcja udostęnia wskaźnik na napis o indeksie idx.
char const *phnumGet(struct PhoneNumbers const *pnum, size_t idx) {
    if (!pnum) {
        return NULL;
    }
    if (pnum->size <= idx) {
        return NULL;
    }

    return (pnum->number)[idx];
}