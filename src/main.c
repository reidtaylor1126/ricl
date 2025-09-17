#include "main.h"
#include "table.h"
#include "scoring.h"

int main(int argc, const char* argv[]) {
    srand(time(NULL));
    // srand(5432);

    struct table* myTable = createTable();

    deal(myTable, 1);    

    printTable(myTable);
    
    tickTurn(myTable);
    
    destroyTable(myTable);

    return 0;
}
