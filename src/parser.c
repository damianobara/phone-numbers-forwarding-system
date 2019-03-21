/** @file
   * Implementacja interfejsu tekstowego programu.
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
#include <ctype.h>

#include "parser.h"
#include "vector.h"
#include "interface_operations.h"
#include "trie_base.h"
#include "errors.h"

#define ENTER '\n' ///<Enter
#define SPACE ' ' ///<Spacja
#define DOLAR '$' ///<Dolar
#define QUESTION '?' ///<Pytajnik
#define TO '>' ///<Operator przekierowania
#define AT '@' ///<Operator countNonTrivial
#define TAB '\t' ///< Tabulacja
#define CR 13 ///< Carriage return

//Funckcje typu bool sprawdzające czym jest następny token:

/**
 * Sprawdza czy c jest białym znakiem.
 * @param c - sprawdzany int.
 * @return true jeśli jest biały, else false.
 */
bool isWhite(int c) {
    if (c == ENTER) {
        return true;
    }
    if (c == SPACE) {
        return true;
    }
    if (c == TAB) {
        return true;
    }
    //Obsługujemy przypadek windowsowego zapisu końca linii.
    if (c == CR) {
        getchar();
        return true;
    }
    return false;
}
/**
 * Sprawdza czy 2 następne chary na wejściu to 2 dolary.
 * Zwraca error, jeśli któryś z nich jest EOF.
 * Zwraca error jeśli jest tylko 1 dolar.
 * @param byte - aktualny bajt wejścia.
 * @param delete - wskaźnik na tablicę charów, którą trzeba usunąć przed wyrzuceniem errora.
 * @return true, jeśli 2 następne chary na wejściu to 2 dolary, false wpp.
 */
bool isComment(const size_t *byte, char *delete) {

    int c1 = getchar();
    int c2 = getchar();
    if (c2 == EOF) {
        if (delete) {
            free(delete);
        }
        errorEOF();
    }

    ungetc(c2, stdin);
    ungetc(c1, stdin);

    if (c1 != DOLAR) {
        return false;
    } else if (c1 == DOLAR && c2 != DOLAR) {
        if (delete) {
            free(delete);
        }
        errorLexical((*byte) + 1);
    }
    return true;
}

/**
 * Sprawdza czy 2 następne chary na wejściu to 2 dolary.
 * Zwraca error, jeśli któryś z nich jest EOF.
 * Nie zwraca errora, jeśli jest tylko 1 dolar.
 * @param delete - wskaźnik na tablicę charów, którą trzeba usunąć przed wyrzuceniem errora.
 * @return true, jeśli 2 następne chary na wejściu to 2 dolary, false wpp.
 */
bool isCommentNoError(char *delete) {
    int c1 = getchar();
    int c2 = getchar();
    if (c2 == EOF) {
        if (delete) {
            free(delete);
        }
        errorEOF();
    }
    ungetc(c2, stdin);
    ungetc(c1, stdin);

    if (c1 == '$' && c2 == '$') {
        return true;
    }
    return false;
}

/**
* Sprawdza czy 2 następne chary na wejściu to 2 dolary.
* Nie zwraca errora, jeśli któryś z nich jest EOF.
* Nie zwraca errora, jeśli jest tylko 1 dolar.
* @return true, jeśli 2 następne chary na wejściu to 2 dolary, false wpp.
*/
bool are2NextCharDolars(void) {
    int c1 = getchar();
    if (c1 != '$' && c1 != EOF) {
        ungetc(c1, stdin);
        return false;
    }
    int c2 = getchar();

    ungetc(c2, stdin);
    ungetc(c1, stdin);

    if (c1 == '$' && c2 == '$') {
        return true;
    }
    return false;
};

/**
 * Sprawdza czy następne 4 znaki na wejściu to NEW znak biały lub komentarz.
 * @param byte - aktuany bajt wejścia.
 * @param delete - wskaźnik na tablicę charów, którą trzeba usunąć w przypadku errora.
 * @return true, jeśli następne 4 znaki na wejściu to NEW znak biały lub komentarz, false wpp.
 */
bool isNEW(size_t *byte, char *delete) {

    int c1 = EOF;
    int c2 = EOF;
    int c3 = EOF;

    c1 = getchar();
    if (c1 != 'N') {
        ungetc(c1, stdin);
        return false;
    }

    c2 = getchar();
    if (c2 != 'E') {
        ungetc(c2, stdin);
        ungetc(c1, stdin);
        return false;
    }
    c3 = getchar();
    if (c3 != 'W') {
        ungetc(c3, stdin);
        ungetc(c2, stdin);
        ungetc(c1, stdin);
        return false;
    }

    if ((c1 == 'N' && c2 == 'E') && c3 == 'W') {
        int c = getchar();
        if (c == EOF) {
            if (delete) {
                free(delete);
            }
            errorEOF();
        }
        ungetc(c, stdin);

        if (isWhite(c)) {
            *byte = *byte + 3;
            return true;
        } else if (isCommentNoError(delete)) {
            *byte = *byte + 3;
            return true;
        } else if ((!isdigit(c) && !isalpha(c)) && (c != ':' && c != ';')) {
            if (delete) {
                free(delete);
            }
            errorLexical(*byte + 3);
        }
    }
    if (c3 != EOF) {
        ungetc(c3, stdin);
    }
    if (c2 != EOF) {
        ungetc(c2, stdin);
    }
    if (c1 != EOF) {
        ungetc(c1, stdin);
    }
    return false;
}

/**
 * Sprawdza czy następne 4 znaki na wejściu to DEL znak biały lub komentarz.
 * @param byte - aktuany bajt wejścia.
 * @param delete - wskaźnik na tablicę charów, którą trzeba usunąć w przypadku errora.
 * @return true, jeśli następne 4 znaki na wejściu to NEW znak biały lub komentarz, false wpp.
 */
bool isDEL(size_t *byte, char *delete) {
    int c1 = EOF;
    int c2 = EOF;
    int c3 = EOF;

    c1 = getchar();
    if (c1 != 'D') {
        ungetc(c1, stdin);
        return false;
    }

    c2 = getchar();
    if (c2 != 'E') {
        ungetc(c2, stdin);
        ungetc(c1, stdin);
        return false;
    }
    c3 = getchar();
    if (c3 != 'L') {
        ungetc(c3, stdin);
        ungetc(c2, stdin);
        ungetc(c1, stdin);
        return false;
    }

    if ((c1 == 'D' && c2 == 'E') && c3 == 'L') {
        int c = getchar();
        ungetc(c, stdin);
        if (isWhite(c)) {
            *byte = *byte + 3;
            return true;
        } else if (isCommentNoError(delete)) {
            if (delete) {
                free(delete);
            }
            *byte = *byte + 3;
            return true;
        } else if ((!isdigit(c) && !isalpha(c)) && (c != ':' && c != ';')) {
            if (delete) {
                free(delete);
            }
            errorLexical(*byte + 3);
        } else if (c == EOF) {
            if (delete) {
                free(delete);
            }
            errorEOF();
        }
    }
    if (c3 != EOF) ungetc(c3, stdin);
    if (c2 != EOF) ungetc(c2, stdin);
    if (c1 != EOF) ungetc(c1, stdin);
    return false;
}

/**
 * Sprawdza czy następny znak z wejścia jest biały.
 * @return wartość logiczna: następny znak jest biały.
 */
bool isNextWhite(void) {
    int c = getchar();
    ungetc(c, stdin);
    if (c == ENTER) {
        return true;
    }
    if (c == SPACE) {
        return true;
    }
    if (c == TAB) {
        return true;
    }
    return false;
}

/**
 * Sprawdza czy następny znak z wejścia to EOF.
 * @return wartość logiczna: następny znak to EOF.
 */
bool isNextEOF(void) {
    int c = getchar();
    ungetc(c, stdin);
    if (c == EOF) {
        return true;
    }
    return false;
}

//Funckcje void wczytujące następny token:

/**
 * Wczytuje jeden znak z wejścia i zwiększa *bajt o 1.
 * @param byte - wskaźnik na bajt wejścia.
 */
void readOne(size_t *byte) {
    getchar();
    (*byte)++;
}

/**
 * Funkcja wczytująca komentarz, aż do następnych 2 dolarów
 * @param byte - wskaźnik na aktualny bajt wejścia.
 * @param delete - wskaźnik na tablicę charów, którą trzeba usunąć w przypadku errora.
 */
void readComment(size_t *byte, char *delete) {
    getchar();
    getchar();
    (*byte) += 2;
    while (!isNextEOF() && !isCommentNoError(delete)) {
        getchar();
        (*byte)++;
    }
    if (isNextEOF()) {
        getchar();
        if (delete) {
            free(delete);
        }
        errorEOF();
    }
    *byte = *byte + 2;
    getchar();
    getchar();
}

/**
 * Funkcja wczytująca numer, dopóki na wejściu dostaje cyfry.
 * @param byte - wskaźnik na aktualny bajt wejścia.
 */
char *readNumber(size_t *byte) {
    vector *v = malloc(sizeof(vector));
    if (!v) {
        exit(1);
    }
    bool ok = vectorInit(v);
    if (!ok) {
        vectorFree(v);
        exit(1);
    }
    int c = getchar();
    (*byte)++;
    while (isdigit(c) || (c == ':' || c == ';')) {
        (*byte)++;
        ok = vectorAdd(v, (char) c);
        if (!ok) {
            vectorFree(v);
            exit(1);
        }
        c = getchar();
    }
    ungetc(c, stdin);
    (*byte)--;
    return vectorGetAll(v);
}

/**
 * Funkcja wczytująca identyfikator, dopóki na wejściu dostaje cyfry lub litery.
 * @param byte - wskaźnik na aktualny bajt wejścia.
 */
char *readId(size_t *byte) {
    vector *v = malloc(sizeof(vector));
    if (!v) {
        exit(1);
    }
    bool ok = vectorInit(v);
    if (!ok) {
        vectorFree(v);
        exit(1);
    }
    int c = getchar();
    (*byte)++;
    while (isalpha(c) || isdigit(c)) {
        (*byte)++;
        ok = vectorAdd(v, (char) c);
        if (!ok) {
            vectorFree(v);
            exit(1);
        }
        c = getchar();
    }
    ungetc(c, stdin);
    (*byte)--;
    char *n = vectorGetAll(v);
    if (!n) {
        exit(1);
    }
    return n;
}

/**
 * Funkcja wszystkie białe znaki i komentarze z wejścia, dopóki nie pojawi się pierwszy inny char.
 * @param byte - wskaźnik na aktualny bajt wejścia.
 * @param del - wskaźnik na tablicę charów, którą trzeba usunąć w przypadku errora.
 */
void readWhiteOrComment(size_t *byte, char *del) {
    while (!isNextEOF() && (isNextWhite() || are2NextCharDolars())) {
        if (isNextWhite()) {
            readOne(byte);
        }
        else readComment(byte, del);
    }
}

//Funkcje void wczytujące do końca input w state oraz wykonujące go, jeśli jest poprawny:

/**
 * Funkcja wczytująca do końca input operacji DEL numer.
 * @param actTrie - wskaźnik na aktualną bazę przekierowań
 * @param secondNumber - wskaźnik na wskaźnik na char* reprezentujący numer.
 * @param firstByte - wskaźnik na bajt wystąpienia operatora.
 * @param byte - wskaźnik na aktualny bajt pamięci
 * @param state - Stan w którym jesteśmy.
 */
void readDelNumberInState(trieBase *actTrie, char **secondNumber, size_t firstByte, size_t *byte, State *state) {
    *secondNumber = readNumber(byte);
    if (*secondNumber == NULL) {
        exit(1);
    }
    *state = delNumber;

    bool corect = removeRedirectionFromTrieBase(actTrie, *secondNumber);
    free(*secondNumber);
    *secondNumber = NULL;
    if (!corect) {
        errorDelNumber(firstByte);
    }
}

/**
 * Funkcja wczytująca do końca input operacji DEL identyfikator.
 * @param actTrie - wskaźnik na aktualną bazę przekierowań.
 * @param trieHead - wskaźnik na bazę baz przekierowań.
 * @param firstId - wskaźnik na wskaźnik na char* reprezentujący identyfikator.
 * @param firstByte - bajt wystąpienia operatora.
 * @param byte - wskaźnik na aktualny bajt pamięci
 * @param state - Stan w którym jesteśmy.
 */
void readDelIdInState(trieBase **actTrie, trieBase **trieHead, char **firstId, size_t firstByte, size_t *byte,
                      State *state) {
    if (isNEW(byte, NULL)) {
        errorLexical((*byte));
    }
    if (isDEL(byte, NULL)) {
        errorLexical((*byte));
    }
    *firstId = readId(byte);
    *state = delId;
    bool corect = delIdFromTrieBase(actTrie, trieHead, *firstId);
    free(*firstId);
    *firstId = NULL;
    if (!corect) {
        errorDelId(firstByte);
    }

}

/**
 * Funkcja wczytująca do końca input operacji NEW identyfikator.
 * @param actTrie - wskaźnik na aktualną bazę przekierowań.
 * @param trieHead - wskaźnik na bazę baz przekierowań.
 * @param firstId - wskaźnik na wskaźnik na char* reprezentujący identyfikator.
 * @param firstByte - bajt wystąpienia operatora.
 * @param byte - wskaźnik na aktualny bajt pamięci
 * @param state - Stan w którym jesteśmy.
 */
void
readNewInState(trieBase **actTrie, trieBase **trieHead, char **firstId, size_t firstByte, size_t *byte, State *state) {
    int a = getchar();
    int b = getchar();
    ungetc(b, stdin);
    ungetc(a, stdin);
    readWhiteOrComment(byte, NULL);
    int c = getchar();
    ungetc(c, stdin);
    if (isNEW(byte, NULL)) {
        errorLexical((*byte));
    }
    if (isDEL(byte, NULL)) {
        errorLexical((*byte));
    }
    if (!isalpha(c)) {
        errorLexical((*byte));
    }
    else {
        *firstId = readId(byte);
        *state = newId;
        bool corect = addIdToTrieBase(actTrie, trieHead, *firstId);
        free(*firstId);
        *firstId = NULL;
        if (!corect) {
            errorNewId(firstByte);
        }
    }
}

/**
 * Funkcja wczytująca do końca input operacji ? numer.
 * @param actTrie - wskaźnik na aktualną bazę przekierowań
 * @param secondNumber - wskaźnik na wskaźnik na char* reprezentujący numer.
 * @param firstByte - bajt wystąpienia operatora.
 * @param byte - wskaźnik na aktualny bajt pamięci
 * @param state - Stan w którym jesteśmy.
 */
void readQuestionInState(trieBase **actTrie, char **secondNumber, size_t firstByte, size_t *byte, State *state) {
    readWhiteOrComment(byte, NULL);
    int c = getchar();
    ungetc(c, stdin);
    if (c == EOF) {
        errorEOF();
    }
    if (!isdigit(c) && (c != ':' && c != ';')) {
        errorLexical(*byte);
    } else {
        *secondNumber = readNumber(byte);
        if (*secondNumber == NULL) {
            exit(1);
        }
        *state = questionNumber;
        bool corect = reverseRediorectionFromTrieBase(*actTrie, *secondNumber);
        free(*secondNumber);
        *secondNumber = NULL;
        if (!corect) {
            errorQuestionNumber(firstByte);
        }
    }
}

/**
 * Funkcja wczytująca do końca input operaci numer ?.
 * @param actTrie - wskaźnik na aktualną bazę przekierowań
 * @param firstNumber - wskaźnik na wskaźnik na char* reprezentujący numer.
 * @param firstByte - bajt wystąpienia operatora.
 * @param state - Stan w którym jesteśmy.
 */
void readNumberQuestionInState(trieBase **actTrie, char **firstNumber, size_t firstByte, State *state) {
    *state = numberQuestion;
    bool corect = getRedirectionFromTrieBase(*actTrie, *firstNumber);
    free(*firstNumber);
    *firstNumber = NULL;
    if (!corect) {
        errorNumberQuestion(firstByte);
    }
}

/**
 * Funkcja wczytująca do końca input operaci numer > numer.
 * @param actTrie - wskaźnik na aktualną bazę przekierowań
 * @param firstNumber - wskaźnik na wskaźnik na char* reprezentujący numer.
 * @param secondNumber - wskaźnik na wskaźnik na char* reprezentujący numer.
 * @param firstByte - wskaźnik na bajt wystąpienia operatora.
 * @param byte - wskaźnik na aktualny bajt pamięci
 * @param state - Stan w którym jesteśmy.
 */
void
readNumberToNumberInState(trieBase **actTrie, char **firstNumber, char **secondNumber, size_t firstByte, size_t *byte,
                          State *state) {
    readWhiteOrComment(byte, *firstNumber);
    int c = getchar();
    ungetc(c, stdin);
    if (!isdigit(c) && (c != ':' && c != ';')) {
        free(*firstNumber);
        if (*byte == 11) {
            errorLexical((*byte));
        }
        errorLexical((*byte) + 1);
    } else {
        *secondNumber = readNumber(byte);
        if (*secondNumber == NULL) {
            free(firstNumber);
            exit(1);
        }
        bool corect = addRedirectionToTrieBase(*actTrie, *firstNumber, *secondNumber);
        free(*firstNumber);
        *firstNumber = NULL;
        free(*secondNumber);
        *secondNumber = NULL;
        if (!corect) {
            errorNumberToNumber(firstByte);
        }
        *state = numberToNumber;
    }
}

/**
* Funkcja wczytująca do końca input operaci @ numer.
* @param actTrie - wskaźnik na aktualną bazę przekierowań
* @param secondNumber - wskaźnik na wskaźnik na char* reprezentujący numer.
* @param byte - wskaźnik na aktualny bajt pamięci
* @param state - Stan w którym jesteśmy.
*/
void readAtNumberInState(trieBase **actTrie, char **secondNumber, size_t *byte,
                         State *state) {
    readWhiteOrComment(byte, NULL);
    int c = getchar();
    ungetc(c, stdin);
    if (c == EOF) {
        errorEOF();
    }
    if (!isdigit(c) && (c != ':' && c != ';')) {
        errorLexical(*byte);
    }
    else {
        *secondNumber = readNumber(byte);
        if (*secondNumber == NULL) exit(1);
        *state = atNumber;
        countNonTrivialInTreeBase(*actTrie, *secondNumber);
        free(*secondNumber);
        *secondNumber = NULL;
    }
}

//Główna funkcja wczytująca input.
State readToState(State prev, size_t *prevByte, trieBase **actTrie, trieBase **trieHead) {
    size_t byte = *prevByte;
    Token token = def;
    size_t initbyte = byte;
    State state;

    int c = getchar();
    ungetc(c, stdin);

    if (((!isWhite(c) && c != DOLAR) && (c != QUESTION && !isdigit(c))) &&
        (((c != ':' && c != ';') && (!isalpha(c)) && c != EOF) && c != AT)) {
        errorLexical(initbyte);
    }

    if (c == EOF) {
        token = endToken;
    } else if (isdigit(c) || (c == ':' || c == ';')) {
        token = number;
    } else if (isWhite(c)) {
        token = white;
        readOne(&byte);
        readWhiteOrComment(&byte, NULL);
    } else if (isComment(&byte, NULL)) {
        token = white;
        readComment(&byte, NULL);
    } else if (c == QUESTION) {
        token = question;
        getchar();
        byte++;
    } else if (c == TO) {
        token = to;
        errorLexical(byte);
    } else if (isNEW(&byte, NULL)) {
        token = NEW;
    }
    else if (isDEL(&byte, NULL)) {
        token = DEL;
    }
    else if (isalpha(c)) {
        token = id;
    } else if (c == AT) {
        token = at;
        getchar();
        byte++;
    } else {
        errorLexical(byte);
    }

    Token second = def;
    char *firstNumber = NULL;
    char *secondNumber = NULL;
    char *firstId = NULL;

    if (token == DEL) {
        readWhiteOrComment(&byte, NULL);
        c = getchar();
        ungetc(c, stdin);
        if (isdigit(c) || (c == ':' || c == ';')) {
            second = number;
        }
        else if (isalpha(c)) {
            second = id;
        }
        else {
            errorLexical(byte);
        }
    }

    if (token == number) {
        firstNumber = readNumber(&byte);
        if (firstNumber == NULL) {
            exit(1);
        }
        readWhiteOrComment(&byte, firstNumber);
        c = getchar();
        byte++;
        if (c == QUESTION) {
            second = question;
        }
        else if (c == TO) {
            second = to;
        }
        else if (c == EOF) {
            free(firstNumber);
            firstNumber = NULL;
            errorEOF();
        } else {
            free(firstNumber);
            firstNumber = NULL;
            errorLexical(byte);
        }
    }

    switch (prev) {
        case empty:
            if (token == DEL && second == number) {
                readDelNumberInState(*actTrie, &secondNumber, initbyte, &byte, &state);
            }
            else if (token == DEL && second == id) {
                readDelIdInState(actTrie, trieHead, &firstId, initbyte, &byte, &state);
            }
            else if (token == id) {
                errorLexical(byte);
            }
            else if (token == white) {
                state = empty;
            }
            else if (token == NEW) {
                readNewInState(actTrie, trieHead, &firstId, initbyte, &byte, &state);
            }
            else if (token == to) {
                errorLexical(byte);
            }
            else if (token == question) {
                readQuestionInState(actTrie, &secondNumber, initbyte, &byte, &state);
            }
            else if (token == endToken) {
                state = end;
            }
            else if (token == number && second == question) {
                readNumberQuestionInState(actTrie, &firstNumber, byte - 1, &state);
            }
            else if (token == number && second == to) {
                readNumberToNumberInState(actTrie, &firstNumber, &secondNumber, byte - 1, &byte, &state);
            }
            else if (token == at) {
                readAtNumberInState(actTrie, &secondNumber, &byte, &state);
            }
            break;
        case delNumber:

            if (token == question) {
                readQuestionInState(actTrie, &secondNumber, initbyte, &byte, &state);
            }
            else if (token == NEW) {
                readNewInState(actTrie, trieHead, &firstId, initbyte, &byte, &state);
            }
            else if (token == DEL && second == number) {
                readDelNumberInState(*actTrie, &secondNumber, initbyte, &byte, &state);
            }
            else if (token == DEL && second == id) {
                readDelIdInState(actTrie, trieHead, &firstId, initbyte, &byte, &state);
            }
            else if (token == white) {
                state = empty;
            }
            else if (token == endToken) {
                state = end;
            }
            else if (token == at) {
                readAtNumberInState(actTrie, &secondNumber, &byte, &state);
            }
            else {
                if (firstNumber) {
                    free(firstNumber);
                }
                errorLexical(initbyte);
            }
            break;

        case delId:
            if (token == question) {
                readQuestionInState(actTrie, &secondNumber, initbyte, &byte, &state);
            }
            else if (token == white) {
                state = empty;
            }
            else if (token == endToken) {
                state = end;
            }
            else if (token == at) {
                readAtNumberInState(actTrie, &secondNumber, &byte, &state);
            }
            else {
                if (firstNumber) {
                    free(firstNumber);
                }
                errorLexical(initbyte);
            }
            break;

        case newId:
            if (token == white) {
                state = empty;
            }
            else if (token == question) {
                readQuestionInState(actTrie, &secondNumber, initbyte, &byte, &state);
            }
            else if (token == at) {
                readAtNumberInState(actTrie, &secondNumber, &byte, &state);
            }
            else if (token == endToken) {
                state = end;
            }
            else {
                if (firstNumber) {
                    free(firstNumber);
                }
                errorLexical(initbyte);
            }
            break;

        case numberToNumber:
            if (token == white) {
                state = empty;
            }
            else if (token == question) {
                readQuestionInState(actTrie, &secondNumber, initbyte, &byte, &state);
            }
            else if (token == NEW) {
                readNewInState(actTrie, trieHead, &firstId, initbyte, &byte, &state);
            }
            else if (token == DEL && second == id) {
                readDelIdInState(actTrie, trieHead, &firstId, initbyte, &byte, &state);
            }
            else if (token == DEL && second == number) {
                readDelNumberInState(*actTrie, &secondNumber, initbyte, &byte, &state);
            }
            else if (token == endToken) {
                state = end;
            }
            else if (token == at) {
                readAtNumberInState(actTrie, &secondNumber, &byte, &state);
            }
            else {
                if (firstNumber) {
                    free(firstNumber);
                }
                errorLexical(initbyte);
            }
            break;

        case numberQuestion:
            if (token == DEL && second == number) {
                readDelNumberInState(*actTrie, &secondNumber, initbyte, &byte, &state);
            }
            else if (token == DEL && second == id) {
                readDelIdInState(actTrie, trieHead, &firstId, initbyte, &byte, &state);
            }
            else if (token == id) {
                errorLexical(byte + 1);
            }
            else if (token == white) {
                state = prev;
            }
            else if (token == NEW) {
                readNewInState(actTrie, trieHead, &firstId, initbyte, &byte, &state);
            }
            else if (token == to) {
                errorLexical(byte);
            }
            else if (token == question) {
                readQuestionInState(actTrie, &secondNumber, initbyte, &byte, &state);
            }
            else if (token == endToken) {
                state = end;
            }
            else if (token == at) {
                readAtNumberInState(actTrie, &secondNumber, &byte, &state);
            }
            else if (token == number && second == question) {
                readNumberQuestionInState(actTrie, &firstNumber, byte - 1, &state);
            }
            else if (token == number && second == to) {
                readNumberToNumberInState(actTrie, &firstNumber, &secondNumber, byte - 1, &byte, &state);
            }
            break;

        case questionNumber:
            if (token == question) {
                readQuestionInState(actTrie, &secondNumber, initbyte, &byte, &state);
            }
            else if (token == NEW) {
                readNewInState(actTrie, trieHead, &firstId, initbyte, &byte, &state);
            }
            else if (token == DEL && second == number) {
                readDelNumberInState(*actTrie, &secondNumber, initbyte, &byte, &state);
            }
            else if (token == DEL && second == id) {
                readDelIdInState(actTrie, trieHead, &firstId, initbyte, &byte, &state);
            }
            else if (token == white) {
                state = empty;
            }
            else if (token == endToken) {
                state = end;
            }
            else if (token == at) {
                readAtNumberInState(actTrie, &secondNumber, &byte, &state);
            }
            else {
                if (firstNumber) {
                    free(firstNumber);
                }
                errorLexical(initbyte);
            }
            break;

        case atNumber:
            if (token == question) {
                readQuestionInState(actTrie, &secondNumber, initbyte, &byte, &state);
            }
            else if (token == NEW) {
                readNewInState(actTrie, trieHead, &firstId, initbyte, &byte, &state);
            }
            else if (token == DEL && second == number) {
                readDelNumberInState(*actTrie, &secondNumber, initbyte, &byte, &state);
            }
            else if (token == DEL && second == id) {
                readDelIdInState(actTrie, trieHead, &firstId, initbyte, &byte, &state);
            }
            else if (token == white) {
                state = empty;
            }
            else if (token == endToken) {
                state = end;
            }
            else if (token == at) {
                readAtNumberInState(actTrie, &secondNumber, &byte, &state);
            }
            else {
                if (firstNumber) {
                    free(firstNumber);
                }
                errorLexical(initbyte);
            }
            break;

        case end:
            break;
    }
    *prevByte = byte;
    return state;
}
