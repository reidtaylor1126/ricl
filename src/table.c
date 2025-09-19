#include "table.h"
#include "cursor.h"
#include "turns.h"

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

void eraseLast(uint8_t nChars) {
    printf("\r\033[1A");
    for (int i = 0; i < nChars; i++) {
        printf(" ");
    }
    printf("\r");
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
        table->players[i]->didRiichi = 0;
    }
    char firstDraw = draw(table->wall);
    table->players[table->dealerSeat]->hand->drawn = firstDraw;
    table->players[table->dealerSeat]->turnStage = HAS_14TH_TILE;

    table->gameRunning = 1;
}

void advanceTurn(struct table* table) {
    uint8_t nextPlayer = (table->playerTurn + 1) % 4;
    table->playerTurn = nextPlayer;
    printf("\nIt is now %s's turn!\n", table->players[nextPlayer]->name);
    table->players[nextPlayer]->turnStage = AWAITING_DRAW;
}

void printTable(struct table* table) {
    printfall(table->wall);
    printf(" | %s\n", table->prevailingWind == 1 ? "South" : "East");
    for (int i = 0; i < 60; i++)
        printf("=");
    printf("\n");
    for (int i = 0; i < 4; i++) {
        if (i == table->playerTurn)
            printf("> ");
        else
            printf("  ");
        if (i == table->dealerSeat)
            printf(ANSI_COLOR_YELLOW);
        printf("%s: ", table->players[i]->name);
        printf(ANSI_COLOR_RESET);
        if (table->players[i]->nDiscards > 0)
            printf(CURSOR_UP, 1);
        renderDiscards(table->players[i]);
        printf(CURSOR_RIGHT, 8);
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
