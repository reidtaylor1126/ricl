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

void setTurn(struct table* table, uint8_t playerIndex) {
    table->playerTurn = playerIndex;
    printf("\nIt is now %s's turn!\n", table->players[playerIndex]->name);
}

void advanceTurn(struct table* table) {
    uint8_t nextPlayer = (table->playerTurn + 1) % 4;
    setTurn(table, nextPlayer);
    table->players[nextPlayer]->turnStage = AWAITING_DRAW;
}

void printTable(struct table* table) {
    printWall(table->wall);
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
        renderDiscards(table->players[i], 0, 1, 6, 0);
        printf(CURSOR_RIGHT, 8);
        renderHand(table->players[i]->hand);
    printf("\n");
    }
}

void renderTable(struct table* table, uint8_t playerPerspective) {
    clearScreen();
    
    moveCursorTo(HAND_POSITION);
    renderHand(table->players[table->playerTurn]->hand);
    moveCursorTo(SELF_DISCARD_POSITION);
    renderDiscards(table->players[table->playerTurn], 0, 1, 6, 1);

    uint8_t nextPlayer = (table->playerTurn+1)%4;
    renderRightHand(table->players[nextPlayer]->hand, 0);
    moveCursorTo(RIGHT_DISCARD_POSITION);
    renderDiscards(table->players[nextPlayer], 5, 6, 4, -19);

    nextPlayer = (table->playerTurn+2)%4;
    renderOppositeHand(table->players[nextPlayer]->hand, 0);
    moveCursorTo(OPPOSITE_DISCARD_POSITION);
    renderDiscards(table->players[nextPlayer], 23, -1, 6, -1);

    nextPlayer = (table->playerTurn+3)%4;
    renderLeftHand(table->players[nextPlayer]->hand, 0);
    moveCursorTo(LEFT_DISCARD_POSITION);
    renderDiscards(table->players[nextPlayer], 18, -6, 4, 19);

    moveCursorTo(32, 11);
    printf("%s" CURSOR_LEFT CURSOR_UP, table->prevailingWind == 1 ? "South" : "East ", 5, 1);
    printWall(table->wall);

    moveCursorTo(0, 24);
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

uint8_t playerCanPon(struct table* table) {
    uint8_t playerHasOne = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t tileCount = countInHand(table->players[i]->hand, table->lastDiscard->tile);
        if (tileCount == 1) {
            if (playerHasOne == 1)
                return 255;
            playerHasOne++;
        } else if (tileCount == 2) {
            return i;
        } else if (tileCount == 3) {
            return i + 4;
        }
    }
    return 255;
}

uint8_t waitForNextReady(struct table* table) {
    clearScreen();
    
    uint8_t nextPlayer = (table->playerTurn+1) % 4;
    char* nextName = table->players[nextPlayer]->name;
    moveCursorTo(32, 8);
    printf("%s plays next.\n", nextName);
    
    moveCursorTo(24, 10);
    printf("Last player discarded: ");
    renderTile(table->lastDiscard->tile);
    printf("\n");
    
    uint8_t canPon = playerCanPon(table);
    // if a player can pon and they are not the previous player
    if (canPon != 255 && (canPon & 3) != table->playerTurn) {
        moveCursorTo(24, 12);
        printf("!! A player can call the discarded tile !!");
        
        moveCursorTo(24, 14);
        uint8_t input = inputInt("(1) Call | (0) Ignore | > ", 24);

        if (input == 0) {
            advanceTurn(table);
            return 1;
        } else if (input == 1) {
            handleSkippingPon(table, canPon);
            return 1;
        } else {
            return 0;
        }
    } 

    moveCursorTo(24, 14);
    inputInt("Ready? > ", 0);
    advanceTurn(table);
    return 1;
}
