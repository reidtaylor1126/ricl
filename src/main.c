#include "main.h"
#include "tiles.h"

int main(int argc, const char* argv[]) {
    struct tile normal5M = {0b00010101};
    struct tile red5M = {0b10010101};
    renderTile(red5M);
    renderTile (normal5M);
    printf(" Done!\n");
}
