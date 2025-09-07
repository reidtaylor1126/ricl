#include "wall.h"

struct wall* buildWall(char* tileSet) {
    shuffleTiles(tileSet);

    struct wall *newWall;
    newWall = malloc(sizeof *newWall);

    newWall->numLive = 122;
    for (int i = 121; i >=0; i--) {
        struct wallTile* newTile = (struct wallTile*) malloc(sizeof(struct wallTile));
        newTile->tile = tileSet[i];
        newTile->next = newWall->live;
        newWall->live = newTile;
    } 

    newWall->numDead = 14;
    newWall->dead = &(tileSet[122]);
    newWall->numDora = 1;

    return newWall;
}

char draw(struct wall* wall) {
    if (wall->numLive == 0) {
        return 0;
    };
    struct wallTile* next = wall->live->next;
    char drawn = wall->live->tile;
    free(wall->live);
    wall->live = next;
    wall->numLive--;

    return drawn;
}

char kan(struct wall* wall) {
    if (wall->numDead <= 10) {
        return (char) {0};
    }
    char kanDraw = wall->dead[14-wall->numDead];

    wall->numDora++;
    wall->numLive--;
    wall->numDead--;

    return kanDraw;
}

void printWall(struct wall* wall) {
    printf("[%u]] | ", wall->numLive);
    for (int i = 0; i < wall->numDora; i++) {
        renderTile(wall->dead[4+(2*i)]);
    }
    printf("\n");
}

void cleanWall(struct wall* wall) {
    while (wall->numLive > 0) {
        struct wallTile* oldHead = wall->live;
        wall->live = oldHead->next;
        free(oldHead);
        wall->numLive--;
    }
}
