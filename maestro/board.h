#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED
#include <cmath>
#include <string>
#include <stdint.h>
#include <ctype.h>
#include "bitboard.h"
#include "phase.h"
using namespace std;

class Board
{
private:
    uint64_t WP;
    uint64_t WN;
    uint64_t WB;
    uint64_t WR;
    uint64_t WQ;
    uint64_t WK;
    uint64_t BP;
    uint64_t BN;
    uint64_t BB;
    uint64_t BR;
    uint64_t BQ;
    uint64_t BK;
    uint64_t EP;
    int halfMoveClock;
    int moveNumber;
    bool whiteToMove;
    bool CWK;
    bool CWQ;
    bool CBK;
    bool CBQ;
    Phase phase;

public:
    Board(uint64_t WP, uint64_t WN, uint64_t WB, uint64_t WR, uint64_t WQ, uint64_t WK, uint64_t BP, uint64_t BN, uint64_t BB, uint64_t BR, uint64_t BQ, uint64_t BK, uint64_t EP, int halfMoveClock, int moveNumber, bool whiteToMove, bool CWK, bool CWQ, bool CBK, bool CBQ) : WP(WP), WN(WN), WB(WB), WR(WR), WQ(WQ), WK(WK), BP(BP), BN(BN), BB(BB), BR(BR), BQ(BQ), BK(BK), EP(EP), halfMoveClock(halfMoveClock), moveNumber(moveNumber), whiteToMove(whiteToMove), CWK(CWK), CWQ(CWQ), CBK(CBK), CBQ(CBQ)
    {
        phase = calculatePhase();
    };

    static Board initiateStandardChess();
    static Board from(string FEN);

    uint64_t getWP();

    uint64_t getWN();

    uint64_t getWB();

    uint64_t getWR();

    uint64_t getWQ();

    uint64_t getWK();

    uint64_t getBP();

    uint64_t getBN();

    uint64_t getBB();

    uint64_t getBR();

    uint64_t getBQ();

    uint64_t getBK();

    uint64_t getEP();

    int getHalfMoveClock();

    int getMoveNumber();

    bool getWhiteToMove();

    bool getCWK();

    bool getCWQ();

    bool getCBK();

    bool getCBQ();

    Phase getPhase();

    uint64_t whitePieces();

    uint64_t blackPieces();

    uint64_t occupied();

    uint64_t empty();

    string boardString();

    string fen();

    Phase calculatePhase();
};

#endif // BOARD_H_INCLUDED
