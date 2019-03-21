/** @file
   * Implementacja funkcji main programu.
   *
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   */

#include <stdio.h>

#include "phone_forward.h"
#include "parser.h"
#include "trie_base.h"
#include "interface_operations.h"
#include "errors.h"

/**
 * Funcja tworzy bazę baz przekierowań a następnie wykonuje operacje na numerach zdefiniowane w interfejsie tekstowym
 * Pod koniec działania programu zwalnia pamięć.
 * @return 0
 */
int main() {

    trieBaseStructure *center = newTrieBaseStructure();
    atExitCleanTrieBaseStructure(center);
    State state = empty;
    size_t byte = 1;
    while (state != end) {
        state = readToState(state, &byte, &(center->act), &(center->head));
    }
    deleteTrieBaseList(&(center->head));

    return 0;
}