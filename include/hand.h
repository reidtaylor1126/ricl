#ifndef HAND_H

#define HAND_H

#include "tiles.h"
#include "wall.h"

struct hand {
    char* tiles;
    char drawn;
};

struct hand drawHand(struct wall wall);

#endif