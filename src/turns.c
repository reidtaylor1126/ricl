#include "turns.h"
#include "scoring.h"

int handleAwaitDraw(struct table* table, struct player* currentPlayer) {
    char inputBuffer[4];
    if (currentPlayer->didRiichi)
        printf("(1) Draw | (5): Ron | (q): Quit | > ");
    else
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
    printf(CURSOR_UP"\rDiscard which tile? > ", 1);
    eraseNextN(36);
    scanf("%s", inputBuffer);
    
    int inputNumber = atoi(inputBuffer);

    char tile;
    if (inputNumber > 0 && (inputNumber & 255) <= currentPlayer->hand->nClosed) {
        tile = removeFromHand(currentPlayer->hand, (inputNumber & 255)-1, 1);
        addDrawnToHand(currentPlayer->hand);
    } else if (currentPlayer->hand->drawn != 0 && inputNumber == 0) {
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

void handleChi(struct table* table, struct player* currentPlayer) {
    if (table->lastDiscard->tile & IS_HONOR) {
        printf("\a");
        return;
    }

    char headsBuffer[3];
    uint8_t sequenceImprint = findSequencesFor(currentPlayer->hand, table->lastDiscard->tile, headsBuffer);

}

void handleRiichi(struct table* table, struct player* currentPlayer) {
    int inputBuffer = 0;
    scanf("%d", &inputBuffer);
    struct hand* testHand = cloneHandWithout(currentPlayer->hand, inputBuffer);
    
    free(testHand);
}

void handleClosedKan(struct table* table, struct player* currentPlayer) {
    char inputBuffer[4];
    printf(CURSOR_UP"\rKan which tile? > ", 1);
    eraseNextN(36);
    scanf("%s", inputBuffer);
    
    uint8_t inputNumber = atoi(inputBuffer) & 255;

    if (inputNumber > currentPlayer->hand->nClosed) {
        printf("\a");
        return;
    }

    struct handTile* targetHandTile = getHandTileAt(currentPlayer->hand, inputNumber);
    char tileValue = targetHandTile->value & (~IS_AKA);
    uint8_t tileData = targetHandTile->data;
    uint8_t tileCount = tileData & HANDTILE_COUNT_MASK;
    if (tileCount == 4 || (tileCount == 3 && tileValue == currentPlayer->hand->drawn & (!IS_AKA))) {
        removeFromHand(currentPlayer->hand, inputNumber, tileCount);
        addMeld(currentPlayer->hand, tileValue, MELD_KAN + MELD_CLOSED);
        currentPlayer->hand->meldsHead->data += tileData & IS_AKA;
        char drawn = kanDraw(table->wall);
        currentPlayer->hand->drawn = drawn;
        currentPlayer->turnStage = HAS_14TH_TILE;
        printf("\033[2A\r");
        renderHand(currentPlayer->hand);
    }
}

int handleAwaitAction(struct table* table, struct player* currentPlayer) {
    char inputBuffer[4];
    if (currentPlayer->didRiichi) {
        printf("(1) Discard | (3): Tsumo | (4): Kan | > ");
    } else {
        printf("(1) Discard | (2): Riichi | (3): Tsumo | (4): Kan | > ");
    }
    eraseNextN(30);

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
            handleClosedKan(table, currentPlayer);
            break;
        default:
            printf("\a\nInvalid input!\n");
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