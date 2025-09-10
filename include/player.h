#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include <string.h>

#define DISCARD_CAPACITY (32)

#define ACTION_DRAW (1)
#define ACTION_DISCARD (1)
#define ACTION_PON (2)
#define ACTION_MAHJONG (2)
#define ACTION_CHI (3)
#define ACTION_RIICHI (3)
#define ACTION_KAN (4)

#define NOT_TURN (0)
#define AWAITING_DRAW (1)
#define HAS_14TH_TILE (2)
#define CHOOSING_CHI (3)
#define CHOOSING_KAN (4)
#define DISCARDING (5)

/** 
 * Turn stages
 * 0 not player's turn
 * 1 waiting; can draw, pon, chi, kan, or declare mahjong (if conditions met)
 * 2 has extra, choosing action
 * 3 choosing set for chi
 * 4 choosing set for kan (if not called)
 * 5 discarding (or riichiing)
*/

struct discard {
    char tile;
    uint8_t called;
};

struct player {
    struct hand* hand;
    struct discard discards[DISCARD_CAPACITY];
    uint8_t nDiscards;
    char name[16];
    int score;
    uint8_t turnStage;
};

void createPlayerIn(struct player* dest, char* name);
struct player* createPlayer(char* name);
void destroyPlayer(struct player* player);

#endif