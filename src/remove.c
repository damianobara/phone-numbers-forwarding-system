/** @file
   * Implementacja funkcji phfwdRemove(struct PhoneForward *pf, char const *num) i jej funkcji pomocniczych.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#include "phone_numbers.h"
#include "list_util.h"
#include "add.h"
#include "phone_forward.h"

/**
 * Funkcja znajduje i zwraca wskaźnik na strukturę reprezentującą napis.
 * @param pf - struktura reprezentująca przekierowania.
 * @param num - wskaźnik na napis reprezentujący numer.
 * @return - wskaźnik na strukturę reprezentującą poszukiwany numer.
 */
struct PhoneForward *findNode(struct PhoneForward *pf, const char *num) {
    struct PhoneForward *act = pf;
    size_t i = 0;
    size_t len = strlen(num);
    while (i < len && (act->sons)[num[i] - '0']) {
        act = (act->sons)[num[i] - '0'];
        i++;
    }
    assert(act);
    if (i < len) {
        return NULL;
    }
    return act;
}

/**
 * Rekurencyjnie usuwa przekierowania numeru reprezentowanego przez strukturę act
 * oraz wszystkich numerów, których jest on prefiksem.
 * @param act - wskaźnik na aktualną strukturę.
 */
void recRemove(struct PhoneForward **act) {
    if (*act) {
        //Wywołujemy się rekurencyjnie na wszystkich synach.
        for (size_t j = 0; j < 12; j++) recRemove(&(((*act)->sons)[j]));

        if ((*act)->redirTo) {
            free((*act)->redirTo);
            (*act)->redirTo = NULL;

            assert ((*act)->myPlace);
            deleteMyPlace(*act);
            (*act)->myPlace = NULL;
        }
    }
}

void phfwdRemove(struct PhoneForward *pf, char const *num) {
    //Sprawdzamy poprawność parametrów.
    if (num && pf) {
        size_t i;
        size_t len = strlen(num);
        for (i = 0; i < len && (isdigit(num[i]) || (num[i] == ':' || num[i] == ';')); i++);
        //Przypadek gdy dane są poprawne.
        if (len != 0 && (i == len)) {
            //Znajdujemy węzeł reprezentujący nasz prefix.
            struct PhoneForward *prefix = findNode(pf, num);
            //Jeśli należy on do drzewa to usuwamy go i całe poddrzewo, którego jest korzeniem.
            if (prefix) {
                recRemove(&prefix);
            }
        }
    }
}