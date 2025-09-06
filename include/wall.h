#ifndef WALL_H

#define WALL_H

#include "tiles.h"

struct wallTile {
    struct tile tile;
    struct wallTile* next;
};

struct wall {
    struct wallTile* live;
    uint8_t numLive;
    struct tile* dead;
    uint8_t numDead;
    uint8_t numDora;
};

struct tile draw(struct wall wall);
struct tile kan(struct wall wall);

#endif