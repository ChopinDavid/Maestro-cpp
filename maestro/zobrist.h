#pragma once
#include <cmath>
#include <random>
#include "board.h"

class Zobrist {
   private:
    // Mersenne twister for random number generation
    std::uint64_t hash_board[64][12];
    std::uint64_t random_bitstring();

   public:
    Zobrist();
    std::uint64_t hash(Board board);
};