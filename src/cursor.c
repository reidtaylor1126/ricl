#include "cursor.h"

void clearScreen() {
    printf("\033[2J");
}

void moveCursorTo(uint8_t x, uint8_t y) {
        printf("\033[%hu;%huH", y, x);
}

void eraseNextN(uint8_t nCols) {
    for (int i = 0; i < nCols; i++)
        printf(" ");
    fflush(stdout);
    printf("\033[%huD", nCols);
}

void printBlockAt(char* buffer, uint8_t x, uint8_t y, uint8_t nRows, uint8_t cPerRow) {
    for (int r = 0; r < nRows; r++) {
        printf("\033[%hu;%huH", y+r, x);
        for (int c = 0; c < cPerRow; c++) {
            putchar(buffer[(r*cPerRow)+c]);
        }
    }
}
