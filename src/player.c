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

void renderDiscards(struct player* player, int start, int step, int nPerRow, int rowStep) {
    uint8_t rowCount = 0;
    int cursor = start;
    for (int i = 0; i < DISCARD_CAPACITY; i++) {
        struct discard* d = &player->discards[cursor];
        if (d->tile == 0)
            printf(".   ");
        else if (d->data & DISCARD_CALLED)
            renderCalledTile(d->tile);
        else
            renderTile(d->tile);
        rowCount++;
        if (rowCount >= nPerRow) {
            rowCount = 0;
            cursor += rowStep;
            printf(CURSOR_LEFT CURSOR_DOWN, 4*nPerRow, 1);
        } else {
            cursor += step;
        }
    }
}

void destroyPlayer(struct player* player) {
    destroyHand(player->hand);
    free(player);
}
