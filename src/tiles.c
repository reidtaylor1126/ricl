#include "tiles.h"

char suitNames[] = {'!', 'm', 's', 'p', '#', 'D', 'W'};
char dragonValues[] = {'W', 'G', 'R'};
char windValues[] = {'E', 'S', 'W', 'N'};


char* generateAllTiles() {
    char* tiles = (char*) malloc(136);
    uint8_t tilesGenerated = 0;

    // Numbers
    for (int suit = SUIT_MAN; suit <= SUIT_PIN; suit += 16) {
        for (int i = 1; i <= 9; i++) {
            for (int j = 0; j < 4; j++) {
                tiles[tilesGenerated] = suit | i;
                if (i == 5 && j == 0) {
                    tiles[tilesGenerated] |= IS_AKA;
                }
                tilesGenerated++;
            }
        }
    }

    // Dragons
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            tiles[tilesGenerated] = IS_DRAGON | i;
            tilesGenerated++;
        }
    }

    // Winds
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tiles[tilesGenerated] = IS_WIND | i;
            tilesGenerated++;
        }
    }

    return tiles;
}

char* shuffleTiles(char* tiles) {
    return (_shuffleTiles(tiles, 136));
}

char* _shuffleTiles(char* subsetHead, size_t nToShuffle) {
/**
 * Recursive divide-and-conquer shuffling algorithm, returns in-place shuffled array.
 */
    if (nToShuffle == 1) {
        return subsetHead;
    }
    size_t nLeft = nToShuffle/2;
    size_t nRight = nToShuffle - nLeft;

    _shuffleTiles(subsetHead, nLeft);
    _shuffleTiles(subsetHead+nLeft, nRight);

    char* merged = malloc(nToShuffle);

    size_t remainingLeft = nLeft;
    size_t remainingRight = nRight;

    while(remainingLeft && remainingRight) {
        size_t take = rand() % (remainingLeft+remainingRight);
        if (take < remainingLeft) {
            merged[remainingLeft+remainingRight-1] = subsetHead[remainingLeft-1];
            remainingLeft--;
        } else {
            merged[remainingLeft+remainingRight-1] = subsetHead[nLeft+remainingRight-1];
            remainingRight--;
        }
    }

    while (remainingLeft) {
        merged[remainingLeft-1] = subsetHead[remainingLeft-1];
        remainingLeft--;
    }
    
    while (remainingRight) {
        merged[remainingLeft+remainingRight-1] = subsetHead[nLeft+remainingRight-1];
        remainingRight--;
    }

    for (size_t i = 0; i < nToShuffle; i++) {
        subsetHead[i] = merged[i];
    }
    free(merged);
    return subsetHead;
}

char* tileDisplay(char t) {
    /**
     * Generates a display string for a tile. 
     * Be sure to free the string returned by this function after using it!
     */
    char* display = malloc(3);

    uint8_t suit = (t >> 4) & 7;
    display[1] = suitNames[suit];
    
    uint8_t value = t & 15;
    if ((t & IS_WIND) == IS_WIND)
        display[0] = windValues[value];
    else if ((t & IS_DRAGON) == IS_DRAGON)
        display[0] = dragonValues[value];
    else
        display[0] = value+'0';

    display[2] = 0;

    return display;
}

void renderTile(char t) {
    char* display = tileDisplay(t);
    if (t & IS_AKA || (t & RED_DRAGON) == RED_DRAGON)
        printf("[" ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "]", display);
    else if ((t & GREEN_DRAGON) == GREEN_DRAGON) 
        printf("[" ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "]", display);
    else if ((t & IS_DRAGON) == IS_DRAGON)
        printf("[" ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "]", display);
    else if ((t & IS_WIND) == IS_WIND) 
        printf("[" ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET "]", display);
    else
        printf("[%s]", display);

    free(display);
}

char tileValue(char* display) {
    return 0;
}
