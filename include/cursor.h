#ifndef CURSOR_H
#define CURSOR_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define N_COLS (84)
#define N_ROWS (28)

#define CURSOR_UP       "\033[%dA"
#define CURSOR_DOWN     "\033[%dB"
#define CURSOR_RIGHT    "\033[%dC"
#define CURSOR_LEFT     "\033[%dD"

void clearScreen();
void moveCursorTo(uint8_t x, uint8_t y);
void eraseNextN(uint8_t nCols);
void printBlockAt(char* buffer, uint8_t x, uint8_t y, uint8_t nRows, uint8_t cPerRow);
uint8_t inputInt(char* prompt, uint8_t clearCols);

#endif