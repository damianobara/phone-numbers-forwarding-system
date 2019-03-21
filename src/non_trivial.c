/** @file
   * Implementacja funkcji phfwdNonTrivialCount(struct PhoneForward *pf, char const *set, size_t len) i jej funkcji pomocniczych.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "phone_forward.h"
#include "phone_numbers.h"
#include "list_util.h"

/**
 * Funkcja potęgująca modulo size_t
 * @param base - podstawa potęgi
 * @param exp - wykładniik potęgi
 * @return - wynik potęgowania
 */
size_t myPower(size_t base, size_t exp) {
    size_t result = 1;
    while (exp != 0) {
        if ((exp & 1) == 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

/**
 * Pomocnicza funkcja rekurencyjna dla phfwdNonTrivialCount(struct PhoneForward *pf, char const *set, size_t len)
 * @param act - wskaźnik na aktualny struct PhoneForward
 * @param digits - cyfry z których może być złożony numer
 * @param actLen - aktualna "głębokość" wejścia w drzewo przekierowań
 * @param totalLen - długość numerów, których nietrywialnych przekierowań szukamy
 * @return - liczba nietrywialnych przekierowań o długości len złożonych z cyfr w digits w poddrzewie aktualnego węzła drzewa
 */
size_t nonTrivialRec(struct PhoneForward *act, bool digits[], size_t actLen, size_t totalLen) {
    if (!act) {
        return 0;
    }
    if (actLen > totalLen) {
        return 0;
    }
    //Dany węzeł dodaje nietrywialne przekierowanie, jeśli długość jego listy przekierowań jest większa niż 2.
    size_t listLen = listLength(act->redirFrom);
    if (2 < listLen) {
        size_t howManyDigits = 0;
        for (size_t i = 0; i < 12; i++) {
            if (digits[i]) {
                howManyDigits++;
            }
        }
        //Liczymy liczbę nietrywialnych przekierowań za które odpowiada aktualny węzeł.
        size_t result = myPower(howManyDigits, (totalLen - actLen));
        return result;
    } else {
        size_t recSum = 0;
        for (size_t i = 0; i < 12; i++) {
            if (digits[i]) {
                recSum += nonTrivialRec((act->sons)[i], digits, actLen + 1, totalLen);
            }
        }
        return recSum;
    }
}

size_t phfwdNonTrivialCount(struct PhoneForward *pf, char const *set, size_t len) {
    //Sprawdzamy poprawność parametrów.
    if (!pf) {
        return 0;
    }
    if (!set) {
        return 0;
    }
    if (strlen(set) == 0) {
        return 0;
    }
    if (len == 0) {
        return 0;
    }
    bool digits[12];
    for (size_t i = 0; i < 12; i++) digits[i] = false;

    bool hasDigit = false;

    //Wyznaczamy cyfry z których może być złożony numer.
    for (size_t i = 0; i < strlen(set); i++) {
        if (0 <= set[i] - '0' && set[i] - '0' < 12) {
            digits[set[i] - '0'] = true;
            hasDigit = true;
        }
    }
    if (hasDigit == false) {
        return 0;
    }
    size_t nonTrivial = 0;
    //Wyznaczamy wszystkie nietrywialne przekierowania.
    for (size_t i = 0; i < 12; i++) {
        if (digits[i]) {
            nonTrivial += nonTrivialRec((pf->sons)[i], digits, 0, len - 1);
        }
    }

    return nonTrivial;
}