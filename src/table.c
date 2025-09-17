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
    printf("(1) Draw | (2): Pon | (3): Chi | (4): Kan | (5): Ron | > ");
    scanf("%d", &inputBuffer);
    switch (inputBuffer) {
        case ACTION_DRAW:
            char drawn = draw(table->wall);
            currentPlayer->hand->drawn = drawn;
            currentPlayer->turnStage = HAS_14TH_TILE;
            break;
        case ACTION_PON:
            if (countInHand(currentPlayer->hand, table->lastDiscard->tile) > 1) {
                table->lastDiscard->data = DISCARD_CALLED;
                pon(currentPlayer->hand, table->lastDiscard->tile);
                currentPlayer->turnStage = DISCARDING;
            } else {
                printf("\a");
            }
            break;
        case ACTION_CHI:
            char options = chiOptions(currentPlayer->hand, table->lastDiscard->tile);
            if (options == 0) {
                printf("\a");
            } else {
                printf("Enter bottom of chi: ");
                scanf("%d", &inputBuffer);
            }
        case ACTION_KAN:
            if (countInHand(currentPlayer->hand, table->lastDiscard->tile) > 2) {
                table->lastDiscard->data = DISCARD_CALLED;
                closedKan(currentPlayer->hand, table->lastDiscard->tile);
            } else {
                printf("\a");
            }
        case ACTION_RON:
            // TODO
            break;
        default:
            printf("\a");
            break;
    }
}

void handleDiscard(struct table* table, struct player* currentPlayer) {
    int inputBuffer = 0;
    printf("Discard which tile? > ");
    fflush(NULL);
    scanf("%d", &inputBuffer);
    printf("Got %d ", inputBuffer);
    char tile;
    if (inputBuffer > 0 && (inputBuffer & 255) < currentPlayer->hand->nClosed) {
        printf("Discarding tile %hu from hand! ", (inputBuffer & 255)-1);
        tile = removeFromHand(currentPlayer->hand, (inputBuffer&255)-1);
    } else if (currentPlayer->hand->drawn != 0 && inputBuffer == 0) {
        printf("Discarding drawn tile! ");
        tile = currentPlayer->hand->drawn;
        currentPlayer->hand->drawn = 0;
    } else {
        printf("\a");
        return;
    }
    printf("Discarding ");
    renderTile(tile);
    struct discard* newDiscard = addDiscard(currentPlayer, tile);
    table->lastDiscard = newDiscard;
    currentPlayer->turnStage = NOT_TURN;
}

void handleRiichi(struct table* table, struct player* currentPlayer) {
    int inputBuffer = 0;
    scanf("%d", &inputBuffer);
    struct hand* testHand = cloneHandWithout(currentPlayer->hand, inputBuffer);
    
    free(testHand);
}

void handleAwaitAction(struct table* table, struct player* currentPlayer) {
    int inputBuffer = 0;
    printf("(1) Discard | (2): Riichi | (3): Tsumo | (4): Kan | > ");
    scanf("%d", &inputBuffer);
    fflush(NULL);
    switch (inputBuffer) {
        case ACTION_DISCARD:
            handleDiscard(table, currentPlayer);
            break;
        case ACTION_RIICHI:
            handleRiichi(table, currentPlayer);
            break;
        case ACTION_TSUMO:
            // TODO
            break;
        case ACTION_KAN:
            // TODO
            break;
        default:
            printf("\a");
    }
}

void tickTurn(struct table* table) {
    struct player* currentPlayer = table->players[table->playerTurn];
    // renderHand(currentPlayer->hand, 0);
    if (currentPlayer->turnStage == AWAITING_DRAW) {
        handleAwaitDraw(table, currentPlayer);
    }
    if (currentPlayer->turnStage == HAS_14TH_TILE) {
        handleAwaitAction(table, currentPlayer);
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
        table->players[i]->didRiichi = 0;
    }
    char firstDraw = draw(table->wall);
    table->players[table->dealerSeat]->hand->drawn = firstDraw;
    table->players[table->dealerSeat]->turnStage = HAS_14TH_TILE;
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
