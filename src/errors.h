/** @file
 * Plik nagłówkowy udostępniający funkcję wypisywania błędów.
 * @author Damian Obara
 * @copyright Uniwersytet Warszawski
 * @date 14.06.2018
 *
 */

#ifndef TELEFONY_ERRORS_H
#define TELEFONY_ERRORS_H

#include <stdio.h>
#include <stdlib.h>

#include "trie_base.h"

/**
 * Funkcja wypisująca nieokreślony błąd leksykalny.
 * @param byte - bajt wejścia w którym był błąd.
 */
void errorLexical(size_t byte);

/**
 * Funkcja wypisująca błąd końca pliku.
 */
void errorEOF(void);

/**
 * Funkcja wypisująca błąd podczas wczytywania operacji NEW identyfikator.
 * @param initByte - bajt wejścia w którym był błąd.
 */
void errorNewId(size_t initByte);

/**
 * Funkcja wypisująca błąd podczas wczytywania operacji DEL identyfikator.
 * @param initByte - bajt wejścia w którym był błąd.
 */
void errorDelId(size_t initByte);

/**
 * Funkcja wypisująca błąd podczas wczytywania operacji numer > numer.
 * @param initByte - bajt wejścia w którym był błąd.
 */
void errorNumberToNumber(size_t initByte);

/**
 * Funkcja wypisująca błąd podczas wczytywania operacji ? numer.
 * @param initByte - bajt wejścia w którym był błąd.
 */
void errorQuestionNumber(size_t initByte);

/**
 * Funkcja wypisująca błąd podczas wczytywania operacji numer ?.
 * @param initByte - bajt wejścia w którym był błąd.
 */
void errorNumberQuestion(size_t initByte);

/**
 * Funkcja wypisująca błąd podczas wczytywania operacji DEL numer.
 * @param initByte - bajt wejścia w którym był błąd.
 */
void errorDelNumber(size_t initByte);

/**
 * Funkcja usuwająca bazę baz przekierowań przy errorze.
 * @param center - usuwana baza baz.
 */
extern void atExitCleanTrieBaseStructure(trieBaseStructure *center);

#endif //TELEFONY_ERRORS_H
