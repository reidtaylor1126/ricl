#ifndef TILES_H

#define TILES_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_YELLOW      "\x1b[93m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define IS_AKA (128)
#define IS_HONOR (64)
#define IS_DRAGON (80)
#define GREEN_DRAGON (IS_DRAGON+1)
#define RED_DRAGON (IS_DRAGON+2)
#define IS_WIND (96)
#define EAST_WIND IS_WIND

#define SUIT_MAN (16)
#define SUIT_SOU (32)
#define SUIT_PIN (48)

/***
 * Tile IDs
 * X X X X X X X X
 * | \ | / '---\|/
 * |  \|/      tile value [1-9, WGR, ESWN]
 * |  suit indicator (bit 6 is honor flag) [man, sou, pin, dragon, wind]
 * aka flag
 */

char* generateAllTiles();
char* shuffleTiles(char* tiles);
char* _shuffleTiles(char* subsetHead, size_t nToShuffle);

char* tileDisplay(char t);
char tileValue(char* display);

void renderTile(char t);

#endif
