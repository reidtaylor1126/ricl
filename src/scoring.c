#include "scoring.h"

uint8_t hasSevenPairs(struct hand* hand) {
    uint8_t numNotPaired;
    char current = 0;
    uint8_t numCurrent = 0;
    struct handTile* cursor = hand->tilesHead;
    while (cursor != 0) {
        if ((cursor->value & ~IS_AKA) != current) {
            numNotPaired++;
            current = cursor->value & ~IS_AKA;
            numCurrent = 1;
        } else {
            if (numCurrent == 1) {
                numNotPaired--;
            }
        }
        cursor = cursor->next;
    }
    return numNotPaired == 0;
}

void scanForTriplets(struct hand* hand) {
    /**
     * In-place iterates through the hand and
     * turns any sequentially found triplets into melds
     */
    struct handTile* cursor = hand->tilesHead;
    struct handTile* anchor = 0;
    struct handTile* current = cursor;
    uint8_t numCurrent = 0;
    uint8_t hasAka = 0;
    while (cursor != 0) {
        if ((cursor->value & (SUIT_MASK + VALUE_MASK)) == (current->value & (SUIT_MASK+VALUE_MASK))) {
            if (cursor->value & IS_AKA)
                hasAka = 1;
            numCurrent++;
            current = cursor;

            renderTile(current->value);
            printf("%hu", numCurrent);

            if (numCurrent == 3) {
                // Create and add a new meld to the hand
                struct meld* newMeld = malloc(sizeof(struct meld));
                newMeld->headTile = current->value & ~IS_AKA;
                newMeld->data = MELD_TRIPLET + MELD_CLOSED;

                if (hasAka)
                    newMeld->data += IS_AKA;

                newMeld->next = hand->meldsHead;
                hand->meldsHead = newMeld;
                hand->nMelds++;

                cursor = cursor->next;
                current = cursor;

                struct handTile* deleter;
                struct handTile* toDelete;

                if (anchor == 0) {
                    deleter = hand->tilesHead;
                    hand->tilesHead = cursor;
                }
                else {
                    deleter = anchor->next;
                    anchor->next = cursor;
                }

                while(deleter != cursor) {
                    toDelete = deleter;
                    deleter = deleter->next;
                    free(toDelete);
                }

                numCurrent = 0;
                hasAka = 0;

            } else {
                renderTile(current->value);
                printf(" has only %hu, looking at ", numCurrent);
                renderTile(cursor->value);
                printf("\n");
                current = cursor;
                cursor = cursor->next;
                printf("Now looking at ");
                if (cursor != 0)
                    renderTile(cursor->value);
                else
                    printf("null");
                printf("\n");
            }
        } else {
            anchor = current;
            numCurrent = 0;
            hasAka = 0;
            cursor = cursor->next;
            current = cursor;
        }
    }
}

void scanForSequences(struct hand* hand) {

}

uint8_t isTenpai(struct hand* hand) {


    return 0;
}