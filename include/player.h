#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include <string.h>

#define DISCARD_CAPACITY (32)

struct discard {
    char tile;
    uint8_t called;
};

struct player {
    struct hand* hand;
    struct discard* discards;
    uint8_t nDiscards;
    char name[16];
    int score;
};

void createPlayerIn(struct player* dest, char* name);
struct player* createPlayer(char* name);
void deletePlayer(struct player*);

#endif