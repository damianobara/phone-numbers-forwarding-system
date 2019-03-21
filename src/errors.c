/** @file
   * Implementacja funkcji zwracających błędy.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parser.h"
#include "interface_operations.h"
#include "trie_base.h"

void errorLexical(size_t byte) {
    fprintf(stderr, "ERROR %zu\n", byte);
    exit(1);
}

void errorEOF(void) {
    fprintf(stderr, "ERROR EOF\n");
    exit(1);
}

void errorNewId(size_t initByte) {
    fprintf(stderr, "ERROR NEW %zu\n", initByte);
    exit(1);
}

void errorDelId(size_t initByte) {
    fprintf(stderr, "ERROR DEL %zu\n", initByte);
    exit(1);
}

void errorNumberToNumber(size_t initByte) {
    fprintf(stderr, "ERROR > %zu\n", initByte);
    exit(1);
}

void errorQuestionNumber(size_t initByte) {
    fprintf(stderr, "ERROR ? %zu\n", initByte);
    exit(1);
}

void errorNumberQuestion(size_t initByte) {
    fprintf(stderr, "ERROR ? %zu\n", initByte);
    exit(1);
}

void errorDelNumber(size_t initByte) {
    fprintf(stderr, "ERROR DEl %zu\n", initByte);
    exit(1);
}

void atExitCleanTrieBaseStructure(trieBaseStructure *center);

/**
 * Statyczna funkcja systemowa do zwalniania pamięci przy exit().
 */
static void clean(void) {
    atExitCleanTrieBaseStructure(NULL);
}

// Dodaje rzeczy do usunięcia przy wychodzeniem z programu.
void atExitCleanTrieBaseStructure(trieBaseStructure *center) {
    static void *x;
    if (center) {
        x = center;
        atexit(clean);
    } else {
        deleteTrieBaseStructure(x);
    }
}