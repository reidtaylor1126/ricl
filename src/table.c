#include "table.h"
#include "cursor.h"

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

int handleAwaitDraw(struct table* table, struct player* currentPlayer) {
    char inputBuffer[4];
    printf("(1) Draw | (2): Pon | (3): Chi | (4): Kan | (5): Ron | (q): Quit | > ");
    fflush(stdout);
    scanf("%s", inputBuffer);

    if (inputBuffer[0] == 'q') {
        printf("Quitting!\n");
        table->gameRunning = 0;
        return 0;
    }

    int inputNumber = atoi(inputBuffer);
    switch (inputNumber & 255) {
        case ACTION_DRAW:
            char drawn = draw(table->wall);
            currentPlayer->hand->drawn = drawn;
            printf("\033[2A\r");
            renderHand(currentPlayer->hand);
            printf("\n");
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
                fflush(stdout);
                scanf("%s", inputBuffer);
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
            printf("Invalid input!\n");
            break;
    }
    return 1;
}

void handleDiscard(struct table* table, struct player* currentPlayer) {
    char inputBuffer[4];
    moveCursor(CURSOR_UP, 1);
    printf("\rDiscard which tile? > ");
    eraseNextN(36);
    scanf("%s", inputBuffer);
    
    int inputNumber = atoi(inputBuffer);

    char tile;
    if (inputNumber > 0 && (inputNumber & 255) <= currentPlayer->hand->nClosed) {
        printf("Discarding tile %hu from hand! ", (inputNumber & 255)-1);
        tile = removeFromHand(currentPlayer->hand, (inputNumber & 255)-1);
    } else if (currentPlayer->hand->drawn != 0 && inputNumber == 0) {
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

int handleAwaitAction(struct table* table, struct player* currentPlayer) {
    char inputBuffer[4];
    printf("(1) Discard | (2): Riichi | (3): Tsumo | (4): Kan | >                 \033[16D");
    fflush(stdout);

    scanf("%s", inputBuffer);
    int inputNumber = atoi(inputBuffer);

    switch (inputNumber) {
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
            printf("\nInvalid input!\n");
            break;
    }
    return 1;
}

int tickTurn(struct table* table) {
    struct player* currentPlayer = table->players[table->playerTurn];
    // renderHand(currentPlayer->hand, 0);
    if (currentPlayer->turnStage == AWAITING_DRAW) {
        if (handleAwaitDraw(table, currentPlayer) == 0)
            return 0;
    }
    if (currentPlayer->turnStage == HAS_14TH_TILE) {
        if (handleAwaitAction(table, currentPlayer) == 0)
            return 0;
    }
    if (currentPlayer->turnStage == DISCARDING) {
        // TODO
    }
    return 1;
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
            moveCursor(CURSOR_UP, 1);
        renderDiscards(table->players[i]);
        moveCursor(CURSOR_RIGHT, 8);
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
