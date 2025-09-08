#include "wall.h"

void buildWall(struct wall* wall, char* tileSet) {
    shuffleTiles(tileSet);

    wall->numLive = 122;
    for (int i = 121; i >=0; i--) {
        struct wallTile* newTile = (struct wallTile*) malloc(sizeof(struct wallTile));
        newTile->tile = tileSet[i];
        newTile->next = wall->live;
        wall->live = newTile;
    } 

    wall->numDead = 14;
    wall->dead = &(tileSet[122]);
    wall->numDora = 1;
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
}

void cleanWall(struct wall* wall) {
    while (wall->numLive > 0) {
        struct wallTile* oldHead = wall->live;
        wall->live = oldHead->next;
        free(oldHead);
        wall->numLive--;
    }
}
