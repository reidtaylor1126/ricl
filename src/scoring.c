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

uint8_t findTriplets(struct hand* hand, uint8_t* ends) {
    ends[0] = 255;
    if (hand->nClosed < 3)
        return 0;
        
    struct handTile* cursor = hand->tilesHead;
    uint8_t index = 0;
    uint8_t numTriplets = 0;

    while (cursor != 0) {
        if ((cursor->data & HANDTILE_COUNT_MASK) > 2) {
            ends[numTriplets] = index + 0x80;
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

uint8_t findSequencesFor(struct hand* hand, char tile) {
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
 * TODO use weighted interval scheduling algorithm to find tenpai
 * identify all possible triplets and sequences
 * store last index of meld in 0-terminated arrays
 * concatenate arrays
 * sort arrays
 * iterate through, choosing the first meld that does not overlap previous
 * if 4 melds, then tenpai
 * if 3 melds, search for pairs
 *   - if 2 pairs, tenpai, add both pair values to waits
 *   - if 1 pair
 *       - if other 2 are sequential, tenpai, add sequence members (if exist) to waits
 *       - else, noten
 *   - else, noten
 * else, noten
 *
 * run iteration again but backwards
 * merge waits
 * 
 * 7 pairs: identify all pairs, if 1 left then tenpai
 *
 * 13 orphans: for each tile: if the next tile is not the same or the next honor, set flag. if flag already set, then noten. 
*/ 

uint8_t findWaits(struct hand* hand, char* waits) {
    

    return 0;
}