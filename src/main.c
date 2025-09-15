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

    unsigned char tiles[] = {
        0b00010001,
        0b00010001,
        0b00010001,
        0b00010010,
        0b00010011,
        0b00010100,
        0b00010101,
        0b10010101,
        0b00010101,
        0b00010110,
        0b00010110,
        0b00010111,
        0b00010111,
    };

    struct hand* myHand = malloc(sizeof(struct hand));

    for (int i = 0; i < 12; i++) {
        addTileToHand(myHand, tiles[i]);
    }

    // struct meld* myMeld = malloc(sizeof(struct meld));
    // myMeld->data = MELD_CLOSED + MELD_KAN;
    // myMeld->headTile = IS_DRAGON + 1;

    // myHand->meldsHead = myMeld;
    // myHand->nMelds = 1;

    renderHand(myHand);
    printf("\n");

    uint8_t possibleMelds[16];

    uint8_t nTriplets = findTriplets(myHand, possibleMelds);

    printf("Found %hu triplets\n", nTriplets);

    uint8_t nSequences = findSequences(myHand, &possibleMelds[nTriplets]);
    
    printf("Found %hu sequences\n", nSequences);

    for (int i = 0; possibleMelds[i] != 255 && i < 16; i++) {
        printf("%x, ", possibleMelds[i]);
    }
    printf("\n");

    destroyHand(myHand);

    return 0;
}
