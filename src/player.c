#include "player.h"

void createPlayerIn(struct player* dest, char* name) {
    strcpy(dest->name, name);
    dest->hand = (struct hand*) malloc(sizeof(struct hand));
    dest->discards = (struct discard*) malloc(sizeof(struct discard)*DISCARD_CAPACITY);
    dest->nDiscards = 0;
    dest->score = 0;
}

struct player* createPlayer(char* name) {
    struct player* newPlayer = (struct player*) malloc(sizeof(struct player));
    createPlayerIn(newPlayer, name);
    return newPlayer;
}
