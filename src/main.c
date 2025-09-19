#include "main.h"
#include "table.h"
#include "scoring.h"

int main(int argc, const char* argv[]) {
    srand(time(NULL));
    // srand(5432);

    struct table* myTable = createTable();

    deal(myTable, 1);    

    renderHand(myTable->players[0]->hand);
    printf("\n");

    while (myTable->gameRunning == 1) {
        tickTurn(myTable);
        struct player* currentPlayer = myTable->players[myTable->playerTurn];
        if (currentPlayer->turnStage == NOT_TURN)  {
            advanceTurn(myTable);
            renderDiscards(myTable->players[myTable->playerTurn]);
            renderHand(myTable->players[myTable->playerTurn]->hand);
            printf("\n");
        }
    }
    
    destroyTable(myTable);

    return 0;
}
