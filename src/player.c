#include "player.h"
#include "cursor.h"

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
    printf("Discard value: %x\n", tile);
    player->discards[player->nDiscards].tile = tile;
    return(&player->discards[player->nDiscards++]);
}

void renderCalledTile(char t) {
    char* display = tileDisplay(t);
    printf(ANSI_COLOR_GRAY "[%s]" ANSI_COLOR_RESET, display);
    free(display);
}

void newDiscardRow(uint8_t hOffset) {
    // printf(CURSOR_DOWN);
    printf("\n"CURSOR_RIGHT, hOffset);
    fflush(stdout);
}

void renderDiscards(struct player* player) {
    for (int i = 0; i < player->nDiscards; i++) {
        if (i % 6 == 0 && i > 0)
            newDiscardRow(24);
        if (player->discards[i].data & DISCARD_CALLED)
            renderCalledTile(player->discards[i].tile);
        else
            renderTile(player->discards[i].tile);
    }
    printf("\n");
}

void destroyPlayer(struct player* player) {
    destroyHand(player->hand);
    free(player);
}
