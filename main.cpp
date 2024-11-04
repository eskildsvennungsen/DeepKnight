#include <bitset>
#include <iostream>

#include "src/Board.h"
#include "src/Types.h"

int main() {
   DeepKnight::Board board("r1bqkbnr/pppp1ppp/2n5/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R b KQk - 3 5");
   auto x = board.printBitBoard();
   std::cout << x << std::endl;

   return 0;
}
