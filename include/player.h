#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include <string.h>

#define DISCARD_CAPACITY (24)

#define ACTION_DRAW (1)
#define ACTION_PON (2)
#define ACTION_CHI (3)
#define ACTION_KAN (4)
#define ACTION_RON (5)

#define ACTION_DISCARD (1)
#define ACTION_RIICHI (2)
#define ACTION_TSUMO (3)

#define NOT_TURN (0)
#define AWAITING_DRAW (1)
#define HAS_14TH_TILE (2)
#define CHOOSING_CHI (3)
#define CHOOSING_KAN (4)
#define DISCARDING (5)

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