#ifndef HAND_H

#define HAND_H

#include "tiles.h"
#include "wall.h"

struct hand {
    struct tile* tiles;
    struct tile drawn;
};

struct hand drawHand(struct wall wall);

#endif