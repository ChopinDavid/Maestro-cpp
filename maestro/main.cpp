#include <iostream>
#include "board.h"
#include "moves.h"
#include "perft.h"

using namespace std;

int main()
{
    Perft &perft = Perft::getInstance();
    Board board = Board::from("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    int a = perft.perft(board, 5);
    cout << a << endl;
    return 0;
}
