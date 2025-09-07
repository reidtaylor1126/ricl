#include "wall.h"

char* shuffle(char* subsetHead, size_t nToShuffle) {
    if (nToShuffle == 1) {
        return subsetHead;
    }
    size_t nLeft = nToShuffle/2;
    size_t nRight = nToShuffle - nLeft;

    shuffle(subsetHead, nLeft);
    shuffle(subsetHead+nLeft, nRight);

    char* merged = malloc(nToShuffle);

    size_t remainingLeft = nLeft;
    size_t remainingRight = nRight;

    while(remainingLeft && remainingRight) {
        size_t take = rand() % (remainingLeft+remainingRight);
        if (take < remainingLeft) {
            merged[remainingLeft+remainingRight-1] = subsetHead[remainingLeft-1];
            remainingLeft--;
        } else {
            merged[remainingLeft+remainingRight-1] = subsetHead[nLeft+remainingRight-1];
            remainingRight--;
        }
    }

    while (remainingLeft) {
        merged[remainingLeft-1] = subsetHead[remainingLeft-1];
        remainingLeft--;
    }
    
    while (remainingRight) {
        merged[remainingLeft+remainingRight-1] = subsetHead[nLeft+remainingRight-1];
        remainingRight--;
    }

    for (size_t i = 0; i < nToShuffle; i++) {
        subsetHead[i] = merged[i];
    }
    free(merged);
    return subsetHead;
}

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
