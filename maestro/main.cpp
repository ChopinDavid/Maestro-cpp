#include <iostream>
#include "boardGeneration.h"
#include "moves.h"

using namespace std;

int main()
{
    BoardGeneration& boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.initiateStandardChess();
    moves.makeMoveAll("6444");
    cout << boardGeneration.boardString() << endl;
    return 0;
}
