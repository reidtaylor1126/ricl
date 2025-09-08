#include "main.h"
#include "table.h"

int main(int argc, const char* argv[]) {
    srand(time(NULL));
    // srand(5432);

    struct table* myTable = createTable();

    deal(myTable, 1);

    printTable(myTable);

    destroyTable(myTable);
}
