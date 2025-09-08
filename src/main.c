#include "main.h"
#include "tiles.h"
#include "hand.h"
#include "wall.h"

int main(int argc, const char* argv[]) {
    srand(time(NULL));
    // srand(5432);

    char* allTiles = generateAllTiles();
    shuffleTiles(allTiles);
    
    struct hand* myHand = createHand();

    for (int i = 0; i < 13; i++) {
        addTileToHand(myHand, allTiles[i]);
    }
    renderHand(myHand);

    destroyHand(myHand);
    free(myHand);
    free(allTiles);
}
