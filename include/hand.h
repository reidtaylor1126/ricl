#ifndef HAND_H
#define HAND_H

#include "tiles.h"

/**
 * handTile structure
 * data: X X X X X X X X
 *       |         \ | /
 *       |          \|/
 *       hasAka    count
 */

 #define HANDTILE_COUNT_MASK (7)

struct handTile {
    struct handTile* next;
    char value;
    uint8_t data;
};

/**
 * meld structure
 * headTile: The lowest tile in a sequence, or the one tile in a triplet/kan
 * data: X X X X X X X X
 *       | |         \ /
 *       | |          |
 *       | closed     meldType [sequence, triplet, kan]
 *       hasAka
 */

#define MELD_SEQUENCE (1)
#define MELD_TRIPLET (2)
#define MELD_KAN (3)
#define MELD_TYPE_MASK (3)

#define MELD_CLOSED (64)

struct meld {
    struct meld* next;
    char headTile;
    char data;
};

struct hand {
    struct handTile* tilesHead;
    struct meld* meldsHead;
    uint8_t nClosed;    // number of closed tiles
    uint8_t nUniqueClosed;
    uint8_t nMelds;      // number of open melds, not tiles
    char drawn;
};

struct hand* createHand();
int addTileToHand(struct hand* hand, unsigned char tile);
int addDrawnToHand(struct hand* hand);
char removeFromHand(struct hand* hand, uint8_t index, uint8_t count);
char removeTileFromHand(struct hand* hand, char tile, uint8_t count);
uint8_t countInHand(struct hand* hand, unsigned char tile);
uint8_t countOpenMelds(struct hand* hand);
char pon(struct hand* hand, unsigned char tile);
char chi(struct hand* hand, unsigned char lowTile);
char chiOptions(struct hand* hand, unsigned char tile);
char closedKan(struct hand* hand, unsigned char tile);
char openKan(struct hand* hand, unsigned char tile);
void addMeld(struct hand* hand, char headTile, char data);
void renderMeld(struct meld* meld);
void renderHand(struct hand* hand);
void destroyHand(struct hand* hand);
struct handTile* getHandTileAt(struct hand* hand, uint8_t index);
struct handTile* closedAsArray(struct hand* hand);
struct hand* cloneHandWithout(struct hand* hand, uint8_t index);

#endif