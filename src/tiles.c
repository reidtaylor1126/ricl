#include "tiles.h"

char suitNames[] = {'!', 'm', 's', 'p', '!', 'D', 'W'};
char dragonValues[] = {'W', 'G', 'R'};
char windValues[] = {'E', 'S', 'W', 'N'};

char* tileDisplay(struct tile t) {
    char* display = malloc(2);

    uint8_t suit = (t.value >> 4) & 7;
    display[1] = suitNames[suit];
    
    uint8_t value = t.value & 15;
    if ((t.value & IS_WIND) == IS_WIND)
        display[0] = windValues[value];
    else if ((t.value & IS_DRAGON) == IS_DRAGON)
        display[0] = dragonValues[value];
    else
        display[0] = value+'0';

    return display;
}

void renderTile(struct tile t) {
    char* display = tileDisplay(t);
    if (t.value & IS_AKA)
        printf("[" ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "]", display);
    else
        printf("[%s]", display);
}

char tileValue(char* display) {
    return 0;
}
