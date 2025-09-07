#include "hand.h"
#include "wall.h"


struct hand drawHand(struct wall* wall) {
    char* handTiles = (char*) malloc(13);

    for (int i = 0; i < 13; i++) {
        handTiles[i] = draw(wall);
    }

    struct hand newHand = {handTiles, 0};
    return newHand;
}
