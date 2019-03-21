/** @file
   * Implementacja funkcji phfwdGet(struct PhoneForward *pf, char const *num) oraz jej funkcji pomocniczych.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "phone_numbers.h"
#include "list_util.h"
#include "phone_forward.h"

/**
 * Funkcja pomocnicza dla phfwdGet(struct PhoneForward *pf, char const *num). Zwraca tablicę charów reprezentującą przekierowanie.
 * @param act - wskaźnik na aktualny struct PhoneForward
 * @param num - wskaźnik tablicę charów reprezentujących numer
 * @param i - indeks tablicy num w którym aktualnie jesteśmy
 * @param n - długość tablicy num
 * @param redirLen - wskaźnik na size_t
 * @return - wskaźnik na tablicę charów reprezentujący numer na który aktualny numer został przekierowany.
 */
char *iterGet(struct PhoneForward *act, char const *num, size_t i, size_t n, size_t *redirLen) {

    char *mem = NULL;
    /* Sprawdzamy wszystkie węzły reprezentujące prefiksy numeru num i zapamiętujemy najdłuższy
     * z nich, który ma jakieś przekierowanie oraz jego długość.*/
    while (i < n && (act->sons)[num[i] - '0']) {
        if (act->redirTo) {
            *redirLen = i;
            mem = act->redirTo;
        }
        act = (act->sons)[num[i] - '0'];
        i++;
    }
    if (act && act->redirTo) {
        *redirLen = i;
        mem = act->redirTo;
    }
    return mem;
}

const struct PhoneNumbers *phfwdGet(struct PhoneForward *pf, char const *num) {

    //Sprawdzamy poprawność parametrów funkcji.
    if (!num || !pf) {
        return emptyPhoneNumber();
    }
    size_t i;
    size_t numLen = strlen(num);
    for (i = 0; i < numLen && (isdigit(num[i]) || (num[i] == ':' || num[i] == ';')); i++);

    if (numLen == 0) {
        return emptyPhoneNumber();
    }
    //Sprawdzamy czy w obu stringach są tylko cyfry i znak końca stringu (czy są liczbami).
    if (i != numLen) {
        return emptyPhoneNumber();
    }
    size_t redirLen = 0;

    char *getResult = iterGet(pf, num, 0, numLen, &redirLen);

    //Jeśli żaden prefiks num nie miał przekierowania zwracamy wskaźnik na strukturę PhoneNumbers reprezentującą num.
    if (!getResult) {
        return newNumberFromString(num);
    }
    /* Jeśli jakiś prefiks num miał przekierowanie zwracamy wskaźnik na strukturę PhoneNumbers reprezentującą
     * konkatenację numeru na który został przekierowany najdłuższy prefiks numeru num oraz pozostego sufiksu num.*/
    size_t last;
    char *resultString;

    /* Liczymy różnicę długości num i najdłuższego prefiksu mającego przekierowanie.
     * Jest tyle ostatnich cyfr numeru num, ile należy dokleić do otrzymanego w iterGet numeru.*/
    assert(redirLen > 0);
    last = numLen - redirLen;
    resultString = malloc(sizeof(char) * (strlen(getResult) + last + 1));
    if (!resultString) {
        return emptyPhoneNumber();
    }
    //Kopiujemy i konkatenujemy otrzymane numery.
    strcpy(resultString, getResult);
    strcat(resultString, num + redirLen);

    struct PhoneNumbers *result = newNumberFromString(resultString);
    free(resultString);
    return result;
}