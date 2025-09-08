#include "table.h"

struct table* createTable() {
    struct table* newTable = (struct table*) malloc(sizeof(struct table));

    newTable->tileSet = generateAllTiles();
    newTable->wall = (struct wall*) malloc(sizeof(struct wall));

    struct player* players = (struct player*) malloc(sizeof(struct player)*4);
    for (int i = 0; i < 4; i++) {
        createPlayerIn(&players[i], "Player ");
        players[i].name[7] = '1'+i;
    }
    newTable->players = players;
    newTable->prevailingWind = EAST_WIND;

    return newTable;
}

void deal(struct table* table, uint8_t advanceDealer) {
    if (advanceDealer) {
        table->dealerSeat++;
        if (table->dealerSeat == 4) {
            table->dealerSeat = 0;
            table->prevailingWind++;
        }
        table->playerTurn = table->dealerSeat;
    } else {
        table->honbaCount++;
    }

    cleanWall(table->wall);
    buildWall(table->wall, table->tileSet);
}
