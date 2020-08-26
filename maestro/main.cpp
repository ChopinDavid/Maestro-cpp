#include <iostream>
#include "board.h"
#include "moves.h"
#include "perft.h"

using namespace std;

int main()
{
    Perft &perft = Perft::getInstance();

    Board board = Board::from("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    int moves = perft.perft(board, 4);
    cout << moves << endl;
    return 0;
}
