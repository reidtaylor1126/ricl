#include "main.h"
#include "tiles.h"
#include "hand.h"
#include "wall.h"

int main(int argc, const char* argv[]) {
    srand(time(NULL));
    // srand(5432);

    char* allTiles = generateAllTiles();

    printf("Generated tiles\n");
    
    struct wall* myWall = buildWall(allTiles);
    printWall(myWall);

    char newTile = draw(myWall);
    printf("Drew ");
    renderTile(newTile);
    printf("!\n");

    printWall(myWall);

    newTile = kan(myWall);
    printf("Drew ");
    renderTile(newTile);
    printf("!\n");

    printWall(myWall);

    cleanWall(myWall);
    free(myWall);

    free(allTiles);
    printf("Done!\n");
}
