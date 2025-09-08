#include "table.h"

struct table* createTable() {
    struct table* newTable = (struct table*) malloc(sizeof(struct table));

    newTable->tileSet = generateAllTiles();
    newTable->wall = (struct wall*) malloc(sizeof(struct wall));

    for (int i = 0; i < 4; i++) {
        struct player* newPlayer = malloc(sizeof(struct player));
        createPlayerIn(newPlayer, "Player ");
        newPlayer->name[7] = '1'+i;
        newTable->players[i] = newPlayer;
    }
    newTable->prevailingWind = EAST_WIND;
    newTable->dealerSeat = 255;
    newTable->honbaCount = 0;

    return newTable;
}

void deal(struct table* table, uint8_t advanceDealer) {
    if (advanceDealer == 1) {
        table->dealerSeat++;
        if (table->dealerSeat == 4) {
            table->dealerSeat = 0;
            table->prevailingWind++;
        }
        printf("%s is dealer this hand\n", table->players[table->dealerSeat]->name);
        table->playerTurn = table->dealerSeat;
    } else {
        printf("Same dealer as last round\n");
        table->honbaCount++;
        printf("Table now has %hu honba\n", table->honbaCount);
    }

    cleanWall(table->wall);
    buildWall(table->wall, table->tileSet);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            char tile = draw(table->wall);
            addTileToHand(table->players[i]->hand, tile);
        }
    }
    char firstDraw = draw(table->wall);
    addTileToHand(table->players[table->dealerSeat]->hand, firstDraw);
}

void printTable(struct table* table) {
    printWall(table->wall);
    printf(" | %s\n", table->prevailingWind == 1 ? "South" : "East");
    for (int i = 0; i < 60; i++)
        printf("=");
    printf("\n");
    for (int i = 0; i < 4; i++) {
        if (i == table->dealerSeat)
            printf(ANSI_COLOR_YELLOW);
        printf("%s: ", table->players[i]->name);
        printf(ANSI_COLOR_RESET);
        renderHand(table->players[i]->hand);
    printf("\n");
    }
}

void destroyTable(struct table* table) {
    for (int i = 0; i < 4; i++) {
        destroyPlayer(table->players[i]);
    }
    cleanWall(table->wall);
    free(table->wall);
    free(table->tileSet);
    free(table);
}
