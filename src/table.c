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

void eraseLast(uint8_t nChars) {
    printf("\r\033[1A");
    for (int i = 0; i < nChars; i++) {
        printf(" ");
    }
    printf("\r");
}

void handleAwaitDraw(struct table* table, struct player* currentPlayer) {
    int inputBuffer = 0;
    scanf("%d", &inputBuffer);
    if (inputBuffer == ACTION_DRAW) {
        char drawn = draw(table->wall);
        currentPlayer->hand->drawn = drawn;
        currentPlayer->turnStage = HAS_14TH_TILE;
    } else if (inputBuffer == ACTION_PON) {
        uint8_t count = countInHand(currentPlayer->hand, table->lastDiscard->tile);
        if (count > 1) {
            table->lastDiscard->called = 1;
            pon(currentPlayer->hand, table->lastDiscard->tile);
            currentPlayer->turnStage = DISCARDING;
        } else {
            printf("\a");
        }
    } else if (inputBuffer == ACTION_CHI) {
        char options = chiOptions(currentPlayer->hand, table->lastDiscard->tile);
        if (options == 0) {
            printf("\a");
        } else {
            printf("Enter bottom of chi: ");
            scanf("%d", &inputBuffer);
        }
    } else if (inputBuffer == ACTION_KAN) {
        uint8_t count = countInHand(currentPlayer->hand, table->lastDiscard->tile);
        if (count > 1) {
            table->lastDiscard->called = 1;
            kan(currentPlayer->hand, table->lastDiscard->tile);
            currentPlayer->turnStage = DISCARDING;
        } else {
            printf("\a");
        }
    }
}

void tickTurn(struct table* table) {
    struct player* currentPlayer = table->players[table->playerTurn];
    renderHand(currentPlayer->hand);
    if (currentPlayer->turnStage == AWAITING_DRAW) {
        handleAwaitDraw(table, currentPlayer);
    }
    if (currentPlayer->turnStage == HAS_14TH_TILE) {
        // TODO
    }
    if (currentPlayer->turnStage == DISCARDING) {
        // TODO
    }
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
    table->players[table->dealerSeat]->turnStage = HAS_14TH_TILE;
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
