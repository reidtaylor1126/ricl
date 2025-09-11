#include "main.h"
#include "table.h"
#include "scoring.h"

int main(int argc, const char* argv[]) {
    srand(time(NULL));
    // srand(5432);

    // struct table* myTable = createTable();

    // deal(myTable, 1);    

    // printTable(myTable);
    
    // tickTurn(myTable);

    // destroyTable(myTable);

    char tiles[] = {
        0b10010101,
        0b00110010,
        0b00010101,
        0b00110010,
        0b00010101,
        0b00110010,
        0b01100001,
        0b01100011,
        0b01100010,
        0b00101001,
        0b00100110
    };

    struct hand* myHand = malloc(sizeof(struct hand));

    for (int i = 0; i < 11; i++) {
        addTileToHand(myHand, tiles[i]);
    }

    struct meld* myMeld = malloc(sizeof(struct meld));
    myMeld->data = MELD_CLOSED + MELD_KAN;
    myMeld->headTile = IS_DRAGON + 1;

    myHand->meldsHead = myMeld;
    myHand->nMelds = 1;

    renderHand(myHand);
    printf("\n");

    scanForTriplets(myHand);

    renderHand(myHand);
    printf("\n");

    destroyHand(myHand);

    return 0;
}
