#ifndef MOVES_H_INCLUDED
#define MOVES_H_INCLUDED

#include "boardGeneration.h"
#include "bitboard.h"
#include <stdint.h>
#include <iostream>
#include <string>
using namespace std;

enum Direction
{
    North,
    Northeast,
    East,
    Southeast,
    South,
    Southwest,
    West,
    Northwest
};

class Moves
{
    static Moves *instance;
    Moves() {}

public:
    static Moves *getInstance()
    {
        if (!instance)
            instance = new Moves;
        return instance;
    }

    BoardGeneration &boardGeneration = BoardGeneration::getInstance();

    uint64_t notMyPieces;
    uint64_t myPieces;
    uint64_t emptySquares;
    uint64_t occupiedSquares;

    uint64_t horizontalAndVerticalMoves(int s)
    {
        uint64_t rankMask = rankMasks8[s / 8];
        uint64_t fileMask = rankMasks8[s % 8];
        uint64_t pseudoPossibleMoves = rankMask ^ fileMask;
        uint64_t unblockedRanks = 0;
        uint64_t unblockedFiles = 0;
        Direction direction = North;

        int testingSquare = s - 8;
        while (direction == North)
        {
            if (testingSquare < 0 || testingSquare % 8 != s % 8)
            {
                direction = East;
            }
            else
            {
                if (1ULL << testingSquare & (occupiedSquares != 0))
                {
                    unblockedRanks += rankMasks8[testingSquare / 8];
                    direction = East;
                }
                else
                {
                    unblockedRanks += rankMasks8[testingSquare / 8];
                    testingSquare -= 8;
                }
            }
        }

        testingSquare = s + 1;
        while (direction == East)
        {
            if (testingSquare > 63 || testingSquare / 8 != s / 8)
            {
                direction = South;
            }
            else
            {
                if (1ULL << testingSquare & (occupiedSquares != 0))
                {
                    unblockedFiles += fileMasks8[testingSquare % 8];
                    direction = South;
                }
                else
                {
                    unblockedFiles += fileMasks8[testingSquare % 8];
                    testingSquare++;
                }
            }
        }

        testingSquare = s + 8;
        while (direction == South)
        {
            if (testingSquare > 63 || testingSquare % 8 != s % 8)
            {
                direction = West;
            }
            else
            {
                if (1ULL << testingSquare & (occupiedSquares != 0))
                {
                    unblockedRanks += rankMasks8[testingSquare / 8];
                    direction = West;
                }
                else
                {
                    unblockedRanks += rankMasks8[testingSquare / 8];
                    testingSquare += 8;
                }
            }
        }

        testingSquare = s - 1;
        while (direction == West)
        {
            if (testingSquare < 0 || testingSquare / 8 != s / 8)
            {
                direction = North;
            }
            else
            {
                if (1ULL << testingSquare & (occupiedSquares != 0))
                {
                    unblockedFiles += fileMasks8[testingSquare % 8];
                    direction = North;
                }
                else
                {
                    unblockedFiles += fileMasks8[testingSquare % 8];
                    testingSquare--;
                }
            }
        }

        uint64_t mask = unblockedRanks | unblockedFiles;
        uint64_t possibleMoves = pseudoPossibleMoves & mask;

        return possibleMoves;
    }

    uint64_t diagonalAndAntiDiagonalMoves(int s)
    {
        uint64_t diagonalMask = diagonalMasks8[s % 8 + s / 8];
        uint64_t antiDiagonalMask = antiDiagonalMasks8[(7 - s % 8) + s / 8];
        uint64_t pseudoPossibleMoves = diagonalMask ^ antiDiagonalMask;
        uint64_t unblockedDiagonals = 0;
        uint64_t unblockedAntiDiagonals = 0;
        Direction direction = Northeast;

        int testingSquare = s - 7;
        while (direction == Northeast)
        {
            if (testingSquare < 0 || testingSquare > 63 || testingSquare / 8 == s / 8)
            {
                direction = Southeast;
            }
            else
            {
                if (1ULL << testingSquare & (occupiedSquares != 0))
                {
                    unblockedAntiDiagonals += antiDiagonalMasks8[(7 - testingSquare % 8) + testingSquare / 8];
                    direction = Southeast;
                }
                else
                {
                    unblockedAntiDiagonals += antiDiagonalMasks8[(7 - testingSquare % 8) + testingSquare / 8];
                    testingSquare -= 7;
                }
            }
        }

        testingSquare = s + 9;
        while (direction == Southeast)
        {
            if (testingSquare > 63 || testingSquare / 8 == s / 8)
            {
                direction = Southwest;
            }
            else
            {
                if (1ULL << testingSquare & (occupiedSquares != 0))
                {
                    unblockedDiagonals += diagonalMasks8[testingSquare % 8 + testingSquare / 8];
                    direction = Southwest;
                }
                else
                {
                    unblockedDiagonals += diagonalMasks8[testingSquare % 8 + testingSquare / 8];
                    testingSquare += 9;
                }
            }
        }

        testingSquare = s + 7;
        while (direction == Southwest)
        {
            if (testingSquare<0 | testingSquare> 63 || testingSquare % 8 == s % 8)
            {
                direction = Northwest;
            }
            else
            {
                if (1ULL << testingSquare & (occupiedSquares != 0))
                {
                    unblockedAntiDiagonals += antiDiagonalMasks8[(7 - testingSquare % 8) + testingSquare / 8];
                    direction = Northwest;
                }
                else
                {
                    unblockedAntiDiagonals += antiDiagonalMasks8[(7 - testingSquare % 8) + testingSquare / 8];
                    testingSquare += 7;
                }
            }
        }

        testingSquare = s - 9;
        while (direction == Northwest)
        {
            if (testingSquare < 0 || testingSquare / 8 == s / 8)
            {
                direction = Northeast;
            }
            else
            {
                if (1ULL << testingSquare & (occupiedSquares != 0))
                {
                    unblockedDiagonals += diagonalMasks8[testingSquare % 8 + testingSquare / 8];
                    direction = Northeast;
                }
                else
                {
                    unblockedDiagonals += diagonalMasks8[testingSquare % 8 + testingSquare / 8];
                    testingSquare -= 9;
                }
            }
        }

        uint64_t mask = unblockedDiagonals | unblockedAntiDiagonals;
        uint64_t possibleMoves = pseudoPossibleMoves & mask;

        return possibleMoves;
    }

    string possibleWP(uint64_t WP, uint64_t BP, uint64_t EP)
    {
        string moveList = "";
        uint64_t possibility;

        //first, look at right-captures
        uint64_t opponentPieces = occupiedSquares & ~myPieces;
        uint64_t pawnMoves = (WP >> 7) & opponentPieces & ~rank8 & ~fileA;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    moveList += (to_string(i / 8 + 1) + to_string(i % 8 - 1) + to_string(i / 8) + to_string(i % 8));
                }
            }
        }

        //now, left-captures
        pawnMoves = (WP >> 9) & opponentPieces & ~rank8 & ~fileH;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    moveList += (to_string(i / 8 + 1) + to_string(i % 8 + 1) + to_string(i / 8) + to_string(i % 8));
                }
            }
        }

        //now, moving 1 forward
        pawnMoves = (WP >> 8) & emptySquares & ~rank8;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    moveList += (to_string(i / 8 + 1) + to_string(i % 8) + to_string(i / 8) + to_string(i % 8));
                }
            }
        }

        //now, moving 2 forward
        pawnMoves = (WP >> 16) & emptySquares & (emptySquares >> 8) & rank4;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    moveList += (to_string(i / 8 + 2) + to_string(i % 8) + to_string(i / 8) + to_string(i % 8));
                }
            }
        }

        //now, promoting, capturing right
        pawnMoves = (WP >> 7) & occupiedSquares & opponentPieces & rank8 & ~fileA;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    moveList += (to_string(i / 8 - 1) + to_string(i % 8) + "QP" + to_string(i / 8 - 1) + to_string(i % 8) + "RP" + to_string(i / 8 - 1) + to_string(i % 8) + "BP" + to_string(i / 8 - 1) + to_string(i % 8) + "NP");
                }
            }
        }

        //now, promoting, capturing left
        pawnMoves = (WP >> 9) & occupiedSquares & opponentPieces & rank8 & ~fileH;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    moveList += (to_string(i / 8 + 1) + to_string(i % 8) + "QP" + to_string(i / 8 + 1) + to_string(i % 8) + "RP" + to_string(i / 8 + 1) + to_string(i % 8) + "BP" + to_string(i / 8 + 1) + to_string(i % 8) + "NP");
                }
            }
        }

        //now, promoting, moving 1 forward
        pawnMoves = (WP >> 8) & emptySquares & rank8;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    moveList += (to_string(i / 8) + to_string(i % 8) + "QP" + to_string(i / 8) + to_string(i % 8) + "RP" + to_string(i / 8) + to_string(i % 8) + "BP" + to_string(i / 8) + to_string(i % 8) + "NP");
                }
            }
        }

        possibility = (WP << 1) & BP & rank5 & ~fileA & EP;
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (to_string(index % 8 - 1) + to_string(index % 8) + "WE");
        }

        possibility = (WP >> 1) & BP & rank5 & ~fileH & EP;
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (to_string(index % 8 + 1) + to_string(index % 8) + "WE");
        }

        return moveList;
    }

    string possibleBP(uint64_t BP, uint64_t WP, uint64_t EP)
    {
        string moveList = "";
        uint64_t possibility;

        //first, let's look at right-captures
        uint64_t opponentPieces = occupiedSquares & ~myPieces;
        uint64_t pawnMoves = (BP << 7) & opponentPieces & ~rank1 & ~fileH;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    moveList += (to_string(i / 8 - 1) + to_string(i % 8 + 1) + to_string(i / 8) + to_string(i % 8));
                }
            }
        }

        //now, left-captures
        pawnMoves = (BP << 9) & opponentPieces & ~rank1 & ~fileA;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    moveList += (to_string(i / 8 - 1) + to_string(i % 8 - 1) + to_string(i / 8) + to_string(i % 8));
                }
            }
        }

        //now, moving 1 forward
        pawnMoves = (BP << 8) & emptySquares & ~rank1;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    moveList += (to_string(i / 8 - 1) + to_string(i % 8) + to_string(i / 8) + to_string(i % 8));
                }
            }
        }

        //now, moving 2 forward
        pawnMoves = (BP << 16) & emptySquares & (emptySquares << 8) & rank5;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    moveList += (to_string(i / 8 - 2) + to_string(i % 8) + to_string(i / 8) + to_string(i % 8));
                }
            }
        }

        //now, promoting, capturing right
        pawnMoves = (BP << 7) & occupiedSquares & opponentPieces & rank1 & ~fileH;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    moveList += (to_string(i / 8 + 1) + to_string(i % 8) + "QP" + to_string(i / 8 + 1) + to_string(i % 8) + "RP" + to_string(i / 8 + 1) + to_string(i % 8) + "BP" + to_string(i / 8 + 1) + to_string(i % 8) + "NP");
                }
            }
        }

        //now, promoting, capturing left
        pawnMoves = (BP << 9) & occupiedSquares & opponentPieces & rank1 & ~fileA;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    moveList += (to_string(i / 8 - 1) + to_string(i % 8) + "QP" + to_string(i / 8 - 1) + to_string(i % 8) + "RP" + to_string(i / 8 - 1) + to_string(i % 8) + "BP" + to_string(i / 8 - 1) + to_string(i % 8) + "NP");
                }
            }
        }

        //now, promoting, moving 1 forward
        pawnMoves = (BP << 8) & emptySquares & rank1;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    moveList += (to_string(i / 8) + to_string(i % 8) + "QP" + to_string(i / 8) + to_string(i % 8) + "RP" + to_string(i / 8) + to_string(i % 8) + "BP" + to_string(i / 8) + to_string(i % 8) + "NP");
                }
            }
        }

        possibility = (BP >> 1) & WP & rank4 & ~fileH & EP;
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (to_string(index % 8 + 1) + to_string(index % 8) + "WE");
        }

        possibility = (BP << 1) & WP & rank4 & ~fileA & EP;
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (to_string(index % 8 - 1) + to_string(index % 8) + "WE");
        }

        return moveList;
    }

    string possibleN(uint64_t occupiedSquares, uint64_t N)
    {
        uint64_t mutableN = N;
        string movesList = "";
        uint64_t i = mutableN & ~(mutableN - 1);
        uint64_t possibility;

        while (i != 0)
        {
            int iLocation = countTrailingZeros(i);
            if (iLocation > 18)
            {
                possibility = knightSpan << (iLocation - 18);
            }
            else
            {
                possibility = knightSpan >> (18 - iLocation);
            }
            if (iLocation % 8 < 4)
            {
                possibility &= ~filesGH & notMyPieces;
            }
            else
            {
                possibility &= ~filesAB & notMyPieces;
            }
            uint64_t j = possibility & ~(possibility & -1);
            while (j != 0)
            {
                int index = countTrailingZeros(j);
                movesList += (to_string(iLocation / 8) + to_string(iLocation % 8) + to_string(index / 8) + to_string(index % 8));
                possibility &= ~j;
                j = possibility & ~(possibility & -1);
            }
            mutableN &= ~i;
            i = mutableN & ~(mutableN & -1);
        }
        return movesList;
    }

    string possibleB(uint64_t occupiedSquares, uint64_t B)
    {
        uint64_t mutableB = B;
        string movesList = "";
        uint64_t i = mutableB & ~(mutableB - 1);
        uint64_t possibility;
        while (i != 0)
        {
            int iLocation = countTrailingZeros(i);
            possibility = diagonalAndAntiDiagonalMoves(iLocation) & notMyPieces;
            uint64_t j = possibility & ~(possibility & -1);
            while (j != 0)
            {
                int index = countTrailingZeros(j);
                movesList += (to_string(iLocation / 8) + to_string(iLocation % 8) + to_string(index / 8) + to_string(index % 8));
                possibility &= ~j;
                j = possibility & ~(possibility & -1);
            }
            mutableB &= ~i;
            i = mutableB & ~(mutableB & -1);
        }
        return movesList;
    }

    string possibleR(uint64_t occupied, uint64_t R)
    {
        uint64_t mutableR = R;
        string movesList = "";
        uint64_t i = mutableR & ~(mutableR - 1);
        uint64_t possibility;
        while (i != 0)
        {
            int iLocation = countTrailingZeros(i);
            possibility = horizontalAndVerticalMoves(iLocation) & notMyPieces;
            uint64_t j = possibility & ~(possibility & -1);
            while (j != 0)
            {
                int index = countTrailingZeros(j);
                movesList += (to_string(iLocation / 8) + to_string(iLocation % 8) + to_string(index / 8) + to_string(index % 8));
                possibility &= ~j;
                j = possibility & ~(possibility & -1);
            }
            mutableR &= ~i;
            i = mutableR & ~(mutableR & -1);
        }
        return movesList;
    }

    string possibleQ(uint64_t occupied, uint64_t Q)
    {
        uint64_t mutableQ = Q;
        string movesList = "";
        uint64_t i = mutableQ & ~(mutableQ & -1);
        uint64_t possibility;
        while (i != 0)
        {
            int iLocation = countTrailingZeros(i);
            possibility = (horizontalAndVerticalMoves(iLocation) | diagonalAndAntiDiagonalMoves(iLocation)) & notMyPieces;
            uint64_t j = possibility & ~(possibility & -1);
            while (j != 0)
            {
                int index = countTrailingZeros(j);
                movesList += (to_string(iLocation / 8) + to_string(iLocation % 8) + to_string(index / 8) + to_string(index % 8));
                possibility &= ~j;
                j = possibility & ~(possibility & -1);
            }
            mutableQ &= ~i;
            i = mutableQ & ~(mutableQ & -1);
        }
        return movesList;
    }

    string possibleK(uint64_t occupied, uint64_t K)
    {
        string movesList = "";
        uint64_t possibility;
        int iLocation = countTrailingZeros(K);
        if (iLocation > 9)
        {
            possibility = kingSpan << (iLocation - 9);
        }
        else
        {
            possibility = kingSpan >> (9 - iLocation);
        }
        if (iLocation % 8 < 4)
        {
            possibility &= ~filesGH & notMyPieces;
        }
        else
        {
            possibility &= ~filesAB & notMyPieces;
        }

        if (boardGeneration.whiteToMove)
        {
            uint64_t safeSquares = ~unsafeForWhite(boardGeneration.WP, boardGeneration.WN, boardGeneration.WB, boardGeneration.WR, boardGeneration.WQ, boardGeneration.WK, boardGeneration.BP, boardGeneration.BN, boardGeneration.BB, boardGeneration.BR, boardGeneration.BQ, boardGeneration.BK);
            uint64_t nonWhiteOccupiedSquares = ~(boardGeneration.BP | boardGeneration.BN | boardGeneration.BB | boardGeneration.BR | boardGeneration.BQ);
            possibility = possibility & safeSquares & nonWhiteOccupiedSquares;
        }
        else
        {
            uint64_t safeSquares = ~unsafeForBlack(boardGeneration.WP, boardGeneration.WN, boardGeneration.WB, boardGeneration.WR, boardGeneration.WQ, boardGeneration.WK, boardGeneration.BP, boardGeneration.BN, boardGeneration.BB, boardGeneration.BR, boardGeneration.BQ, boardGeneration.BK);
            uint64_t nonBlackOccupiedSquares = ~(boardGeneration.BP | boardGeneration.BN | boardGeneration.BB | boardGeneration.BR | boardGeneration.BQ);
            possibility = possibility & safeSquares & nonBlackOccupiedSquares;
        }

        uint64_t j = possibility & ~(possibility & -1);

        while (j != 0)
        {
            int index = countTrailingZeros(j);
            movesList += (to_string(iLocation / 8) + to_string(iLocation % 8) + to_string(index / 8) + to_string(index % 8));
            possibility &= ~j;
            j = possibility & ~(possibility & -1);
        }

        return movesList;
    }

    string possibleCW(uint64_t WP, uint64_t WN, uint64_t WB, uint64_t WR, uint64_t WQ, uint64_t WK, uint64_t BP, uint64_t BN, uint64_t BB, uint64_t BR, uint64_t BQ, uint64_t BK, bool CWK, bool CWQ)
    {
        string movesList = "";
        uint64_t unsafe = unsafeForWhite(boardGeneration.WP, boardGeneration.WN, boardGeneration.WB, boardGeneration.WR, boardGeneration.WQ, boardGeneration.WK, boardGeneration.BP, boardGeneration.BN, boardGeneration.BB, boardGeneration.BR, boardGeneration.BQ, boardGeneration.BK);

        bool inCheck = unsafe & (WK == 0);
        if (inCheck)
        {
            if (CWK && (((1ULL << castleRooks[0]) & WR) != 0))
            {
                uint64_t occupiedAndUnsafeSquares = (occupiedSquares | unsafe);
                uint64_t kingSideBishopAndKnight = ((1ULL << 61) | (1ULL << 62));
                bool shortCastleLegal = (occupiedAndUnsafeSquares & kingSideBishopAndKnight) == 0;
                if (shortCastleLegal)
                {
                    movesList += "7476";
                }
            }

            if (CWQ && (((1ULL << castleRooks[1]) & WR) != 0))
            {
                uint64_t knight = (1ULL << 57);
                uint64_t bishop = (1ULL << 58);
                uint64_t queen = (1ULL << 59);
                bool longCastleLegal = ((occupiedSquares | (unsafe & ~knight)) & (knight | bishop | queen)) == 0;
                if (longCastleLegal)
                {
                    movesList += "7472";
                }
            }
        }

        return movesList;
    }

    string possibleCB(uint64_t WP, uint64_t WN, uint64_t WB, uint64_t WR, uint64_t WQ, uint64_t WK, uint64_t BP, uint64_t BN, uint64_t BB, uint64_t BR, uint64_t BQ, uint64_t BK, bool CBK, bool CBQ)
    {
        string movesList = "";
        uint64_t unsafe = unsafeForBlack(boardGeneration.WP, boardGeneration.WN, boardGeneration.WB, boardGeneration.WR, boardGeneration.WQ, boardGeneration.WK, boardGeneration.BP, boardGeneration.BN, boardGeneration.BB, boardGeneration.BR, boardGeneration.BQ, boardGeneration.BK);

        bool inCheck = unsafe & (BK == 0);
        if (inCheck)
        {
            if (CBK && (((1ULL << castleRooks[2]) & BR) != 0))
            {
                uint64_t occupiedAndUnsafeSquares = (occupiedSquares | unsafe);
                uint64_t kingSideBishopAndKnight = ((1ULL << 5) | (1ULL << 6));
                bool shortCastleLegal = (occupiedAndUnsafeSquares & kingSideBishopAndKnight) == 0;
                if (shortCastleLegal)
                {
                    movesList += "0406";
                }
            }
            if (CBQ && (((1ULL << castleRooks[3]) & BR) != 0))
            {
                uint16_t knight = (1ULL << 1);
                uint64_t bishop = (1ULL << 2);
                uint64_t queen = (1ULL << 3);
                bool longCastleLegal = ((occupiedSquares | (unsafe & ~knight)) & (knight | bishop | queen)) == 0;
                if (longCastleLegal)
                {
                    movesList += "0402";
                }
            }
        }
        return movesList;
    }

    uint64_t unsafeForWhite(uint64_t WP, uint64_t WN, uint64_t WB, uint64_t WR, uint64_t WQ, uint64_t WK, uint64_t BP, uint64_t BN, uint64_t BB, uint64_t BR, uint64_t BQ, uint64_t BK)
    {
        uint64_t mutableBN = BN;
        uint64_t unsafe;
        occupiedSquares = WP | WN | WB | WR | WQ | WK | BP | BN | BB | BR | BQ | BK;

        //pawn
        unsafe = (BP << 7) & ~fileH;  //pawn capture right
        unsafe |= (BP << 9) & ~fileA; //pawn capture left

        uint64_t possibility;

        //knight
        uint64_t i;
        if (mutableBN != 0)
        {
            i = mutableBN & ~(mutableBN - 1);
            while (i != 0)
            {
                int iLocation = countTrailingZeros(i);
                if (iLocation > 18)
                {
                    possibility = knightSpan << (iLocation - 18);
                }
                else
                {
                    possibility = knightSpan >> (18 - iLocation);
                }

                if (iLocation % 8 < 4)
                {
                    possibility &= ~filesGH;
                }
                else
                {
                    possibility &= ~filesAB;
                }

                unsafe |= possibility;
                mutableBN &= ~i;
                if (mutableBN != 0)
                {
                    i = mutableBN & ~(mutableBN - 1);
                }
                else
                {
                    i = 0;
                }
            }
        }

        //bishop/queen
        uint64_t QB = BQ | BB;
        if (QB != 0)
        {
            i = QB & ~(QB - 1);
            while (i != 0)
            {
                int iLocation = countTrailingZeros(i);
                possibility = diagonalAndAntiDiagonalMoves(iLocation);
                unsafe |= possibility;
                QB &= ~i;
                if (QB != 0)
                {
                    i = QB & ~(QB - 1);
                }
                else
                {
                    i = 0;
                }
            }
        }

        //rook/queen
        uint64_t QR = BQ | BR;
        if (QR != 0)
        {
            while (i != 0)
            {
                int iLocation = countTrailingZeros(i);
                possibility = horizontalAndVerticalMoves(iLocation);
                unsafe |= possibility;
                QR &= ~i;
                if (QR != 0)
                {
                    i = QR & ~(QR - 1);
                }
                else
                {
                    i = 0;
                }
            }
        }

        //king
        int iLocation = countTrailingZeros(BK);
        if (iLocation > 9)
        {
            possibility = kingSpan << (iLocation - 9);
        }
        else
        {
            possibility = kingSpan >> (9 - iLocation);
        }
        if (iLocation % 8 < 4)
        {
            possibility &= ~filesGH;
        }
        else
        {
            possibility &= ~filesAB;
        }
        unsafe |= possibility;

        return unsafe;
    }

    uint64_t unsafeForBlack(uint64_t WP, uint64_t WN, uint64_t WB, uint64_t WR, uint64_t WQ, uint64_t WK, uint64_t BP, uint64_t BN, uint64_t BB, uint64_t BR, uint64_t BQ, uint64_t BK)
    {
        uint64_t mutableWN = WN;
        uint64_t unsafe;
        occupiedSquares = WP | WN | WB | WR | WQ | WK | BP | BN | BB | BR | BQ | BK;

        //pawn
        unsafe = (WP >> 7) & ~fileA;  //pawn capture right
        unsafe |= (WP >> 9) & ~fileH; //pawn capture left

        uint64_t possibility;

        //knight
        uint64_t i;
        if (mutableWN != 0)
        {
            i = mutableWN & ~(mutableWN - 1);
            while (i != 0)
            {
                int iLocation = countTrailingZeros(i);
                if (iLocation > 18)
                {
                    possibility = knightSpan << (iLocation - 18);
                }
                else
                {
                    possibility = knightSpan >> (18 - iLocation);
                }

                if (iLocation % 8 < 4)
                {
                    possibility &= ~filesGH;
                }
                else
                {
                    possibility &= ~filesAB;
                }

                unsafe |= possibility;
                mutableWN &= ~i;
                if (mutableWN != 0)
                {
                    i = mutableWN & ~(mutableWN - 1);
                }
                else
                {
                    i = 0;
                }
            }
        }

        //bishop/queen
        uint64_t QB = WQ | WB;

        if (QB != 0)
        {
            i = QB & ~(QB - 1);
            while (i != 0)
            {
                int iLocation = countTrailingZeros(i);
                possibility = diagonalAndAntiDiagonalMoves(iLocation);
                unsafe |= possibility;
                QB &= ~i;
                if (QB != 0)
                {
                    i = QB & ~(QB - 1);
                }
                else
                {
                    i = 0;
                }
            }
        }

        //rook/queen
        uint64_t QR = WQ | WR;
        if (QR != 0)
        {
            i = QR & ~(QR - 1);
            while (i != 0)
            {
                int iLocation = countTrailingZeros(i);
                possibility = horizontalAndVerticalMoves(iLocation);
                unsafe |= possibility;
                QR &= ~i;
                if (QR != 0)
                {
                    i = QR & ~(QR - 1);
                }
                else
                {
                    i = 0;
                }
            }
        }

        //king
        int iLocation = countTrailingZeros(WK);
        if (iLocation > 9)
        {
            possibility = kingSpan << (iLocation - 9);
        }
        else
        {
            possibility = kingSpan >> (9 - iLocation);
        }
        if (iLocation % 8 < 4)
        {
            possibility &= ~filesGH;
        }
        else
        {
            possibility &= ~filesAB;
        }
        unsafe |= possibility;

        return unsafe;
    }

    int countLeadingZeros(uint64_t x)
    {
        // Keep shifting x by one until leftmost bit
        // does not become 1.
        int total_bits = sizeof(x) * 8;
        int res = 0;
        while (!(x & (1ULL << (total_bits - 1))))
        {
            x = (x << 1);
            res++;
        }

        return res;
    }

    int countTrailingZeros(uint64_t x)
    {
        int count = 0;
        while ((x & 1) == 0)
        {
            x = x >> 1;
            count++;
        }
        return count;
    }
};

#endif // MOVES_H_INCLUDED
