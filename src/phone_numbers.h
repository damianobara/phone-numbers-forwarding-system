/** @file
   * Plik nagłówkowy udostępniający strukturę phoneNUmberList oraz funkcje pomocnicze na struct PhoneNumbers.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#ifndef TELEFONY_PHONENUMBER_H
#define TELEFONY_PHONENUMBER_H

#include <stdio.h>

#include "phone_forward.h"

/**
 * Dwukierunkowa lista wskaźników na chary.
 */
struct phoneNumberList {
    char *key;                          ///< Wskaźnik na napis reprezentujący numer reprezentowany przez strukturę
    struct phoneNumberList *prev;       ///< Wskaźnik na następny element listy.
    struct phoneNumberList *next;      ///< Wskaźnik na poprzedni element listy.
};

/**
 * Tworzy pustą strukturę.
 * @return wskaźnik na utworzoną strukturę.
 */
struct PhoneNumbers *emptyPhoneNumber(void);

/**
 * @brief Tworzy strukturę.
 * Tworzy strukturę reprezentującą numer na podstawie wskaźnika na napis reprezentujący numer
 * @param num - wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę reprezentującą napis. NULL, jeśli alokacja się nie udała.
 */
struct PhoneNumbers *newNumberFromString(const char *num);

#endif //TELEFONY_PHONENUMBER_H
