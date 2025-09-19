#include "cursor.h"

void moveCursor(char* direction, uint8_t spaces) {
    for (int i = 0; i < spaces; i++) {
        printf("%s", direction);
    }
}

void eraseNextN(uint8_t nCols) {
    for (int i = 0; i < nCols; i++)
        printf(" ");
    fflush(stdout);
    printf("\033[%huD", nCols);
}