/** @file
   * Plik nagłówkowy udostępniający funckje pomocnicze na listach.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#ifndef TELEFONY_LISTUTIL_H
#define TELEFONY_LISTUTIL_H

#include <stdio.h>

#include "phone_numbers.h"

/**
 * Zlicza węzły w liście.
 * @param head - wskaźnik na głowę listy
 * @return size_t reprezentujący liczbę węzłów w liście.
 */
size_t listLength(struct phoneNumberList *head);

/**
 * Usuwa listę i ustawia jej głowę na NULLa.
 * @param head - wskaźnik na głowę listy.
 */
void deleteList(struct phoneNumberList **head);

#endif //TELEFONY_LISTUTIL_H
