#ifndef WALL_H

#define WALL_H

#include "tiles.h"
#include <stdlib.h>

struct wallTile {
    char tile;
    struct wallTile* next;
};

struct wall {
    struct wallTile* live;
    uint8_t numLive;
    char* dead;
    uint8_t numDead;
    uint8_t numDora;
};

struct wall* buildWall(char* tileSet);
char draw(struct wall* wall);
char kan(struct wall* wall);
void printWall(struct wall* wall);
void cleanWall(struct wall* wall);

#endif