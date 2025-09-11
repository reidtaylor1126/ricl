#include "player.h"

void createPlayerIn(struct player* dest, char* name) {
    strcpy(dest->name, name);
    dest->hand = (struct hand*) malloc(sizeof(struct hand));
    dest->nDiscards = 0;
    dest->score = 0;
}

struct player* createPlayer(char* name) {
    struct player* newPlayer = (struct player*) malloc(sizeof(struct player));
    createPlayerIn(newPlayer, name);
    return newPlayer;
}

struct discard* addDiscard(struct player* player, char tile) {
    player->discards[player->nDiscards].tile = tile;
    return(&player->discards[player->nDiscards++]);
}

void destroyPlayer(struct player* player) {
    destroyHand(player->hand);
    free(player);
}
