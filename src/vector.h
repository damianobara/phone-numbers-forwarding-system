/** @file
   * Plik nagłówkowy udostępniający pomocniczą bibliotekę wektorów.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>

#define VECTOR_INIT_CAPACITY 4 ///<Początkowa pojemność wektora

/**
 * Tablicowa reprezentacja dynamicznego wektora.
 */
typedef struct vector {
    char *data;             ///< wskaźnik na tablicę charów, które przechowuje wektor.
    size_t capacity;           ///< całkowity rozmiar aktualnej tablicy data.
    size_t size;               ///< zajęty rozmiar aktualnej tablicy data.
} vector;

/**
 *Allokuje i inicjuje pusty wektor.
 * @param v wskaźnik na wektor.
 */
bool vectorInit(vector *v);

/**
 * Zmienia rozmiar wektora na capacity.
 * @param v - wskaźnik na wektor.
 * @param capacity - rozmiar nowego wektora.
 */
bool vectorResize(vector *v, size_t capacity);

/**
 * Dodaje chara do wektora.
 * @param v - wskaźnik na wektor.
 * @param item - dodawany char.
 */
bool vectorAdd(vector *v, char item);

/**
 * Zwalnia wszystkie pola wektora i wektor.
 * @param v - wskaźnik na wektor.
 */
void vectorFree(vector *v);

/**
 * Tworzy nową tablicę charów z zawartości wektora i ją zwraca.
 * @param v - wskaźnik na wektor.
 * @return - wskaźnik na tablicę charów.
 */
char *vectorGetAll(vector *v);

#endif