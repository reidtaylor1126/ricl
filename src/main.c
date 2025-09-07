#include "main.h"
#include "tiles.h"
#include "hand.h"

int main(int argc, const char* argv[]) {
    srand(time(NULL));
    // srand(5432);

    char* allTiles = generateAllTiles();
    shuffle(allTiles, 136);

    for (int i = 0; i < 136; i++) {
        renderTile(allTiles[i]);
    }
    printf("Done!\n");
    free(allTiles);
}
