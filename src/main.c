#include "main.h"
#include "tiles.h"
#include "hand.h"

int main(int argc, const char* argv[]) {
    struct tile* allTiles = generateAllTiles();
    for (int i = 0; i < 136; i++) {
        renderTile(allTiles[i]);
    }
    printf("Done!\n");
    free(allTiles);
}
