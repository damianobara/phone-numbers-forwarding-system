/** @file
   * Implementacja funkcji wykonujących operacje interfejsu.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "phone_numbers.h"
#include "interface_operations.h"
#include "trie_base.h"
#include "phone_forward.h"
#include "non_trivial.h"

bool addIdToTrieBase(trieBase **act, trieBase **head, char *id) {
    //Przypadek, gdy nie ma aktualnej bazy przekierowań.
    if ((*head) == NULL) {
        *head = newTrieBase(id);
        *act = *head;
        if (*act == NULL) {
            return false;
        }
        return true;
    }
    //Przypadek, gdy baza przekierowań, którą chcemy ustawić na aktualną jest już na nią ustawiona.
    if (strcmp((*head)->trieId, id) == 0) {
        *act = *head;
        return true;
    }
    //Przypadek gdy jest tylko jedno drzewo przekierowań
    if ((*head)->next == NULL) {
        trieBase *newHead = newTrieBase(id);
        newHead->next = *head;
        *head = newHead;
        *act = *head;
        return true;
    }

    trieBase *prev = *head;
    trieBase *help = prev->next;

    //Sprawdzamy czy baza o identyfikatorze id jest już w bazie baz.
    while (help->next != NULL && strcmp(help->trieId, id) != 0) {
        prev = prev->next;
        help = help->next;
    }

    if (strcmp(help->trieId, id) == 0) {
        *act = help;
        return true;
    }

    trieBase *newHead = newTrieBase(id);
    if (newHead == NULL) {
        return false;
    }
    newHead->next = (*head);
    *head = newHead;
    *act = *head;
    return true;
}

bool delIdFromTrieBase(trieBase **act, trieBase **head, char *id) {
    //Przypadek gdy nie ma żadnej bazy przekierowań
    if ((*head) == NULL) {
        return false;
    }
    //Przypadek, gdy baza o identyfikatorze id jest na początku listy baz
    if (strcmp((*head)->trieId, id) == 0) {
        trieBase *help = (*head)->next;
        if ((*act) && strcmp((*head)->trieId, (*act)->trieId) == 0) {
            *act = NULL;
        }
        deleteTrieBase(*head);
        *head = help;
        return true;
    }
    //Przypadek, gdy jest tylko jedna baza
    if ((*head)->next == NULL) {
        return false;
    }

    trieBase *prev = *head;
    trieBase *help = prev->next;

    //Sprawdzamy czy baza o identyfikatorze id jest już w bazie baz.
    while (help->next != NULL && strcmp(help->trieId, id) != 0) {
        prev = prev->next;
        help = help->next;
    }

    if (strcmp(help->trieId, id) == 0) {
        if ((*act) && strcmp(id, (*act)->trieId) == 0) {
            *act = NULL;
        }
        trieBase *del = takeFromBehind(prev);
        deleteTrieBase(del);
        return true;
    }
    return false;
}

bool addRedirectionToTrieBase(trieBase *root, char *number1, char *number2) {
    if (root == NULL) {
        return false;
    }
    return phfwdAdd(root->trie, number1, number2);

}

bool getRedirectionFromTrieBase(trieBase *root, char *number) {
    if (root == NULL) {
        return false;
    }
    const struct PhoneNumbers *result = phfwdGet(root->trie, number);
    if (result == NULL) {
        return false;
    }
    if (result->number == NULL) {
        return false;
    }
    if (result->size == 0) {
        return false;
    }

    const char *num = NULL;
    size_t idx = 0;
    while ((num = phnumGet(result, idx++)) != NULL) {
        printf("%s\n", num);
    }
    phnumDelete(result);
    return true;
}

bool reverseRediorectionFromTrieBase(trieBase *root, char *number) {
    if (root == NULL) {
        return false;
    }
    const struct PhoneNumbers *result = phfwdReverse(root->trie, number);
    if (result == NULL) {
        return false;
    }
    if (result->number == NULL) {
        return false;
    }
    if (result->size == 0) {
        return false;
    }

    const char *num = NULL;
    size_t idx = 0;
    while ((num = phnumGet(result, idx++)) != NULL) {
        printf("%s\n", num);
    }
    phnumDelete(result);
    return true;
}

bool removeRedirectionFromTrieBase(trieBase *root, char *number) {
    if (root == NULL) {
        return false;
    }
    phfwdRemove(root->trie, number);
    return true;
}

void countNonTrivialInTreeBase(trieBase *root, char *number) {
    size_t len;
    if (12 < strlen(number)) {
        len = strlen(number) - 12;
    } else len = 0;
    size_t result = phfwdNonTrivialCount(root->trie, number, len);
    printf("%zu\n", result);
}