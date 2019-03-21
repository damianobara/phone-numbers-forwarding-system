/** @file
   *Plik nagłówkowy udostępniający funkcję deleteMyPlace.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#ifndef TELEFONY_ADD_H
#define TELEFONY_ADD_H

#include <stdio.h>

#include "phone_forward.h"

/**
 * @brief Usuwa strukturę act->myPlace.
 * Usuwa strukturę wskazywaną przez act->myPlace tak, by zachować ciągłość listy,
 * w której była przechowywana.
 * Ustawia act->myPlace na NULL.
 * @param act - wskaźnik na strukturę.
 */
void deleteMyPlace(struct PhoneForward *act);


#endif //TELEFONY_ADD_H
