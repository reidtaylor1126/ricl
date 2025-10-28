#include "turns.h"
#include "scoring.h"

void handleDiscard(struct table* table, struct player* currentPlayer) {
    // char inputBuffer[4];
    // printf(CURSOR_UP"\rDiscard which tile? > ", 1);
    // eraseNextN(48);
    // scanf("%s", inputBuffer);
    
    // int inputNumber = atoi(inputBuffer);

    char* prompt = malloc(32);
    sprintf(prompt, CURSOR_UP"\rDiscard which tile? > ", 1);
    int inputNumber = inputInt(prompt, 48);
    free(prompt);

    char tile;
    if (inputNumber > 0 && (inputNumber & 255) <= currentPlayer->hand->nClosed) {
        tile = removeFromHand(currentPlayer->hand, (inputNumber & 255)-1, 1);
        if (currentPlayer->hand->drawn != 0)
            addDrawnToHand(currentPlayer->hand);
    } else if (currentPlayer->hand->drawn != 0 && inputNumber == 0) {
        tile = currentPlayer->hand->drawn;
        currentPlayer->hand->drawn = 0;
    } else {
        printf("\a");
        return;
    }
    struct discard* newDiscard = addDiscard(currentPlayer, tile);
    table->lastDiscard = newDiscard;
    printf("Discarding ");
    renderTile(tile);
    currentPlayer->turnStage = NOT_TURN;
}

void handlePon(struct table* table, struct player* currentPlayer) {
    char targetTile = table->lastDiscard->tile;
    char tilesFromHand = removeTileFromHand(currentPlayer->hand, targetTile & ~IS_AKA, 2, 1);
    if (tilesFromHand != 0) {
        table->lastDiscard->data |= DISCARD_CALLED;
        addMeld(currentPlayer->hand, targetTile & ~IS_AKA, MELD_TRIPLET + ((targetTile | tilesFromHand) & IS_AKA));
        // rerenderHand(currentPlayer->hand);
        renderTable(table, 0);
        // printf("\n");
        currentPlayer->turnStage = DISCARDING;
    } else {
        printf("Insufficient ");
        renderTile(targetTile & ~IS_AKA);
        printf(" in hand!");
    }
}

void handleChi(struct table* table, struct player* currentPlayer) {
    if (table->lastDiscard->tile & IS_HONOR) {
        printf("Cannot chi ");
        renderTile(table->lastDiscard->tile);
        printf("\n");
        return;
    }

    currentPlayer->hand->drawn = table->lastDiscard->tile;

    uint8_t sequenceImprint = findSequencesForTile(currentPlayer->hand, table->lastDiscard->tile);

    if (! (sequenceImprint & HAS_SEQUENCE)) {
        printf("No sequences in hand for tile!\n");
        return;
    }

    currentPlayer->turnStage = CHOOSING_CHI;
}

uint8_t mapChiRemoves(int* toRemove, int headDistance, uint8_t sequenceImprint) {
    switch (headDistance) {
        case 0:
            if ((sequenceImprint & SEQUENCE_ABOVE_MASK) == SEQUENCE_ABOVE_MASK) {
                toRemove[0] = 1;
                toRemove[1] = 2;
            } else {
                printf("Invalid chi start!\n");
                return 0;
            }
            break;
        case 1:
            if ((sequenceImprint & SEQUENCE_CENTER_MASK) == SEQUENCE_CENTER_MASK) {
                toRemove[0] = -1;
                toRemove[1] = 1;
            } else {
                printf("Invalid chi start!\n");
                return 0;
            }
            break;
        case 2:
            if ((sequenceImprint & SEQUENCE_BELOW_MASK) == SEQUENCE_BELOW_MASK) {
                toRemove[0] = -2;
                toRemove[1] = -1;
            } else {
                printf("Invalid chi start!\n");
                return 0;
            }
            break;
        default:
            printf("Invalid index!\n");
            return 0;
    }
    return 1;
}

void chooseChi(struct table* table, struct player* currentPlayer) {
    uint8_t sequenceImprint = findSequencesForTile(currentPlayer->hand, table->lastDiscard->tile);
    printf("Sequence imprint: %hu\n", sequenceImprint);

    if (! (sequenceImprint & HAS_SEQUENCE)) {
        printf("No sequences in hand for tile!\n");
        return;
    } else {
        currentPlayer->hand->drawn = table->lastDiscard->tile;
        table->lastDiscard->data |= DISCARD_CALLED;
        // rerenderHand(currentPlayer->hand);
        renderTable(table, 0);
    }

    // char inputBuffer[4];
    // printf("\rChi starting with which tile > ");
    // eraseNextN(36);
    // scanf("%s", inputBuffer);
    // uint8_t inputNumber = atoi(inputBuffer) & 255;

    uint8_t inputNumber = inputInt("\rChi starting with which tile > ", 24);

    // moveCursorTo(1, 32);

    unsigned char headValue = 0;
    if (inputNumber == 0) 
        headValue = currentPlayer->hand->drawn & ~IS_AKA & 255;
    else if (inputNumber > 0 && inputNumber < currentPlayer->hand->nClosed) {
        struct handTile* headTile = getHandTileAt(currentPlayer->hand, inputNumber-1);
        headValue = headTile->value & ~IS_AKA & 255;
    } else { // invalid index
        currentPlayer->hand->drawn = 0;
        printf("Index out of range!\n");
        return;
    }

    int headDistance = (table->lastDiscard->tile & ~IS_AKA & 255) - (headValue & 255);
    // printf("Head distance between %x and %x = %d\n", currentPlayer->hand->drawn, headValue, headDistance);
    if (headDistance < 0 || headDistance > 2) {
        currentPlayer->hand->drawn = 0;
        printf("Invalid index!\n");
        return ;
    } else {
        int toRemove[2];
        if (!mapChiRemoves(toRemove, headDistance, sequenceImprint)) {
            currentPlayer->hand->drawn = 0;
            return;
        }
        
        char meldData = MELD_SEQUENCE;
        for (int i = 0; i < 2; i++) {
            printf("%d ", i);
            char removedTile = removeTileFromHand(currentPlayer->hand, currentPlayer->hand->drawn + toRemove[i], 1, 1);
            meldData |= removedTile & IS_AKA;
        }
        
        meldData |= currentPlayer->hand->drawn & IS_AKA;

        addMeld(currentPlayer->hand, headValue & ~IS_AKA, meldData);

        currentPlayer->hand->drawn = 0;

        renderTable(table, 0);
        // rerenderHand(currentPlayer->hand);
        // printf("\n");

        currentPlayer->turnStage = DISCARDING;
    }
}

void handleOpenKan(struct table* table, struct player* currentPlayer) {
    char targetTile = table->lastDiscard->tile;
    char tilesFromHand = removeTileFromHand(currentPlayer->hand, targetTile & ~IS_AKA, 3, 1);
    if (tilesFromHand != 0) {
        table->lastDiscard->data |= DISCARD_CALLED;
        addMeld(currentPlayer->hand, targetTile & ~IS_AKA, MELD_KAN + ((targetTile | tilesFromHand) & IS_AKA));
        // rerenderHand(currentPlayer->hand);
        renderTable(table, 0);
        // printf("\n");
        currentPlayer->turnStage = DISCARDING;
    } else {
        printf("Insufficient ");
        renderTile(targetTile & ~IS_AKA);
        printf(" in hand!");
    }
}

void handleRiichi(struct table* table, struct player* currentPlayer) {
    int inputBuffer = 0;
    scanf("%d", &inputBuffer);
    struct hand* testHand = cloneHandWithout(currentPlayer->hand, inputBuffer);
}

void handleClosedKan(struct table* table, struct player* currentPlayer) {
    char inputBuffer[4];
    printf(CURSOR_UP"\rKan which tile? > ", 1);
    eraseNextN(40);
    scanf("%s", inputBuffer);
    uint8_t inputNumber = atoi(inputBuffer) & 255;

    if (inputNumber > currentPlayer->hand->nClosed) {
        printf("\a");
        return;
    }

    if (closedKan(currentPlayer->hand, inputNumber)) {
        char drawn = kanDraw(table->wall);
        currentPlayer->hand->drawn = drawn;
        currentPlayer->turnStage = HAS_14TH_TILE;
        // rerenderHand(currentPlayer->hand);
        renderTable(table, 0);
    } else {
        printf("\a");
        return;
    }
}

int handleAwaitDraw(struct table* table, struct player* currentPlayer) {
    char inputBuffer[4];
    if (currentPlayer->didRiichi)
        printf("(1) Draw | (5): Ron | (q): Quit | > ");
    else
        printf("(1) Draw | (2): Pon | (3): Chi | (4): Kan | (5): Ron | (q): Quit | > ");
    // fflush(stdout);
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
            // printf(CURSOR_UP, 1);
            // rerenderHand(currentPlayer->hand);
            renderTable(table, 0);
            currentPlayer->turnStage = HAS_14TH_TILE;
            break;
        case ACTION_PON:
            handlePon(table, currentPlayer);
            break;
        case ACTION_CHI:
            handleChi(table, currentPlayer);
            break;
        case ACTION_KAN:
            handleOpenKan(table, currentPlayer);
            break;
        case ACTION_RON:
            // TODO
            break;
        default:
            printf("Invalid input!\n");
            break;
    }
    return 1;
}

int handleAwaitAction(struct table* table, struct player* currentPlayer) {
    moveCursorTo(0, 24);

    // char inputBuffer[4];
    // if (currentPlayer->didRiichi) {
    //     printf("(1) Discard | (3): Tsumo | (4): Kan | > ");
    // } else {
    //     printf("(1) Discard | (2): Riichi | (3): Tsumo | (4): Kan | > ");
    // }

    // scanf("%s", inputBuffer);
    // int inputNumber = atoi(inputBuffer);

    char* prompt = "(1) Discard | (2): Riichi | (3): Tsumo | (4): Kan | > ";
    if (currentPlayer->didRiichi)
        prompt = "(1) Discard | (3): Tsumo | (4): Kan | > ";
    uint8_t inputNumber = inputInt(prompt, 0);

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
    if (currentPlayer->turnStage == CHOOSING_CHI) {
        chooseChi(table, currentPlayer);
    }   
    if (currentPlayer->turnStage == DISCARDING) {
        handleDiscard(table, currentPlayer);
    }
    return 1;
}

void handleSkippingPon(struct table* table, uint8_t playerData) {
    moveCursorTo(0, 24);
    table->playerTurn = playerData & 3;
    uint8_t inputNumber;
    while (1) {
        if (playerData & CAN_KAN) {
            moveCursorTo(24, 14);
            inputNumber = inputInt("\r(2) Pon | (4) Kan | > ", 24);
        } else {
            inputNumber = 2;
        }
        if (inputNumber == 2) {
            handlePon(table, table->players[playerData & 3]);
            break;
        } else if (inputNumber == 4) {
            handleOpenKan(table, table->players[playerData & 3]);
            break;
        }
    }
}
