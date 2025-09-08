#ifndef TABLE_H
#define TABLE_H

#include "wall.h"
#include "player.h"
#include <string.h>

struct table {
    char* tileSet;
    struct wall* wall;
    struct player* players;
    uint8_t dealerSeat;
    uint8_t playerTurn;
    uint8_t riichiCount;
    uint8_t honbaCount;
    uint8_t prevailingWind;
};

struct table* createTable();
void deal(struct table* table, uint8_t advanceDealer);
void printTable(struct table* table);
void destroyTable(struct table*);

#endif