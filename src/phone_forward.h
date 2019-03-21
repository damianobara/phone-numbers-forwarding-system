/** @file
   * Interfejs klasy przechowującej przekierowania numerów telefonicznych
   *
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   */

#ifndef TELEFONY_PHONE_FORWARD_H
#define TELEFONY_PHONE_FORWARD_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "phone_numbers.h"

/**
  * Struktura przechowująca przekierowania numerów telefonów.
  * Struktura jest drzewem trie.
  */
struct PhoneForward {
    char key;                               ///< Znak opowiadający krawędzi pomiędzy strukturą (Node'm) a jej ojcem.
    struct PhoneForward *sons[12];          ///< Wskaźniki na struktury reprezentujące numery o długości o 1 większej od aktualnego, których aktualny numer jest prefiksem.
    char *redirTo;                          ///< Wskaźnik na napis reprezentujący numer, na który przekierowany jest numer reprezentowany przez aktualną strukturę.
    struct phoneNumberList *redirFrom;      ///< Wskaźnik na listę numerów, które są przekierowane na numer reprezentowany przez aktualną strukturę.
    struct phoneNumberList *myPlace;        ///< Wskaźnik na węzeł listy reprezentujący przekinumer reprezentowany przez aktualną strukturę.
};


/**
 * Struktura przechowująca ciąg numerów telefonów.
 */
struct PhoneNumbers {
    char **number;                          ///< Wskaźnik na tablicę wskaźników na napisy reprezentujące numery.
    size_t size;                            ///< Rozmiar tej tablicy.
};


/** @brief Udostępnia numer.
 * Udostępnia wskaźnik na napis reprezentujący numer. Napisy są indeksowane
 * kolejno od zera.
 * @param[in] pnum – wskaźnik na strukturę przechowującą ciąg napisów;
 * @param[in] idx  – indeks napisu.
 * @return Wskaźnik na napis. Wartość NULL, jeśli indeks ma za dużą wartość.
 */
char const *phnumGet(struct PhoneNumbers const *pnum, size_t idx);


/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pnum. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] pnum – wskaźnik na usuwaną strukturę.
 */
void phnumDelete(const struct PhoneNumbers *pnum);


/** @brief Usuwa przekierowania.
  * Usuwa wszystkie przekierowania, w których parametr @p num jest prefiksem
  * parametru @p num1 użytego przy dodawaniu. Jeśli nie ma takich przekierowań
  * lub napis nie reprezentuje numeru, nic nie robi.
  *
  * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
  * @param[in] num – wskaźnik na napis reprezentujący prefiks numerów.
  */
void phfwdRemove(struct PhoneForward *pf, char const *num);


/** @brief Wyznacza przekierowania na dany numer.
 * Wyznacza wszystkie przekierowania na podany numer. Wynikowy ciąg zawiera też
 * dany numer. Wynikowe numery są posortowane leksykograficznie i nie mogą się
 * powtarzać. Jeśli podany napis nie reprezentuje numeru, wynikiem jest pusty
 * ciąg. Alokuje strukturę @p PhoneNumbers, która musi być zwolniona za pomocą
 * funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów.
 */
struct PhoneNumbers const *phfwdReverse(struct PhoneForward *pf, char const *num);



/** @brief Dodaje przekierowanie.
 * Dodaje przekierowanie wszystkich numerów mających prefiks @p num1, na numery,
 * w których ten prefiks zamieniono odpowiednio na prefiks @p num2. Każdy numer
 * jest swoim własnym prefiksem. Jeśli wcześniej zostało dodane przekierowanie
 * z takim samym parametrem @p num1, to jest ono zastępowane.
 * @param[in] pf   – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num1 – wskaźnik na napis reprezentujący prefiks numerów
 *                   przekierowywanych;
 * @param[in] num2 – wskaźnik na napis reprezentujący prefiks numerów, na które
 *                   jest wykonywane przekierowanie.
 * @return Wartość @p true, jeśli przekierowanie zostało dodane.
 *         Wartość @p false, jeśli wystąpił błąd, np. podany napis nie
 *         reprezentuje numeru, oba podane numery są identyczne lub nie udało
 *         się zaalokować pamięci.
 */
bool phfwdAdd(struct PhoneForward *pf, char const *num1, char const *num2);

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych przekierowań.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         zaalokować pamięci.
 */
struct PhoneForward *phfwdNew(void);


/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p pf. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in] act – wskaźnik na usuwaną strukturę.
 */
void phfwdDelete(struct PhoneForward *act);

/** @brief Wyznacza przekierowanie numeru.
 * Wyznacza przekierowanie podanego numeru. Szuka najdłuższego pasującego
 * prefiksu. Wynikiem jest co najwyżej jeden numer. Jeśli dany numer nie został
 * przekierowany, to wynikiem jest ten numer. Jeśli podany napis nie
 * reprezentuje numeru, wynikiem jest pusty ciąg. Alokuje strukturę
 * @p PhoneNumbers,która musi być zwolniona za pomocą funkcji @ref phnumDelete.
 * @param[in] pf  – wskaźnik na strukturę przechowującą przekierowania numerów;
 * @param[in] num – wskaźnik na napis reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą ciąg numerów.
 */
const struct PhoneNumbers *phfwdGet(struct PhoneForward *pf, char const *num);


#endif //TELEFONY_PHONE_FORWARD_H
