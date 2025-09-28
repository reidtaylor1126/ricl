#include "hand.h"
#include "cursor.h"

struct hand* createHand() {
    struct hand* newHand = malloc(sizeof(struct hand));
    newHand->tilesHead = 0;
    newHand->meldsHead = 0;
    newHand->nClosed = 0;
    newHand->nMelds = 0;
    newHand->drawn = 0;

    return newHand;
}

int addTileToHand(struct hand* hand, unsigned char tile) {
    if (hand->nClosed + (3*hand->nMelds) > 13) {
        printf("Hand full!\n");
        return -1;
    }

    hand->nClosed++;
    struct handTile* cursor = hand->tilesHead;
    struct handTile* follower = 0;

    int i = 0;
    while (1) {
        if (cursor == 0 || cursor->value > (tile & ~IS_AKA)) {
            struct handTile* newHandTile = malloc(sizeof(struct handTile));
            newHandTile->value = tile & ~IS_AKA;
            newHandTile->data = (tile & IS_AKA) + 1;
            newHandTile->next = cursor;
            hand->nUniqueClosed++;
            if (follower == 0) 
                hand->tilesHead = newHandTile;
            else
                follower->next = newHandTile;
            return i;
        } else if (cursor->value < (tile & ~IS_AKA)) {
            i += cursor->data & HANDTILE_COUNT_MASK;
            follower = cursor;
            cursor = cursor->next;
        } else {
            cursor->data++;
            cursor->data |= (tile & IS_AKA);
            return i+(cursor->data & ~HANDTILE_COUNT_MASK)-1;
        }
    }
}

int addDrawnToHand(struct hand* hand) {
    if (hand->drawn == 0)
        return -1;
    int addedAt = addTileToHand(hand, hand->drawn);
    hand->drawn = 0;
    return addedAt;
}

char getTileFromHand(struct hand* hand, uint8_t index) {
    struct handTile* cursor = hand->tilesHead;
    uint8_t i = 0;
    while (cursor != 0) {
        if (i == index) {
            return cursor->value + (cursor->data & IS_AKA);
        } else if (i > index){
            return cursor->value;
        }
        i += cursor->data & HANDTILE_COUNT_MASK;
        cursor = cursor->next;
    }
    return 0;
}

uint8_t countInHand(struct hand* hand, char tile) {
    struct handTile* cursor = hand->tilesHead;
    while(cursor != 0) {
        if ((cursor->value & ~IS_AKA) == (tile & ~IS_AKA))
            return cursor->data & HANDTILE_COUNT_MASK;
        else if ((cursor->value & ~IS_AKA) > (tile & ~IS_AKA))
            return 0;
        cursor = cursor->next;
    }
    return 0;
}

char removeFromHand(struct hand* hand, uint8_t index, uint8_t count) {
    struct handTile* remove = hand->tilesHead;
    struct handTile* previous = 0;
    int i = 0;
    while (remove != 0 && i < hand->nClosed) {
        if (i == index) {
            break;
        } else {
            i += remove->data & HANDTILE_COUNT_MASK;

            // if this handTile is the one we're searching for but there's more than 1
            if (i > index) {
                break;
            } else {
                previous = remove;
                remove = remove->next;
            }
        }
    }

    if ((remove->data & ~IS_AKA) < count)
        return 0;

    remove->data -= count;

    char value = remove->value;
    if (remove->data & IS_AKA && i == index) {
        value += IS_AKA;
        remove->data -= IS_AKA;
    }

    if ((remove->data & HANDTILE_COUNT_MASK) == 0) {
        if (previous == 0)
            hand->tilesHead = remove->next;
        else
            previous->next = remove->next;
        free(remove);
        hand->nUniqueClosed--;
    }
    hand->nClosed -= count;
    return value;
}

char removeTileFromHand(struct hand* hand, char tile, uint8_t count, uint8_t takeAka) {
    struct handTile* remove = hand->tilesHead;
    struct handTile* previous = 0;
    uint8_t i = 0;
    while (remove != 0) {
        if (remove->value == (tile & ~IS_AKA)) {
            break;
        } else if (remove->value > (tile & ~IS_AKA)) {
            return 0;
        }
        i += remove->data & HANDTILE_COUNT_MASK;
        previous = remove;
        remove = remove->next;
    }

    if ((remove->data & ~IS_AKA) < count)
        return 0;

    remove->data -= count;

    char value = remove->value;
    if ((tile & IS_AKA || takeAka) && remove->data & IS_AKA) {
        value += IS_AKA;
        remove->data -= IS_AKA;
    }

    if ((remove->data & HANDTILE_COUNT_MASK) == 0) {
        if (previous == 0)
            hand->tilesHead = remove->next;
        else
            previous->next = remove->next;
        free(remove);
        hand->nUniqueClosed--;
    }
    hand->nClosed -= count;
    return value;
}

char closedKan(struct hand* hand, uint8_t tileIndex) {
    
    struct handTile* targetHandTile = getHandTileAt(hand, tileIndex);
    char tileValue = targetHandTile->value & (~IS_AKA);
    uint8_t tileData = targetHandTile->data;
    uint8_t tileCount = tileData & HANDTILE_COUNT_MASK;
    if (tileCount == 4 || (tileCount == 3 && tileValue == (hand->drawn & !IS_AKA))) {
        removeFromHand(hand, tileIndex, tileCount);
        addMeld(hand, tileValue, MELD_KAN + MELD_CLOSED);
        hand->meldsHead->data += tileData & IS_AKA;
        return 1;
    } 
    return 0;
}

void addMeld(struct hand* hand, char headTile, char data) {
    struct meld* newMeld = malloc(sizeof(struct meld));
    newMeld->headTile = headTile;
    newMeld->data = data;
    newMeld->next = hand->meldsHead;
    hand->meldsHead = newMeld;
}

void renderMeld(struct meld* meld, char* sep) {
    char tileToRender;
    switch (meld->data & MELD_TYPE_MASK) {
        case MELD_SEQUENCE:
            for (uint8_t i = 0; i < 3; i++) {
                tileToRender = meld->headTile+i;
                if ((meld->data & IS_AKA) && (tileToRender & VALUE_MASK) == 5)
                    renderTile(tileToRender | IS_AKA);
                else
                    renderTile(tileToRender);
                printf("%s", sep);
            }
            break;
        case MELD_TRIPLET:
            for (uint8_t i = 0; i < 3; i++) {
                if (meld->data & IS_AKA && i == 1)
                    renderTile(meld->headTile + IS_AKA);
                else
                    renderTile(meld->headTile & ~IS_AKA);
                printf("%s", sep);
            }
            break;
        case MELD_KAN:
            for (uint8_t i = 0; i < 4; i++) {
                if (meld->data & MELD_CLOSED && (i == 0 || i == 3)) 
                    printf("[ ]");
                else {
                    if (meld->data & IS_AKA && i == 1)
                        renderTile(meld->headTile + IS_AKA);
                    else
                        renderTile(meld->headTile);
                    printf("%s", sep);
                }
            }
            break;
    }
}

void renderHand(struct hand* hand) {
    struct handTile* tileCursor = hand->tilesHead;
    while (tileCursor != 0) {
        for (int i = 0; i < (tileCursor->data & HANDTILE_COUNT_MASK); i++) {
            if (tileCursor->data & IS_AKA && i == 0)
                renderTile(tileCursor->value + IS_AKA);
            else
                renderTile(tileCursor->value);
        }
        if (tileCursor->next != 0 && (tileCursor->value & SUIT_MASK) != (tileCursor->next->value & SUIT_MASK))
            printf(" ");
        tileCursor = tileCursor->next;
    }
    printf(" | ");
    if (hand->drawn != 0) {
        renderTile(hand->drawn);
    } else 
        printf("    ");
    printf(" | ");
    // printf(" | %hu Melds: ", hand->nMelds);
    struct meld* meldCursor = hand->meldsHead;
    while (meldCursor != 0) {
        renderMeld(meldCursor, "");
        printf(" ");
        meldCursor = meldCursor->next;
    }

    eraseNextN(12);
}

void renderSideHand(struct hand* hand, uint8_t showClosed, char* sep) {
    if (showClosed) {
        struct handTile* tileCursor = hand->tilesHead;
        while (tileCursor != 0) {
            for (int i = 0; i < (tileCursor->data & HANDTILE_COUNT_MASK); i++) {
                if (tileCursor->data & IS_AKA && i == 0)
                    renderTile(tileCursor->value + IS_AKA);
                else
                    renderTile(tileCursor->value);
                printf("%s", sep);
            }
            tileCursor = tileCursor->next;
        }
        printf("----%s", sep);
        if (hand->drawn != 0) {
            renderTile(hand->drawn);
            printf("%s", sep);
        }
    } else {
        for (int i = 0; i < hand->nClosed; i++) {
            printf("[  ]%s", sep);
        }
        printf("----%s", sep);
        if (hand->drawn != 0) 
            printf("[  ]%s", sep);
        else
            printf("    %s", sep);
    }
    printf("----%s", sep);

    struct meld* meldCursor = hand->meldsHead;
    while (meldCursor != 0) {
        renderMeld(meldCursor, sep);
        // printf("%s", sep);
        meldCursor = meldCursor->next;
    }
}

void renderOppositeHand(struct hand* hand, uint8_t showClosed) {
    moveCursorTo(OPPOSITE_HAND_POSITION);
    if (showClosed) {
        struct handTile* tileCursor = hand->tilesHead;
        while (tileCursor != 0) {
            for (int i = 0; i < (tileCursor->data & HANDTILE_COUNT_MASK); i++) {
                if (tileCursor->data & IS_AKA && i == 0)
                    renderTile(tileCursor->value + IS_AKA);
                else
                    renderTile(tileCursor->value);
                printf(CURSOR_LEFT, 8);
            }
            tileCursor = tileCursor->next;
        }

        printf("  | "CURSOR_LEFT, 7);

        if (hand->drawn != 0) {
            renderTile(hand->drawn);
            printf(CURSOR_LEFT, 8);
        }
    } else {
        for (int i = 0; i < hand->nClosed; i++) {
            printf("[  ]" CURSOR_LEFT, 8);
        }
        printf("  | " CURSOR_LEFT, 7);
        if (hand->drawn != 0) 
            printf("[  ]" CURSOR_LEFT, 8);
        else
            printf(CURSOR_LEFT, 4);
    }
    printf("  | "CURSOR_LEFT, 7);

    struct meld* meldCursor = hand->meldsHead;
    char* meldSep = malloc(16);
    sprintf(meldSep, CURSOR_LEFT, 8);
    while (meldCursor != 0) {
        renderMeld(meldCursor, meldSep);
        printf(CURSOR_LEFT, 2);
        meldCursor = meldCursor->next;
    }
    free(meldSep);
}

void renderLeftHand(struct hand* hand, uint8_t showClosed) { 
    moveCursorTo(LEFT_HAND_POSITION);
    char* sep = malloc(16);
    sprintf(sep, CURSOR_LEFT CURSOR_DOWN, 4, 1);
    renderSideHand(hand, showClosed, sep);
    free(sep);
}

void renderRightHand(struct hand* hand, uint8_t showClosed) {
    moveCursorTo(RIGHT_HAND_POSITION);
    char* sep = malloc(16);
    sprintf(sep, CURSOR_LEFT CURSOR_UP, 4, 1);
    renderSideHand(hand, showClosed, sep);
    free(sep);
}

void destroyHand(struct hand* hand) {
    struct handTile* tileCursor = hand->tilesHead;
    if (tileCursor != 0) {
        while (tileCursor->next != 0) {
            struct handTile* nextTile = tileCursor->next;
            free(tileCursor);
            tileCursor = nextTile;
        }
    }

    struct meld* meldCursor = hand->meldsHead;
    if (meldCursor != 0) {
        while (meldCursor->next != 0) {
            struct meld* nextMeld = meldCursor->next;
            free(meldCursor);
            meldCursor = nextMeld;
        }
    }

    free(hand);
}

struct handTile* getHandTileAt(struct hand* hand, uint8_t index) {
    struct handTile* cursor = hand->tilesHead;
    uint8_t counted = 0;

    while (cursor != 0) {
        counted += cursor->data & HANDTILE_COUNT_MASK;
        if (counted > index)
            return cursor;
        cursor = cursor->next;
    }
    
    return 0;
}

struct hand* cloneHandWithout(struct hand* hand, uint8_t index) {
    return hand;
}
