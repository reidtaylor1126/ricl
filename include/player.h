#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include <string.h>

#define DISCARD_CAPACITY (24)

/** 
 * Turn stages
 * 0 not player's turn
 * 1 waiting; can draw, pon, chi, kan, or ron (if conditions met)
 * 2 has extra, choosing action
 * 3 choosing set for chi
 * 4 choosing set for kan (if not called)
 * 5 discarding (or riichiing)
*/

#define DISCARD_CALLED (1)
#define DISCARD_RIICHI (2)

struct discard {
    char tile;
    uint8_t data;
};

struct player {
    struct hand* hand;
    struct discard discards[DISCARD_CAPACITY];
    uint8_t nDiscards;
    char name[16];
    int score;
    uint8_t turnStage;
    uint8_t didRiichi;
};

void createPlayerIn(struct player* dest, char* name);
struct player* createPlayer(char* name);
struct discard* addDiscard(struct player* player, char tile);
void renderDiscards(struct player* player);
void destroyPlayer(struct player* player);

#endif