#include "scoring.h"

uint8_t getSuits(struct hand* hand, struct handTile** buffer) {
    struct handTile* cursor = hand->tilesHead;
    uint8_t numSuits = 0;
    char currentSuit = cursor->value & SUIT_MASK;
    while (cursor != 0) {
        char cursorSuit = cursor->value & SUIT_MASK;
        if (cursorSuit != currentSuit) {
            currentSuit = cursorSuit;
            buffer[numSuits] = cursor;
            numSuits++;
        }
        cursor = cursor->next;
    }
    return numSuits+1;
}

char orphansWait(struct hand* hand) {
    if (hand->nClosed != 13)
        return 0;

    char pattern[] = {
        SUIT_MAN + 1,
        SUIT_MAN + 9,
        SUIT_SOU + 1,
        SUIT_SOU + 9,
        SUIT_PIN + 1,
        SUIT_PIN + 9,
        IS_DRAGON,
        IS_DRAGON + 1,
        IS_DRAGON + 2,
        IS_WIND,
        IS_WIND + 1,
        IS_WIND + 2,
        IS_WIND + 3
    };

    uint16_t match = ORPHANS_TEMPLATE;
    uint8_t missing = 0;

    uint8_t i = 0;
    struct handTile* cursor = hand->tilesHead;
    while (cursor != 0) {
        if (cursor->value == pattern[i]) {
            if ((cursor->data & HANDTILE_COUNT_MASK) > 2) {
                return 0;
            } else if ((cursor->data & HANDTILE_COUNT_MASK) == 2) {
                match ^= 1 << 15;
            }
            match ^= 1 << i;
            i++;
        } else if (i < 12 && cursor->value == pattern[i+1]) {
            if (missing != 0) {
                return 0;
            }
            missing = pattern[i];
            i+=2;
        } else {
            return 0;
        }
        cursor = cursor->next;
    }

    if (match == 1 << 15)
        return 255;

    return missing;
}

uint8_t getExclusivePairs(struct hand* hand, uint8_t* locations) {
    uint8_t numPairs = 0;
    uint8_t index = 0;
    struct handTile* cursor = hand->tilesHead;
    while (cursor != 0) {
        if ((cursor->data & HANDTILE_COUNT_MASK) == 2) {
            locations[numPairs] = index;
            numPairs++;
        }
        cursor = cursor->next;
        index++;
    }
    return numPairs;
}

uint8_t findTriplets(struct hand* hand, uint8_t* ends) {
    ends[0] = 255;
    if (hand->nClosed < 3)
        return 0;
        
    struct handTile* cursor = hand->tilesHead;
    uint8_t index = 0;
    uint8_t numTriplets = 0;

    while (cursor != 0) {
        if ((cursor->data & HANDTILE_COUNT_MASK) > 2) {
            ends[numTriplets] = index + IS_TRIPLET;
            numTriplets++;
            ends[numTriplets] = 255;
        }

        index++;
        cursor = cursor->next;
    }
    return numTriplets;
}

uint8_t findSequences(struct hand* hand, uint8_t* ends) {
    ends[0] = 255;
    if (hand->nClosed < 3)
        return 0;

    struct handTile* anchor = hand->tilesHead;
    struct handTile* cursor = anchor->next;
    char nextExpected = (anchor->value & ~IS_AKA)+1;
    uint8_t fewestInSeq = anchor->data & HANDTILE_COUNT_MASK;
    uint8_t seqLength = 1;
    uint8_t numSequences = 0;
    uint8_t index = 0;

    while (cursor != 0 && !(anchor->value & IS_HONOR)) {
        if ((cursor->value & ~IS_AKA) == nextExpected) {
            seqLength++;
            if ((cursor->data & HANDTILE_COUNT_MASK) < fewestInSeq)
                fewestInSeq = cursor->data & HANDTILE_COUNT_MASK;
            if (seqLength == 3) {
                for (int i = 0; i < fewestInSeq; i++) {
                    ends[numSequences] = index;
                    numSequences++;
                }
                ends[numSequences] = 255;
                anchor = anchor->next;
                cursor = anchor->next;
                nextExpected = (anchor->value & ~IS_AKA)+1;
                fewestInSeq = anchor->data & HANDTILE_COUNT_MASK;
                seqLength = 1;
                index++;
            } else {
                nextExpected++;
                cursor = cursor->next;
            }
        } else if ((cursor->value & ~IS_AKA) > nextExpected) {
            anchor = anchor->next;
            cursor = anchor->next;
                nextExpected = (anchor->value & ~IS_AKA)+1;
            fewestInSeq = anchor->data & HANDTILE_COUNT_MASK;
            seqLength = 1;
            index++;
        }
    }

    return numSequences;
}

uint8_t findSequencesForTile(struct hand* hand, char tile) {
    if (tile & IS_HONOR)
        return 0;

    uint8_t tileExistsRegister = 0;
    uint8_t bitCursor = 16;
    char tileTarget = (tile & ~IS_AKA) - 2;
    struct handTile* tileCursor = hand->tilesHead;

    while (tileCursor != 0 && bitCursor > 0) {
        if ((tileCursor->value & ~IS_AKA) == tileTarget) {
            tileExistsRegister |= bitCursor;
            tileTarget++;
            bitCursor >>= 1;
            tileCursor = tileCursor->next;
        } else if ((tileCursor->value & ~IS_AKA) > tileTarget) {
            tileTarget++;
            bitCursor >>= 1;
        } else {
            tileCursor = tileCursor->next;
        }
    }

    if ((tileExistsRegister & SEQUENCE_ABOVE_MASK) == SEQUENCE_ABOVE_MASK ||
        (tileExistsRegister & SEQUENCE_CENTER_MASK) == SEQUENCE_CENTER_MASK ||
        (tileExistsRegister & SEQUENCE_BELOW_MASK) == SEQUENCE_BELOW_MASK) {
            tileExistsRegister |= HAS_SEQUENCE;
        }

    return tileExistsRegister;
}

/** 
 * Strategy: check special cases first
 * 
 * 7 pairs: if there are 6 pairs then the sole tile is the wait
 *
 * 13 orphans: 
*/ 

uint8_t findWaits(struct hand* hand, char* waits) {


    return 0;
}