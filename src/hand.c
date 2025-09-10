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
    if (hand->nClosed + (3*hand->nMelds) >= 14) {
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
    } else if ((hand->tilesHead->value&VALUE_MASK) > (tile&VALUE_MASK)) {
        newTile->next = hand->tilesHead;
        hand->tilesHead = newTile;
        hand->nClosed++;
        return 0;
    } else {
        struct handTile* cursor = hand->tilesHead;
        for(int i = 0; i < hand->nClosed+1; i++) {
            if (cursor->next == 0 || (cursor->next->value & VALUE_MASK) > (tile & VALUE_MASK)) {
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

// TODO remove this function and handle evaluation in the pon and kan functions (return status)
uint8_t countInHand(struct hand* hand, char tile) {
    uint8_t count = 0;
    
    for (struct handTile* cursor = hand->tilesHead; cursor != 0; cursor = cursor->next) {
        if ((cursor->value & VALUE_MASK) == (tile & VALUE_MASK)) {
            count++;
        } else if ((cursor->value & VALUE_MASK) > (tile & VALUE_MASK)) {
            return count;
        }
    }

    return count;
}

void pon(struct hand* hand, char tile) {

}

void chi(struct hand* hand, char lowTile) {

}

char chiOptions(struct hand* hand, char tile) {
    return 0;
}

char kan(struct hand* hand, char tile) {

}

void renderHand(struct hand* hand) {
    struct handTile* cursor = hand->tilesHead;
    while (cursor->next != 0) {
        renderTile(cursor->value);
        cursor = cursor->next;
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
