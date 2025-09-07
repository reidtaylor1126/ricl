#include "tiles.h"

char suitNames[] = {'!', 'm', 's', 'p', '!', 'D', 'W'};
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

char* tileDisplay(char t) {
    /**
     * Generates a display string for a tile. Be sure to free the string returned by this function after using it!
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
    if (t & IS_AKA)
        printf("[" ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "]", display);
    else
        printf("[%s]", display);
    free(display);
}

char tileValue(char* display) {
    return 0;
}
