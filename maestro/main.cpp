#include <iostream>
#include "board.h"
#include "moves.h"

using namespace std;

int main()
{
    Moves &moves = Moves::getInstance();

    Board board = Board::initiateStandardChess();
    board = moves.makeMoveAll(board, "6444");
    cout << board.boardString() << endl;
    return 0;
}
