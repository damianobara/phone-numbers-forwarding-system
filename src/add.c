/** @file
   * Implementacja funkcji phfwdAdd(struct PhoneForward *pf, char const *num1, char const *num2) oraz jej funkcji pomocniczych.
   *
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#include "phone_numbers.h"
#include "list_util.h"
#include "phone_forward.h"

//Funkcja tworzy i zwraca wskaźnik na korzeń drzewa przekierowań numerów telefonów.
struct PhoneForward *phfwdNew(void) {
    struct PhoneForward *new = malloc(sizeof(struct PhoneForward));
    if (!new) {
        return NULL;
    }
    new->key = 'a';
    for (size_t j = 0; j < 12; j++) new->sons[j] = NULL;
    new->redirTo = NULL;
    new->redirFrom = NULL;
    new->myPlace = NULL;

    return new;
}

//Funkcja usuwająca listę phoneNumberList. Po jej użyciu należy zawsze ustawić act->myPlace na NULL.
void deleteMyPlace(struct PhoneForward *act) {
    assert (act);
    if (act->myPlace) {
        struct phoneNumberList *my = act->myPlace;
        if (my->prev) {
            //Na początku listy zawsze jest atrapa.
            ((act->myPlace)->prev)->next = my->next;
        }
        if (my->next) {
            (my->next)->prev = my->prev;
        }
        if (my->key) {
            free(my->key);
            my->key = NULL;
        }
        free(my);
    }
}


/* Usuwa rekurencyjnie całe poddrzewo o korzeniu w act. Funkcja jest używana przy nieprawidłowej alokacji,
 * by usunąć wszystkie zaalokowane do tej pory węzły.*/
void phfwdDelete(struct PhoneForward *act) {
    if (act) {
        //Usuwamy poddrzewa o korzeniach w węzłąch będąch dziećmi aktualnego węzła.
        for (size_t i = 0; i < 12; i++) {
            phfwdDelete(act->sons[i]);
        }
        act->myPlace = NULL;
        //Zwalaniamy wszystkie pola usuwanego węzła.
        if (act->redirFrom) {
            deleteList(&(act->redirFrom));
        }
        if (act->redirTo) {
            free(act->redirTo);
            act->redirTo = NULL;
        }
        free(act);
    }
}

/**
 * @brief Tworzy nową strukturę i inicjuje jej pola.
 * Tworzy nową strukturę.
 * Inicjuje wskaźnik key na num[i].
 * Inicjuje wskaźnik redirTo na NULL.
 * Tworzy strukturę phoneNumberList i inicjuje jej wskaźnik key na napis num o długości i.
 * Inicjuje wskaźnik redirFrom na tą strukturę.
 * @param num - wskaźnik na napis reprezentujący numer
 * @param i - liczba reprezentująca długość prefixu num, który będzie nas interesował.
 * @return wskaźnik na strukturę. NULL jeśli allokacja się nie udała.
 */
struct PhoneForward *mallocPhoneForward(char const *num, size_t i) {

    struct PhoneForward *new;
    struct phoneNumberList *r;
    struct phoneNumberList *hisNumber;
    char *hisKey;

    new = malloc(sizeof(struct PhoneForward));
    r = malloc(sizeof(struct phoneNumberList));
    hisNumber = malloc(sizeof(struct phoneNumberList));

    hisKey = malloc(sizeof(char) * (i + 2));

    if ((!new || !r) || (!hisNumber || !hisKey)) {
        if (new) {
            free(new);
        }
        if (r) {
            free(r);
        }
        if (hisNumber) {
            free(hisNumber);
        }
        if (hisKey) {
            free(hisKey);
        }

        return false;
    }

    //Zerujemy wszystkie pola w nowym Node'ie.
    new->key = num[i];
    for (size_t j = 0; j < 12; j++) new->sons[j] = NULL;
    new->redirTo = NULL;
    new->redirFrom = NULL;
    new->myPlace = NULL;
    //Koniec zerowania.

    r->prev = NULL;
    r->key = NULL;
    r->next = hisNumber;

    //Kopiujemy numer reprezentowany przez char *num.
    strncpy(hisKey, num, i + 1);
    hisKey[i + 1] = '\0';

    hisNumber->prev = r;
    hisNumber->key = hisKey;
    hisNumber->next = NULL;

    new->redirFrom = r;

    return new;
}

/* Wyznacza węzeł reprezentujący prefix *num. Jeśli taki węzeł nie istnieje, to dodaje do drzewa wszystkie węzły reprezentujące
 * jego prefiksy oraz węzeł reprezentujący numer num.
 * Zwraca wskaźnik na szkujany węzeł.
 * Jeśli do drzewa były dodawane nowe węzły to modyfikuje również **whereAdd
 * struct PhoneForward **whereAdd - wskaźnik na miejsce, w które należy dopiąć nowe drzewo
 * struct PhoneForward **root - wskaźnik na korzeń nowo budowanego drzewa
 * int *son - w miejsce którego syna należy podpiąć *root do *whereAdd
 * Jeśli droga nie byłą budowana (istniała już wcześniej parametry te pozostają ustawione odpowiednio na NULL, NULL, -1
 * */

/**
 * @brief Znajduje strukturę reprezentującą napis num. Jeśli nie istnieje to ją tworzy.
 * W pętli szuka drogi do struktury reprezentującej napis. Jeśli struktura ta nie istnieje,
 * to tworzy ją oraz struktury reprezentujące wszystkie prefixy numeru num, które nie istnieją,
 * tworząc drzewo trie.
 * Po zakończeniu funkcji należy podpiąć podpiąć nowo powstałe drzewo przekierowań do głównego drzewa.
 * @param pf - wskaźnik na strukturę reprezentującą przekierowania numerów.
 * @param num - wskaźnik na napis reprezentujący poszukiwany numer.
 * @param whereAdd - wskaźnik na miejsce, w które należy dopiąć nowe drzewo.
 * @param root - wskaźnik na korzeń nowo budowanego drzewa.
 * @return wskaźnik na poszukiwaną strukturę.
 */
struct PhoneForward *
buildPrefix(struct PhoneForward *pf, char const *num, struct PhoneForward **whereAdd, struct PhoneForward **root) {

    struct PhoneForward *actNode = pf;
    size_t i = 0;
    size_t len = strlen(num);

    /* Idziemy do węzła reprezentującego napis num, dopóki w drzewie przekierowań są węzły
     * reprezentujące jego prefixy.*/
    while (i < len && (actNode->sons)[num[i] - '0'] != NULL) {
        actNode = (actNode->sons)[num[i] - '0'];
        i++;
    }
    //Jeśli węzeł ten jest już w drzewie przekierowań to go zwracamy.
    if (i == len) {
        return actNode;
    }
        /* Jeśli węzła tego nie ma w drzewie to dodajemy do drzewa wszystkie węzły reprezentujące prefixy napisu num
         * nienależące do drzewa oraz sam węzeł reprezentujący napis num.*/
    else {
        *whereAdd = actNode;
        actNode = mallocPhoneForward(num, i);
        if (actNode == NULL) {
            return NULL;
        } else {
            *root = actNode;
        }
        i++;
        //Pętla dodająca węzły reprezentujące pozostałe prefisky do drzewa.
        struct PhoneForward *nextNode;
        while (i < len) {
            nextNode = mallocPhoneForward(num, i);
            if (nextNode == NULL) {
                phfwdDelete(*root);
                return NULL;
            } else {
                (actNode->sons)[num[i] - '0'] = nextNode;
                actNode = nextNode;
            }
            i++;
        }
        return actNode;
    }
}

bool phfwdAdd(struct PhoneForward *pf, char const *num1, char const *num2) {

    //Sprawdzamy czy parametry funkcji są poprawne.
    if (!pf) {
        return false;
    }
    if (!num1 || !num2) {
        return false;
    }
    //Sprawdzamy czy num1 i num2 nie są takie same.
    if (strcmp(num1, num2) == 0) {
        return false;
    }

    size_t i, j;
    size_t len1, len2;
    len1 = strlen(num1);
    len2 = strlen(num2);
    for (i = 0; i < len1 && (isdigit(num1[i]) || (num1[i] == ':' || num1[i] == ';')); i++);
    for (j = 0; j < len2 && (isdigit(num2[j]) || (num2[j] == ':' || num2[j] == ';')); j++);

    //Sprawdzamy przypadek stringów pustych.
    if (len1 == 0 || len2 == 0) {
        return false;
    }
    //Sprawdzamy czy w obu stringach są tylko cyfry i null (czy są liczbami).
    else if (i != len1 || j != len2) {
        return false;
    }
    else {
        //Wszystkie rzeczy które trzeba będzie pododawać po udanych mallocach w pierwszym poddrzewie.
        struct PhoneForward *root = NULL;
        struct PhoneForward *whereAddAfterMalloc = NULL;
        char *redirectionTo = NULL;
        struct PhoneForward *prefix1 = NULL;

        //Dodajemy do drzewa węzły reprezentujące wszystkie niedodane jeszcze prefixy num1.
        prefix1 = buildPrefix(pf, num1, &whereAddAfterMalloc, &root);
        redirectionTo = malloc(sizeof(char) * (1 + len2));

        //Sprawdzamy, czy nie zabrakło pamięci.
        if (!prefix1 || !redirectionTo) {
            if (prefix1) {
                free(prefix1);
            }
            if (redirectionTo) {
                free(redirectionTo);
            }
            return false;
        }

        //Jeśli istnieje jakieś przekierowanie dla num1 to je usuwamy.
        if (prefix1->redirTo) {
            free(prefix1->redirTo);
            deleteMyPlace(prefix1);
            prefix1->myPlace = NULL;
        }

        //Kopiujemy num2 do redirTo.
        strcpy(redirectionTo, num2);

        //Podpinamy wszystko co się da.
        prefix1->redirTo = redirectionTo;
        if (whereAddAfterMalloc && root) {
            (whereAddAfterMalloc->sons)[(root->key) - '0'] = root;
        }

        //Wszystkie rzeczy, które trzeba pododawać po udanych malocach w drugim drzewie.
        struct PhoneForward *root2 = NULL;
        struct PhoneForward *whereAddAfterMalloc2 = NULL;
        struct phoneNumberList *redirectionFrom = NULL;
        char *redirectionKey = NULL;
        struct PhoneForward *prefix2 = NULL;

        //Dodajemy do drzewa węzły reprezentujące wszystkie niedodane jeszcze prefixy num2.
        prefix2 = buildPrefix(pf, num2, &whereAddAfterMalloc2, &root2);
        redirectionFrom = malloc(sizeof(struct phoneNumberList));
        redirectionKey = malloc((len1 + 1) * sizeof(char));

        //Przypadek nieudanej alokacji.    
        if ((!prefix2 || !redirectionFrom) || !redirectionKey) {
            if (prefix2) {
                free(prefix2);
            }
            if (redirectionFrom) {
                free(redirectionFrom);
            }
            if (redirectionKey) {
                free(redirectionKey);
            }
            phfwdDelete(root);
            (whereAddAfterMalloc->sons)[(root->key) - '0'] = NULL;
            root = NULL;
            return false;
        }
        strcpy(redirectionKey, num1);

        //Podpinamy redirection do redirFrom.
        redirectionFrom->key = redirectionKey;
        redirectionFrom->next = (prefix2->redirFrom)->next;
        if (redirectionFrom->next) {
            (redirectionFrom->next)->prev = redirectionFrom;
        }
        (prefix2->redirFrom)->next = redirectionFrom;
        redirectionFrom->prev = prefix2->redirFrom;

        //Podpinamy myPlace.
        prefix1->myPlace = redirectionFrom;

        //Podpianamy podrzewo prefiksów num2.
        if (whereAddAfterMalloc2 && root2) {
            (whereAddAfterMalloc2->sons)[(root2->key) - '0'] = root2;
        }
        return true;
    }
}
