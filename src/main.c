#include "main.h"
#include "table.h"
#include "turns.h"
#include "scoring.h"
#include "cursor.h"

int main(int argc, const char* argv[]) {

    long seed;

    if (argc > 1) 
        seed = atoi(argv[1]);
    else
        seed = time(NULL);
    // seed = 1758948379;
    srand(seed);

    struct table* myTable = createTable();

    deal(myTable, 1);    

    renderTable(myTable, 0);
    moveCursorTo(0, 24);

    while (myTable->gameRunning == 1) {
        tickTurn(myTable);
        struct player* currentPlayer = myTable->players[myTable->playerTurn];
        if (currentPlayer->turnStage == NOT_TURN)  {
            
            while (! waitForNextReady(myTable));

            // renderDiscards(myTable->players[myTable->playerTurn]);
            renderTable(myTable, 0);
            moveCursorTo(0, 24);
        }
    }

    destroyTable(myTable);
    printf("Game seed was: %ld\n", seed);

    return 0;
}
