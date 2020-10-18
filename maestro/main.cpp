#include <iostream>
#include "board.h"
#include "moves.h"
#include "perft.h"
#include <chrono>

using namespace std;

int main()
{
    Perft &perft = Perft::getInstance();
    Board board = Board::from("KR6/8/8/8/8/8/8/6rk w - - 0 1");
    auto start_time = std::chrono::high_resolution_clock::now();
    perft.perft(board, 4);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    std::cout << "took " << time/std::chrono::milliseconds(1) << "ms to run.\n";
    return 0;
}
