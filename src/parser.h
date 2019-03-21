/** @file
   * Plik nagłówkowy udostępniający funkcje interfejsu tekstowego programu.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#ifndef TELEFONY_PARSER_H
#define TELEFONY_PARSER_H

#include "trie_base.h"

/**
 * Typ wyliczeniowy służący do opisania możliwych operacji programu.
 */
typedef enum {
    empty, delNumber, delId, questionNumber, newId, numberQuestion, numberToNumber, end, atNumber
} State;

/**
 * Typ wyliczeniowy służący do opisania rozpoznawanych przez program leksemów.
 */
typedef enum {
    id, white, DEL, NEW, number, question, to, endToken, def, at
} Token;

/**@brief Funkcja wczytująca i wykonująca jedną operację programu
 * Funkcja wczytuje minimalną liczbę danych potrzebną do sprawdzenia jaka opercja będzie wykonywana.
 * Jeśli dane te są niepoprawne zwraca error.
 * Następnie w zależności od poprzedniej operacji sprawdza czy aktualna operacja jest poprawna.
 * Jeśli jest niepoprawna zwraca error.
 * Później funkcja wczytuje do końca daną operację i jeśli nie jest ona błędna wykonuje ją.
 * Funkcja zwraca State reprezentujący wczytaną operację i modyfikuje *prevByte, na aktualny bajt wejścia.
 * @param prev - poprzednio wczytana operacja.
 * @param prevByte - wskaźnik na pierwszy bajt nowej operacji.
 * @param actTrie - wskaźnik na aktualne drzewo przekierowań.
 * @param trieHead - wskaźnik na bazę drzew przekierowań.
 * @return State reprezentujący wczytaną operację.
 */
State readToState(State prev, size_t *prevByte, trieBase **actTrie, trieBase **trieHead);

#endif //TELEFONY_PARSER_H
