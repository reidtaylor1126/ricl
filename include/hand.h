#ifndef HAND_H
#define HAND_H

#include "tiles.h"

/**
 * Meld structure
 * headTile: The lowest tile in a sequence, or the one tile in a triplet/kan
 * data: X X X X X X X X
 *       |           \ /
 *       |            |
 *       |            meldType [sequence, triplet, kan]
 *       hasAka
 */

struct handTile {
    struct handTile* next;
    char value;
};

struct meld {
    struct meld* next;
    char headTile;
    char data;
};

struct hand {
    struct handTile* tilesHead;
    struct meld* meldsHead;
    uint8_t nClosed;    // number of closed tiles
    uint8_t nOpen;      // number of open melds, not tiles
    char drawn;
};

struct hand* createHand();
int addTileToHand(struct hand* hand, char tile);
void renderHand(struct hand* hand);
void destroyHand(struct hand* hand);

#endif