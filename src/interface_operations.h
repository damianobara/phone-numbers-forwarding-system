/** @file
   * Plik nagłówkowy udostępniający funckje wykonujące operacje interfejsu.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#ifndef TELEFONY_INTERFACEOPERATIONS_H
#define TELEFONY_INTERFACEOPERATIONS_H

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#include "phone_forward.h"
#include "trie_base.h"
#include "phone_numbers.h"

/**@brief Ustawia drzewo przekierowań o identyfikatorze id jako aktualne.
 * Szuka w bazie baz przekierowań bazy o identyfiaktorze id.
 * Jeśli taka już istnieje to ustawia ją jako aktualną.
 * Jeśli nie ma takiej to ją tworzy i ustawia jako aktualną.
 * Zwraca true jeśli wszystko się udało, w przeciwnym razie false.
 * @param act - wskaźnik na aktualną bazę.
 * @param head - wskaźnik na głowę listy baz.
 * @param id - wskaźnik na char* reprezentujący identyfikator bazy przekierowań.
 * @return true jeśli wszystko się udało, false w przeciwnym razie.
 */
bool addIdToTrieBase(trieBase **act, trieBase **head, char *id);

/**
 * @brief Usuwa drzwo przekierowań o identyfikatorze id.
 * Szuka w bazie baz przekierowań bazy o identyfiaktorze id.
 * Jeśli takie istnieje usuwa je.
 * Jeśli było ono ustawione jako aktualne to ustawia aktalne na NULLa
 * Jeśli takie nie istnieje zwraca false.
 * @param act - wskaźnik na aktualną bazę.
 * @param head - wskaźnik na głowę listy baz.
 * @param id - wskaźnik na char* reprezentujący identyfikator bazy przekierowań.
 * @return true jeśli baza o szukanum identyfikatorze istniała, false w przeciwnym przypadku.
 */
bool delIdFromTrieBase(trieBase **act, trieBase **head, char *id);

/**
 * Jeśli jej parametry są poprawne wywołuje phfwdAdd(root, number1, number2).
 * Zwraca true jeśli phfwdAdd zwróci true.
 * @param root - wskaźnik na bazę przekierowań.
 * @param number1 - wskaźnik na tablicę charów reprezentującą numner.
 * @param number2 wskaźnik na tablicę charów reprezentującą numner.
 * @return true, jeśli dodanie przekierowania się udało, false wpp.
 */
bool addRedirectionToTrieBase(trieBase *root, char *number1, char *number2);

/**
 * Jeśli jej parametry są poprawne wywołuje phfwdGet(root, number).
 * Zwraca true, jeśli dostanie niepusty wynik.
 * @param root - wskaźnik na bazę przekierowań numerów
 * @param number - wskaźnik na tablicę charów reprezentującą numner.
 * @return true, jeśli otrzymała niepusty wynik, false wpp.
 */
bool getRedirectionFromTrieBase(trieBase *root, char *number);

/**
 * Jeśli jej parametry są poprawne wywołuje phfwdReverse(root, number).
 * Zwraca true, jeśli dostanie niepusty wynik.
 *@param root - wskaźnik na bazę przekierowań numerów
 * @param number - wskaźnik na tablicę charów reprezentującą numner.
 * @return true, jeśli otrzymała niepusty wynik, false wpp.
 */
bool reverseRediorectionFromTrieBase(trieBase *root, char *number);

/**
 * Jeśli jej parametry są poprawne wywołuje phfwdRemove(root, number).
 * Zwraca true, jeśli to się udało.
 * @param root - wskaźnik na bazę przekierowań numerów
 * @param number - wskaźnik na tablicę charów reprezentującą numner.
 * @return true, jeśli dane parametry były poprawne, false wpp.
 */
bool removeRedirectionFromTrieBase(trieBase *root, char *number);

/**
 * Wypisuje jedną linię (zakończoną znakiem przejścia no nowej linii) zawierającą liczbę dziesiętną
 * (bez zer wiodących i innych dodatkowych znaków) będącą wynikiem działania funkcji phfwdNonTrivialCount dla aktualnej bazy przekierowań,
 * gdzie jako parametr set podano napis reprezentujący numer, a wartość len to max(0, |numer| - 12),
 * przy czym |numer| oznacza liczbę cyfr numeru numer.
 * @param root - wskaźnik na bazę przekierowań numerów
 * @param number - set
 */
void countNonTrivialInTreeBase(trieBase *root, char *number);

#endif //TELEFONY_INTERFACEOPERATIONS_H
