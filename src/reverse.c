/** @file
   * Implementacja funkcji phfwdReverse(struct PhoneForward *pf, char const *num) i jej funkcji pomocniczych.
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
#include "phone_forward.h"

/**
 * Komparator do quicksorta.
 * @param first - wskaźnik na napis reprezentujący numer.
 * @param second - wskaźnik na napis reprezentujący numer.
 * @return 1 jeśli numer reprezentowany przez first jest większy leksykograficznie
 * od numeru reprezentowanego przez second, -1 w przeciwnym razie lub 0 jeśli są równe.
 */
static int compare(const void *first, const void *second) {
    char *firstNum = *(char **) first;
    char *secondNum = *(char **) second;
    return strcmp(firstNum, secondNum);
}

/**@brief Usuwa powtarzające się napisy w posortowanej tablicy i zwraca ilość różnych napisów.
 * Usuwa wszystkie powtarzające się napisy oprócz ostatnich w ciągu powtórzeń. Usunięte napisy ustawia na NULL.
 * Zwraca liczbę różnych napisów w tablicy.
 * @param tab wskaźnik na tablicę wskaźników na napisy reprezentujące numery.
 * @param n - długość tablicy tab.
 * @return int reprezentujący liczbę różnych numerów w tablicy.
 */
size_t removeDuplicates(char **tab, size_t n) {
    if (n == 0 || n == 1) {
        return n;
    }

    /*Usuwamy powtarzające się węzły,
    * Ustawiamy je na NULLa.
    * Zostawiamy ostatni powtarzający się element.
    * Zliczamy ile węzłów usunęliśmy.*/
    size_t howManyNulls = 0;
    for (size_t i = 0; i < n - 1; i++) {
        assert(tab[i] && tab[i + 1]);
        if (!strcmp(tab[i], tab[i + 1])) {
            free(tab[i]);
            tab[i] = NULL;
            howManyNulls++;
        }
    }
    //Zwracamy ile zostało nie NULLi.
    return (n - howManyNulls);
}

struct PhoneNumbers const *phfwdReverse(struct PhoneForward *pf, char const *num) {

    //Sprawdzamy poprawność parametrów funkcji.
    if (!num || !pf) {
        return emptyPhoneNumber();
    }
    size_t len = strlen(num);
    if (len == 0) {
        return emptyPhoneNumber();
    }
    size_t i;
    for (i = 0; i < len && (isdigit(num[i]) || (num[i] == ':' || num[i] == ';')); i++);

    //Sprawdzamy czy w obu stringach są tylko cyfry i znak końca napisu (czy są liczbami).
    if (i != len) {
        return emptyPhoneNumber();
    }

    struct PhoneForward *act = pf;
    size_t allListLen = 0;

    // Zliczamy sumaryczną długość wszystkich list z przekierowaniami we wszystkich węzłach reprezentujących prefiksy num.
    i = 0;
    while (i < len && (act->sons)[num[i] - '0']) {
        act = (act->sons)[num[i] - '0'];
        assert(act->redirFrom);
        allListLen += listLength((act->redirFrom)->next);
        i++;
    }

    //Allokujemy wynikową strukturę PhoneNUmbers.
    struct PhoneNumbers *result = malloc(sizeof(struct PhoneNumbers));
    if (!result) {
        return emptyPhoneNumber();
    }
    result->number = malloc(sizeof(char *) * (allListLen + 1));
    if (!result->number){
        phnumDelete(result);
        return emptyPhoneNumber();
    }
    result->size = allListLen + 1;

    for (size_t k = 0; k < result->size; k++) (result->number)[k] = NULL;

    struct phoneNumberList *head;
    size_t allAllocatedStrings = 0;
    size_t newAllocatedStrings = 0;
    size_t currentListLen = 0;

    act = pf;
    i = 0;
    //Przechodzimy w pętli po wszystkich węzłach reprezentujących reprezentujących prefiksy numeru num.
    while (i < len && (act->sons)[num[i] - '0']) {
        //Przychodzimy na nowy węzeł drzewa.
        act = (act->sons)[num[i] - '0'];
        assert(act->redirFrom);

        //W każdym węźle przechodzimy listę jego przekierowań pomijając atrapę i zliczamy jej długość.
        head = (act->redirFrom)->next;
        currentListLen = listLength(head);
        newAllocatedStrings = allAllocatedStrings + currentListLen;

        //Przechodzimy po wszystkich węzłach w liście przekierowań danego węzłą drzewa i kopiujemy te przekierowania.
        while (allAllocatedStrings < newAllocatedStrings && head) {
            (result->number)[allAllocatedStrings] = malloc(sizeof(char) * (strlen(head->key) + len - i + 1));
            if (!(result->number)[allAllocatedStrings]) {
                phnumDelete(result);
                return emptyPhoneNumber();
            }
            strcpy((result->number)[allAllocatedStrings], head->key);
            strcat((result->number)[allAllocatedStrings], num + i + 1);

            head = head->next;
            allAllocatedStrings++;
        }
        allAllocatedStrings = newAllocatedStrings;
        i++;
    }

    //Dopisujemy otrzymany numer na sam koniec wynikowej tablicy napisuów.
    (result->number)[allListLen] = malloc(sizeof(char) * (len + 1));
    if (!(result->number)[allListLen]){
        phnumDelete(result);
        return emptyPhoneNumber();
    }
    strcpy((result->number)[allListLen], num);

    //Sortujemy tablicę.
    qsort(result->number, result->size, sizeof(char *), compare);

    //NULLujemy powtórzenia.
    size_t newSize = removeDuplicates(result->number, result->size);

    //Allokujemy nową tablicę.
    char **newNumber = malloc(sizeof(char *) * newSize);
    if (!newNumber){
        phnumDelete(result);
        return emptyPhoneNumber();
    }

    size_t oldPosition = 0;
    size_t newPosition = 0;

    //Przepisujemy starą tablicę do nowej bez NULLi i powtórzeń.
    while (oldPosition < result->size && newPosition < newSize) {
        if ((result->number)[oldPosition]) {
            newNumber[newPosition] = malloc(sizeof(char *) * (strlen((result->number)[oldPosition]) + 1));
            if (!newNumber[newPosition]){
                phnumDelete(result);
                for(size_t s = 0; s < newPosition; s++) free(newNumber[s]);
                free(newNumber);
                return emptyPhoneNumber();
            }
            strcpy(newNumber[newPosition], (result->number)[oldPosition]);
            newPosition++;
        }
        oldPosition++;
    }

    assert(oldPosition == result->size);
    assert(newPosition == newSize);

    //Usuwamy starą tablicę.
    for (size_t k = 0; k < result->size; k++) {
        free((result->number)[k]);
        (result->number)[k] = NULL;
    }
    free(result->number);

    //Podpinamy nową tablicę.
    result->number = newNumber;
    result->size = newSize;

    return result;
}