/** @file
   * Plik nagłówkowy udostępniający funkcję phfwdNonTrivialCount(struct PhoneForward *pf, char const *set, size_t len).
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#ifndef TELEFONY_NONTRIVIAL_H
#define TELEFONY_NONTRIVIAL_H

#include <stdio.h>

#include "phone_forward.h"

/**
* Funkcja oblicza liczbę nietrywialnych numerów długości len zawierających tylko cyfry, które znajdują się w napisie set.
* Jeśli wskaźnik pf ma wartość NULL, set ma wartość NULL, set jest pusty, set nie zawiera żadnej cyfry lub parametr len jest równy zeru, wynikiem jest zero.
* Obliczenia należy wykonywać modulo dwa do potęgi liczba bitów reprezentacji typu size_t. Napis set może zawierać dowolne znaki.
* @param pf - wskaźnik na struct PhoneForward reprezentujące bazę przekierowań.
* @param set - wskaźnik na tablicę cyfr z których możemy korzystać
* @param len - długość nietrywialnych numeróœ, których szukamy
* @return - liczbę nietrywialnych numerów długości len zawierających tylko cyfry, które znajdują się w napisie set.
*/
size_t phfwdNonTrivialCount(struct PhoneForward *pf, char const *set, size_t len);

#endif //TELEFONY_NONTRIVIAL_H
