#include "main.h"
#include "table.h"
#include "turns.h"
#include "scoring.h"
#include "cursor.h"

int main(int argc, const char* argv[]) {
    long seed = time(NULL);
    // srand(seed);
    srand(1758948379);

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
