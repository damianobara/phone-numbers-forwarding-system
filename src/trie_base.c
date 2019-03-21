/** @file
   * Implementacja funkcji pomocniczych ma trieBase.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "phone_numbers.h"
#include "trie_base.h"
#include "phone_forward.h"

trieBase *newTrieBase(char *id) {
    assert(id != NULL);
    trieBase *new = malloc(sizeof(trieBase));
    if (new == NULL) return NULL;
    new->next = NULL;
    new->trieId = malloc(sizeof(char) * (strlen(id) + 1));
    if (new->trieId == NULL) {
        free(new);
        return NULL;
    }
    for (size_t i = 0; i < strlen(id); i++) (new->trieId)[i] = id[i];
    (new->trieId)[strlen(id)] = '\0';
    new->trie = phfwdNew();
    if (new->trie == NULL) {
        free(new->trieId);
        free(new);
        return NULL;
    }
    return new;
}

void deleteTrieBase(trieBase *del) {
    if (del) {
        if (del->trieId) {
            free(del->trieId);
        }
        phfwdDelete(del->trie);
        free(del);
    }
}

trieBase *takeFromBehind(trieBase *prev) {
    assert (prev && prev->next);
    trieBase *result = prev->next;
    prev->next = result->next;
    return result;
}

void deleteTrieBaseList(trieBase **head) {
    trieBase *current = *head;
    trieBase *next;

    while (current != NULL) {
        next = current->next;
        deleteTrieBase(current);
        current = next;
    }
    *head = NULL;
}

trieBaseStructure *newTrieBaseStructure(void) {
    trieBaseStructure *new = malloc(sizeof(struct trieBaseStructure));
    if (new == NULL) {
        return NULL;
    }
    (new->head) = NULL;
    (new->act) = NULL;
    return new;
}

void deleteTrieBaseStructure(trieBaseStructure *del) {
    deleteTrieBaseList(&(del->head));
    free(del);
}