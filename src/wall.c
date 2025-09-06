#include "wall.h"

struct tile draw(struct wall wall) {
    if (wall.numLive == 0) {
        return (struct tile) {0};
    };
    struct wallTile* next = wall.live->next;
    struct tile drawn = wall.live->tile;
    free(wall.live);
    wall.live = next;
    wall.numLive--;

    return drawn;
}

struct tile kan(struct wall wall) {
    if (wall.numDead <= 10) {
        return (struct tile) {0};
    }
    struct tile kanDraw = wall.dead[14-wall.numDead];

    wall.numDora++;
    wall.numLive--;
    wall.numDead--;

    return kanDraw;
}
