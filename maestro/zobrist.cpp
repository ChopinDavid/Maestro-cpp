#include "zobrist.h"
#include <chrono>
#include <cmath>
#include <random>

std::uint64_t Zobrist::random_bitstring() {
    std::mt19937_64 rng(
        std::chrono::steady_clock::now().time_since_epoch().count());
    return rng();
}

Zobrist::Zobrist() {
    // Initialize the hash board
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 12; j++) {
            hash_board[i][j] = random_bitstring();
        }
    }
}

std::uint64_t Zobrist::hash(Board board) {
    std::uint64_t hash = 0;
    for (int i = 0; i < 64; i++) {
        uint64_t bitRepOfSquare = 1ULL << i;
        if ((board.getWP() & bitRepOfSquare) != 0) {
            hash = hash ^ hash_board[i][0];
            continue;
        }
        if ((board.getWN() & bitRepOfSquare) != 0) {
            hash = hash ^ hash_board[i][1];
            continue;
        }
        if ((board.getWB() & bitRepOfSquare) != 0) {
            hash = hash ^ hash_board[i][2];
            continue;
        }
        if ((board.getWR() & bitRepOfSquare) != 0) {
            hash = hash ^ hash_board[i][3];
            continue;
        }
        if ((board.getWQ() & bitRepOfSquare) != 0) {
            hash = hash ^ hash_board[i][4];
            continue;
        }
        if ((board.getWK() & bitRepOfSquare) != 0) {
            hash = hash ^ hash_board[i][5];
            continue;
        }
        if ((board.getBP() & bitRepOfSquare) != 0) {
            hash = hash ^ hash_board[i][6];
            continue;
        }
        if ((board.getBN() & bitRepOfSquare) != 0) {
            hash = hash ^ hash_board[i][7];
            continue;
        }
        if ((board.getBB() & bitRepOfSquare) != 0) {
            hash = hash ^ hash_board[i][8];
            continue;
        }
        if ((board.getBR() & bitRepOfSquare) != 0) {
            hash = hash ^ hash_board[i][9];
            continue;
        }
        if ((board.getBQ() & bitRepOfSquare) != 0) {
            hash = hash ^ hash_board[i][10];
            continue;
        }
        if ((board.getBK() & bitRepOfSquare) != 0) {
            hash = hash ^ hash_board[i][11];
            continue;
        }
    }
    return hash;
}