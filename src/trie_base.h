/** @file
   * Plik nagłówkowy udostępniający strukturę trieBase oraz funkcje pomocnicze na trieBase.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#ifndef TELEFONY_TRIEBASE_H
#define TELEFONY_TRIEBASE_H

#include <stdio.h>

#include "phone_numbers.h"
#include "phone_forward.h"

/**
 * Struktura reprezentująca pojedyńczą bazę przekierowań.
 * Struktura ma formę listy.
 */
typedef struct trieBase {
    char *trieId;                   ///< wskaźnik na tablicę charów reprezentującą id bazy
    struct PhoneForward *trie;      ///< wskaźnik na drzewo trie z bazą przekierowań
    struct trieBase *next;          ///< wskaźnik na następną strukturę w liście
} trieBase;

/**
 * Struktura reprezentująca całe centrum przekierowań z dostępem do aktualnej bazy w czasie stałym.
 */
typedef struct trieBaseStructure {
    trieBase *head;                 ///< wskaźnik na głowę listy reprezentującą drzewa przekierowań.
    trieBase *act;                  ///< wskaźnik na aktualne drzewo przekierowań.
} trieBaseStructure;

/**
 * Funkcja alokująca nową strukturę trieBase o identyfikatorze id.
 * @param id - wskaźnik na tablicę charów reprezentującą identyfikator
 * @return nowo zaalokowaną strukturę lub NULL jeśli alokacja się nie udała.
 */
trieBase *newTrieBase(char *id);

/**
 * Usuwa następny element z listy zza prev i zwraca go.
 * @param prev - wskaźnik na element listy trieBase.
 * @return wskaźnik na prev->next.
 */
trieBase *takeFromBehind(trieBase *prev);

/**
 * Usuwa wszystkie pola w strukturze trieBase a następnie usuwa strukturę.
 * @param del - wkaźnik na usuwaną strukturę.
 */
void deleteTrieBase(trieBase *del);

/**
 * Przechodzi po liście trieBase i usuwa wszystkie jej elementy.
 * Ustawia *head na NULLa.
 * @param head - wskaźnik na głowę listy.
 */
void deleteTrieBaseList(trieBase **head);

/**
 * Allokuje nową strukturę trieBaseStructure i jej pola pustymi strukurami trieBase.
 * @return wskaźnik na zaalokowaną strukturę lub NULL jeśli allokacja się nie udała.
 */
trieBaseStructure *newTrieBaseStructure(void);

/**
 * Usuwa strukturę trieBaseStructure i jej pola.
 * @param del - wskaźnik na usuwaną strukturę.
 */
void deleteTrieBaseStructure(trieBaseStructure *del);

#endif //TELEFONY_TRIEBASE_H
