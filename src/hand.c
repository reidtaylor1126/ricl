#include "hand.h"

struct hand* createHand() {
    struct hand* newHand = malloc(sizeof(struct hand));
    newHand->tilesHead = 0;
    newHand->meldsHead = 0;
    newHand->nClosed = 0;
    newHand->nMelds = 0;
    newHand->drawn = 0;

    return newHand;
}

int addTileToHand(struct hand* hand, char tile) {
    if (hand->nClosed + (3*hand->nMelds) > 13) {
        printf("Hand full!\n");
        return -1;
    }

    struct handTile* newTile = malloc(sizeof(struct handTile));
    newTile->value = tile;
    newTile->next = 0;

    if (hand->nClosed == 0 || hand->tilesHead == 0) {
        hand->tilesHead = newTile;
        hand->nClosed++;
        return 0;
    } else if ((hand->tilesHead->value&(SUIT_MASK+VALUE_MASK)) > (tile&(SUIT_MASK+VALUE_MASK))) {
        newTile->next = hand->tilesHead;
        hand->tilesHead = newTile;
        hand->nClosed++;
        return 0;
    } else {
        struct handTile* cursor = hand->tilesHead;
        for(int i = 0; i < hand->nClosed+1; i++) {
            if (cursor->next == 0 || (cursor->next->value & ~IS_AKA) > (tile & ~IS_AKA) || cursor->next->value == (tile + IS_AKA)) {
                newTile->next = cursor->next;
                cursor->next = newTile;
                hand->nClosed++;
                return i;
            }
            cursor = cursor->next;
        }
    }
    return -1;
}

char removeFromHand(struct hand* hand, uint8_t index) {
    struct handTile* remove = hand->tilesHead;
    if (index == 0) {
        hand->tilesHead = remove->next;
    } else {
        struct handTile* previous;
        for (int i = 1; i < index; i++) {
            previous = remove;
            remove = remove->next;
        }
        previous->next = remove->next;
    }
    char value = remove->value;
    free(remove);
    return value;
}

// TODO remove this function and handle evaluation in the pon and kan functions (return status)
uint8_t countInHand(struct hand* hand, char tile) {
    uint8_t count = 0;
    
    for (struct handTile* cursor = hand->tilesHead; cursor != 0; cursor = cursor->next) {
        if ((cursor->value & ~IS_AKA) == (tile & ~IS_AKA)) {
            count++;
        } else if ((cursor->value & ~IS_AKA) > (tile & ~IS_AKA)) {
            return count;
        }
    }

    return count;
}

char pon(struct hand* hand, char tile) {
    return 0;
}

char chi(struct hand* hand, char lowTile) {
    return 0;

}

char chiOptions(struct hand* hand, char tile) {
    return 0;
}

char closedKan(struct hand* hand, char tile) {
    return 0;

}

void renderMeld(struct meld* meld) {
    char tileToRender;
    switch (meld->data & MELD_TYPE_MASK) {
        case MELD_SEQUENCE:
            for (uint8_t i = 0; i < 3; i++) {
                tileToRender = meld->headTile+i;
                if ((meld->data & IS_AKA) && (tileToRender & VALUE_MASK) == 5)
                    tileToRender &= IS_AKA;
                renderTile(tileToRender);
            }
            break;
        case MELD_TRIPLET:
            for (uint8_t i = 0; i < 3; i++) {
                if (meld->data & IS_AKA && i == 1)
                    renderTile(meld->headTile + IS_AKA);
                else
                    renderTile(meld->headTile);
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
                }
            }
            break;
    }
}

void renderHand(struct hand* hand) {
    struct handTile* tileCursor = hand->tilesHead;
    while (tileCursor != 0) {
        renderTile(tileCursor->value);
        tileCursor = tileCursor->next;
    }
    if (hand->drawn != 0) {
        printf(" | ");
        renderTile(hand->drawn);
    }
    // printf(" | %hu Melds: ", hand->nMelds);
    printf(" | ");
    struct meld* meldCursor = hand->meldsHead;
    while (meldCursor != 0) {
        renderMeld(meldCursor);
        printf(" ");
        meldCursor = meldCursor->next;
    }
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

struct hand* cloneHandWithout(struct hand* hand, uint8_t index) {
    return hand;
}
