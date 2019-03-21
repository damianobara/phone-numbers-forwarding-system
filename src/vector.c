/** @file
   * Implementacja pomocniczej biblioteki wektorów.
   * @author Damian Obara
   * @copyright Uniwersytet Warszawski
   * @date 14.06.2018
   *
   */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "vector.h"

bool vectorInit(vector *v) {
    v->capacity = VECTOR_INIT_CAPACITY;
    v->size = 0;
    v->data = malloc(sizeof(char) * v->capacity);
    if (v->data) {
        return true;
    }
    return false;
}

bool vectorResize(vector *v, size_t capacity) {

    char *items = realloc(v->data, sizeof(size_t) * capacity);
    if (items) {
        v->data = items;
        v->capacity = capacity;
        return true;
    }
    return false;
}

bool vectorAdd(vector *v, char item) {
    bool ok = true;
    if (v->capacity == v->size) {
        ok = vectorResize(v, v->capacity * 2);
    }
    if (ok) {
        v->data[v->size++] = item;
    }
    return ok;
}

char *vectorGetAll(vector *v) {
    char *result = malloc(sizeof(size_t) * (v->size + 1));
    if (!result) {
        return NULL;
    }
    for (size_t i = 0; i < v->size; i++) result[i] = (v->data)[i];
    result[v->size] = '\0';
    vectorFree(v);
    return result;
}

void vectorFree(vector *v) {
    free(v->data);
    free(v);
}