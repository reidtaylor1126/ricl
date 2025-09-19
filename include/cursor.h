#ifndef CURSOR_H
#define CURSOR_H

#include <stdio.h>
#include <stdint.h>

#define CURSOR_UP       "\033[1A"
#define CURSOR_DOWN     "\033[1B"
#define CURSOR_RIGHT    "\033[1C"
#define CURSOR_LEFT     "\033[1D"

void moveCursor(char* direction, uint8_t spaces);
void eraseNextN(uint8_t nCols);

#endif