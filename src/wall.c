#include "wall.h"

char draw(struct wall wall) {
    if (wall.numLive == 0) {
        return 0;
    };
    struct wallTile* next = wall.live->next;
    char drawn = wall.live->tile;
    free(wall.live);
    wall.live = next;
    wall.numLive--;

    return drawn;
}

char kan(struct wall wall) {
    if (wall.numDead <= 10) {
        return (char) {0};
    }
    char kanDraw = wall.dead[14-wall.numDead];

    wall.numDora++;
    wall.numLive--;
    wall.numDead--;

    return kanDraw;
}
