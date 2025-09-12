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
    struct handTile* cursor = hand->tilesHead;  // the leading cursor that moves first and is compared against
    struct handTile* anchor = 0;                // keeps track of the node before a triplet that will need its next updated
    struct handTile* previous = cursor;          // tracks the previous node in each triplet
    uint8_t numCurrent = 0;
    uint8_t hasAka = 0;
    while (cursor != 0) {
        // if the cursor's tile has the same non-aka value as the previous one
        if ((cursor->value & (SUIT_MASK + VALUE_MASK)) == (previous->value & (SUIT_MASK+VALUE_MASK))) {
            if (cursor->value & IS_AKA)
                hasAka = 1;
            numCurrent++;
            previous = cursor;

            if (numCurrent == 3) {
                // Create and add a new meld to the hand
                struct meld* newMeld = malloc(sizeof(struct meld));
                newMeld->headTile = previous->value & ~IS_AKA;
                newMeld->data = MELD_TRIPLET + MELD_CLOSED;

                if (hasAka)
                    newMeld->data += IS_AKA;

                newMeld->next = hand->meldsHead;
                hand->meldsHead = newMeld;
                hand->nMelds++;

                cursor = cursor->next;
                previous = cursor;

                // set up to remove triplet
                struct handTile* deleter;
                struct handTile* toDelete;

                // handle case where first tile is part of a triplet
                if (anchor == 0) {
                    deleter = hand->tilesHead;
                    hand->tilesHead = cursor;
                }
                else {
                    deleter = anchor->next;
                    anchor->next = cursor;
                }

                // delete nodes
                while(deleter != cursor) {
                    toDelete = deleter;
                    deleter = deleter->next;
                    free(toDelete);
                }

                numCurrent = 0;
                hasAka = 0;

            } else {
                previous = cursor;
                cursor = cursor->next;
            }
        } else {
            anchor = previous;
            numCurrent = 0;
            hasAka = 0;
            previous = cursor;
        }
    }
}

void scanForSequences(struct hand* hand) {
    
}

uint8_t isTenpai(struct hand* hand) {


    return 0;
}