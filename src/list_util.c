/** @file
   * Implementacja funkcji pomocniczych na listach.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "phone_numbers.h"

//Funkcja zliczająca węzły w liście.
size_t listLength(struct phoneNumberList *head) {
    size_t len = 0;
    struct phoneNumberList *act = head;
    while (act != NULL) {
        len++;
        act = act->next;
    }
    return len;
}

//Funkcja usuwająca listę i ustawiająca jej głowę na NUlla.
void deleteList(struct phoneNumberList **head) {
    struct phoneNumberList *current = *head;
    struct phoneNumberList *next;

    while (current != NULL) {
        next = current->next;
        if (current->key) {
            free(current->key);
            current->key = NULL;
        }
        free(current);
        current = next;
    }
    *head = NULL;
}