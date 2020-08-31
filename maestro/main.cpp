#include <iostream>
#include "board.h"
#include "moves.h"
#include "perft.h"

using namespace std;

int main()
{
    Perft &perft = Perft::getInstance();
    Board board = Board::from("r1b2rk1/ppppqpp1/2n2n1p/2b1p3/2B1P3/2NPBN2/PPPQ1PPP/R3K2R w KQ - 4 8");
    perft.divide(board, 2);
    return 0;
}
