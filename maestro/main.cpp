#include <iostream>
#include "board.h"
#include "moves.h"
#include "perft.h"
#include <chrono>

using namespace std;

int main()
{
    Perft &perft = Perft::getInstance();
    Board board = Board::from("r1b2rk1/ppppqpp1/2n2n1p/2b1p3/2B1P3/2NPBN2/PPPQ1PPP/R3K2R w KQ - 4 8");
    auto start_time = std::chrono::high_resolution_clock::now();
    perft.perft(board, 3);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    std::cout << "took " << time/std::chrono::milliseconds(1) << "ms to run.\n";
    return 0;
}
