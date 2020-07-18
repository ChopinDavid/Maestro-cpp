#ifndef MOVES_H_INCLUDED
#define MOVES_H_INCLUDED

#include "boardGeneration.h"

class Moves
{
private:
    Moves() {}

public:
    static Moves& getInstance()
    {
        static Moves shared;
        return shared;
    }

    BoardGeneration& boardGeneration = BoardGeneration::getInstance();

    uint64_t fileA = 72340172838076673;
    uint64_t fileH = 9259542123273814144;
    uint64_t filesAB = 217020518514230019;
    uint64_t filesGH = 13889313184910721216;
    uint64_t rank1 = 18374686479671623680;
    uint64_t rank4 = 1095216660480;
    uint64_t rank5 = 4278190080;
    uint64_t rank8 = 255;
    uint64_t center = 103481868288;
    uint64_t extendedCenter = 66229406269440;
    uint64_t kingSide = 17361641481138401520;
    uint64_t kingSpan = 460039;
    uint64_t knightSpan = 43234889994;
    uint64_t notMyPieces;
    uint64_t myPieces;
    uint64_t emptySquares;
    uint64_t occupiedSquares;

    uint64_t castleRooks[4] = {63, 56, 7, 0};
    uint64_t rankMasks8[8] = { 255, 65280, 16711680, 4278190080, 1095216660480, 280375465082880, 71776119061217280, 18374686479671623680 };
    uint64_t fileMasks8[8] = { 72340172838076673, 144680345676153346, 289360691352306692, 578721382704613384, 1157442765409226768, 2314885530818453536, 4629771061636907072, 9259542123273814144 };
    uint64_t diagonalMasks8[15] = { 1, 258, 66052, 16909320, 4328785936, 1108169199648, 283691315109952, 72624976668147840, 145249953336295424, 290499906672525312, 580999813328273408, 1161999622361579520, 2323998145211531264, 4647714815446351872, 9223372036854775808 };
    uint64_t antiDiagonalMasks8[15] = { 128, 32832, 8405024, 2151686160, 550831656968, 141012904183812, 36099303471055874, 9241421688590303745, 4620710844295151872, 2310355422147575808, 1155177711073755136, 577588855528488960, 288794425616760832, 144396663052566528, 72057594037927936 };

    uint64_t horizontalAndVerticalMoves(int s)
    {
        uint64_t rankMask = rankMasks8[s/8];
        uint64_t fileMask = rankMasks8[s%8];
        uint64_t pseudoPossibleMoves = rankMask ^ fileMask;
        uint64_t unblockedRanks = 0;
        uint64_t unblockedFiles = 0;
        Direction direction = Direction.North;

        int testingSquare = s - 8;
        while (direction == Direction.North)
        {
            if (testingSquare < 0 || testingSquare % 8 != s % 8)
            {
                direction = Direction.East;
            }
            else
            {
                if (1<<testingSquare&occupiedSquares != 0)
                {
                    unblockedRanks += rankMasks8[testingSquare / 8];
                    direction = Direction.East
                }
                else
                {
                    unblockedRanks += rankMasks8[testingSquare / 8];
                    testingSquare -= 8;
                }
            }
        }

        testingSquare = s + 1;
        while (direction == Direction.East)
        {
            if (testingSquare > 63 || testingSquare / 8 != s / 8)
            {
                direction = Direction.South;
            }
            else
            {
                if (1<<testingSquare&occupiedSquares != 0)
                {
                    unblockedFiles += fileMasks8[testingSquare%8];
                    direction = Direction.South;
                }
                else
                {
                    unblockedFiles += fileMasks8[testingSquare%8];
                    testingSquare++;
                }
            }
        }

        testingSquare = s + 8;
        while (direction == Direction.South)
        {
            if (testingSquare > 63 || testingSquare%8 != s%8)
            {
                direction = Direction.West;
            }
            else
            {
                if (1<<testingSquare&occupiedSquares != 0)
                {
                    unblockedRanks += rankMasks8[testingSquare/8];
                    direction = Direction.West;
                }
                else
                {
                    unblockedRanks += rankMAsks8[testingSquare/8];
                    testingSquare += 8;
                }
            }
        }

        testingSquare = s - 1;
        while (direction == Direction.West)
        {
            if (testingSquare < 0 || testingSquare/8 != s/8)
            {
                direction = Direction.North;
            }
            else
            {
                if (1<<testingSquare&occupiedSquares != 0)
                {
                    unblockedFiles += fileMasks8[testingSquare%8];
                    direction = Direction.North;
                }
                else
                {
                    unblockedFiles += fileMasks8[testingSquare%8];
                    testingSquare--;
                }
            }
        }

        uint64_t mask = unblockedRanks | unblockedFiles;
        uint64_t possibleMoves = pseudoPossibleMoves&mask;

        return possibleMoves;
    }

    uint64_t diagonalAndAntiDiagonalMoves(int s)
    {
        uint64_t diagonalMask = diagonalMasks8[s%8 + s/8];
        uint64_t antiDiagonalMask = antiDiagonalMasks8[(7 - s%8) + s/8];
        uint64_t pseudoPossibleMoves = diagonalMask ^ antiDiagonalMask;
        uint64_t unblockedDiagonals = 0;
        uint64_t unblockedAntiDiagonals = 0;
        Direction direction = Direction.Northeast;

        int testingSquare = s - 7;
        while (direction == Direction.Northeast)
        {
            if (testingSquare < 0 || testingSquare > 63 || testingSquare/8 == s/8)
            {
                direction = Direction.Southeast;
            }
            else
            {
                if (1<<testingSquare&occupiedSquares != 0 )
                {
                    unblockedAntiDiagonals += antiDiagonalMasks8[(7 - testingSquare%8) + testingSquare/8];
                    direction = Direction.Southeast;
                }
                else
                {
                    unblockedAntiDiagonals += antiDiagonalMasks8[(7 - testingSquare%8) + testingSquare/8];
                    testingSquare -= 7;
                }
            }
        }

        testingSquare = s + 9;
        while (direction == Direction.Southeast)
        {
            if (testingSquare > 63 || testingSquare/8 == s/8)
            {
                direction = Direction.Southwest;
            }
            else
            {
                if (1<<testingSquare&occupiedSquares != 0)
                {
                    unblockedDiagonals += diagonalMasks8[testingSquare%8 + testingSquare/8];
                    direction = Direction.Southwest
                }
                else
                {
                    unblockedDiagonals += diagonalMasks8[testingSquare%8 + testingSquare/8];
                    testingSquare += 9
                }
            }
        }

        testingSquare = s + 7;
        while (direction == Direction.Southwest)
        {
            if (testingSquare < 0 | testingSquare > 63 || testingSquare%8 == s%8)
            {
                direction = Direction.Northwest;
            }
            else
            {
                if (1<<testingSquare&occupiedSquares != 0)
                {
                    unblockedAntiDiagonals += antiDiagonalMasks8[(7 - testingSquare%8) + testingSquare/8];
                    direction = Direction.Northwest;
                }
                else
                {
                    unblockedAntiDiagonals += antiDiagonalMasks8[(7 - testingSquare%8) + testingSquare/8];
                    testingSquare += 7;
                }
            }
        }

        testingSquare = s - 9;
        while (direction == Direction.Northwest)
        {
            if (testingSquare < 0 || testingSquare/8 == s/8)
            {
                direction = Direction.Northeast;
            }
            else
            {
                if (1<<testingSquare&occupiedSquares != 0)
                {
                    unblockedDiagonals += diagonalMasks8[testingSquare%8 + testingSquare/8];
                    direction = Direction.Northeast;
                }
                else
                {
                    unblockedDiagonals += diagonalMasks8[testingSquare%8 + testingSquare/8];
                    testingSquare -= 9;
                }
            }
        }

        uint64_t mask = unblockedDiagonals | unblockedAntiDiagonals;
        uint64_t possibleMoves = pseudoPossibleMoves&mask;

        return possibleMoves;
    }

    string possibleWP(uint64_t WP, uint64_t BP, uint64_t EP)
    {
        string moveList = "";
        uint64_t possibility;

        //first, look at right-captures
        uint64_t opponentPieces = occupiedSquares & ~myPieces;
        uint64_t pawnMoves = (CP>>7) & opponentPieces & ~rank8 & ~fileA;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves>>i)&1) == 1)
                {
                    moveList += (std::to_string(i/8+1) + std::to_string(i%8-1) + std::to_string(i/8) + std::to_string(i%8));
                }
            }
        }

        //now, left-captures
        pawnMoves = (WP>>9) & opponentPieces & ~rank8 & ~fileH;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves>>i)&1) == 1)
                {
                    moveList += (std::to_string(i/8+1) + std::to_string(i%8+1) + std::to_string(i/8) + std::to_string(i%8));
                }
            }
        }

        //now, moving 1 forward
        pawnMoves = (WP>>8) & empty & ~rank8;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves>>i)&1) == 1)
                {
                    moveList += (std::to_string(i/8+1) + std::to_string(i%8) + std::to_string(i/8) + std::to_string(i%8));
                }
            }
        }

        //now, moving 2 forward
        pawnMoves = (WP>>16) & empty & (empty>>8) & rank4;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves>>i)&1) == 1)
                {
                    moveList += (std::to_string(i/8+2) + std::to_string(i%8) + std::to_string(i/8) + std::to_string(i%8));
                }
            }
        }

        //now, promoting, capturing right
        pawnMoves = (WP>>7) & occupied & opponentPieces & rank8 & ~fileA;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves>>i)&1) == 1)
                {
                    moveList += (std::to_string(i/8-1) + std::to_string(i%8) + "QP" + std::to_string(i/8-1) + std::to_string(i%8) + "RP" + std::to_string(i/8-1) + std::to_string(i%8) + "BP" + std::to_string(i/8-1) + std::to_string(i%8) + "NP");
                }
            }
        }

        //now, promoting, capturing left
        pawnMoves = (WP>>9) & occupied & opponentPieces & rank8 & ~fileH;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves>>i)&1) == 1)
                {
                    moveList += (std::to_string(i/8+1) + std::to_string(i%8) + "QP" + std::to_string(i/8+1) + std::to_string(i%8) + "RP" + std::to_string(i/8+1) + std::to_string(i%8) + "BP" + std::to_string(i/8+1) + std::to_string(i%8) + "NP");
                }
            }
        }

        //now, promoting, moving 1 forward
        pawnMoves = (WP>>8) & empty & rank8;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves>>i)&1) == 1)
                {
                    moveList += (std::to_string(i/8) + std::to_string(i%8) + "QP" + std::to_string(i/8) + std::to_string(i%8) + "RP" + std::to_string(i/8) + std::to_string(i%8) + "BP" + std::to_string(i/8) + std::to_string(i%8) + "NP");
                }
            }
        }

        possibility = (WP<<1) & BP & rank5 & ~fileA & EP;
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (std::to_string(index%8-1) + std::to_string(index%8) + "WE");
        }

        possibility = (WP>>1) & BP & rank5 & ~fileH & EP;
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (std::to_string(index%8+1) + std::to_string(index%8) + "WE");
        }

        return moveList;
    }

    string possibleBP(uint64_t BP, uint64_t WP, uint64_t EP)
    {
        string moveList = "";
        uint64_t possibility;

        //first, let's look at right-captures
        uint64_t opponentPieces = occupiedSquares & ~myPieces;
        uint64_t pawnMoves = (BP<<7) & opponentPieces & ~rank1 & ~fileH;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves>>i)&1) == 1)
                {
                    moveList += (std::to_string(i/8-1) + std::to_string(i%8+1) + std::to_string(i/8) + std::to_string(i%8));
                }
            }
        }

        //now, left-captures
        pawnMoves = (BP<<9) & opponentPieces & ~rank1 & ~fileA;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves>>i)&1) == 1)
                {
                    moveList += (std::to_string(i/8-1) + std::to_string(i%8-1) + std::to_string(i/8) + std::to_string(i%8));
                }
            }
        }

        //now, moving 1 forward
        pawnMoves = (BP<<8) & empty & ~rank1;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves>>i)&1) == 1)
                {
                    moveList += (std::to_string(i/8-1) + std::to_string(i%8) + std::to_string(i/8) + std::to_string(i%8));
                }
            }
        }

        //now, moving 2 forward
        pawnMoves = (BP<<16) & empty & (empty<<8) rank5;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves>>i)&1) == 1)
                {
                    moveList += (std::to_string(i/8-2) + std::to_string(i%8) + std::to_string(i/8) + std::to_string(i%8));
                }
            }
        }

        //now, promoting, capturing right
        pawnMoves = (BP<<7) & occupiedSquares & opponentPieces & rank1 & ~fileH;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves>>i)&1) == 1)
                {
                    moveList += (std::to_string(i/8+1) + std::to_string(i%8) + "QP" + std::to_string(i/8+1) + std::to_string(i%8) + "RP" + std::to_string(i/8+1) + std::to_string(i%8) + "BP" + std::to_string(i/8+1) + std::to_string(i%8) + "NP");
                }
            }
        }

        //now, promoting, capturing left
        pawnMoves = (BP<<9) & occupiedSquares & opponentPieces & rank1 & ~fileA;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves>>i)&1) == 1)
                {
                    moveList += (std::to_string(i/8-1) + std::to_string(i%8) + "QP" + std::to_string(i/8-1) + std::to_string(i%8) + "RP" + std::to_string(i/8-1) + std::to_string(i%8) + "BP" + std::to_string(i/8-1) + std::to_string(i%8) + "NP");
                }
            }
        }

        //now, promoting, moving 1 forward
        pawnMoves = (BP<<8) & emptySquares & rank1;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves>>i)&1) == 1)
                {
                    moveList += (std::to_string(i/8) + std::to_string(i%8) + "QP" + std::to_string(i/8) + std::to_string(i%8) + "RP" + std::to_string(i/8) + std::to_string(i%8) + "BP" + std::to_string(i/8) + std::to_string(i%8) + "NP");
                }
            }
        }

        possibility = (BP>>1) & WP & rank4 & ~fileH & EP;
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (std::to_string(index%8+1) + std::to_string(index%8) + "WE");
        }

        possibility = (BP<<1) & WP & rank4 & ~fileA & EP;
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (std::to_string(index%8-1) + std::to_string(index%8) + "WE");
        }

        return moveList;
    }

    string possibleN(uint64_t occupiedSquares, uint64_t N)
    {
        uint64_t N = N;
        string movesList = "";
        uint64_t i = N & ~(N-1);
        uint64_t possibility;

        while (i != 0)
        {
            int iLocation = countTrailingZeros(i);
            if (iLocation > 18)
            {
                possibility = knightSpan<<(iLocation-18);
            }
            else
            {
                possibility = knightSpan>>(18-iLocation);
            }
            if (iLocation%8 < 4)
            {
                possibility &= ~filesGH&notMyPieces;
            }
            else
            {
                possibility &= ~filesAB&notMyPieces;
            }
            uint64_t j = possibility & ~(possibility&-1);
            while (j != 0)
            {
                int index = countTrailingZeros(j);
                movesList += (std::to_string(iLocation/8) + std::to_string(iLocation%8) + std::to_string(index/8) + std::to_string(index%8));
                possibility &= ~j;
                j = possibility & ~(possibility&-1);
            }
            N &= ~i;
            i = N & ~(N&-1);
        }
        return movesList;
    }

    string possibleB(uint64_t occupiedSquares, uint64_t B)
    {
        uint64_t B = B;
        string movesList = "";
        uint64_t i = B & ~(B-1);
        uint64_t possibility;
        while (i != 0)
        {
            int iLocation = countTrailingZeros(i);
            possibility = diagonalAndAntiDiagonalMoves(iLocation)&notMyPieces;
            uint64_t j = possibility & ~(possibility&-1);
            while (j != 0)
            {
                int index = countTrailingZeros(j);
                movesList += (std::to_string(iLocation/8) + std::to_string(iLocation%8) + std::to_string(index/8) + std::to_string(index%8));
                possibility &= ~j;
                j = possibility & ~(possibility&-1);
            }
            B &= ~i;
            i = B & ~(B&-1);
        }
        return movesList;
    }

    string possibleR(uint64_t occupied, uint64_t R)
    {
        uint64_t R = R;
        string movesList = "";
        uint64_t i = R & ~(R-1);
        uint64_t possibility;
        while (i != 0)
        {
            int iLocation = countTrailingZeros(i);
            possibility = horizontalAndVerticalMoves(iLocation)&notMyPieces;
            uint64_t j = possibility & ~(possibility&-1);
            while (j != 0)
            {
                int index = countTrailingZeros(j);
                movesList += (std::to_string(iLocation/8) + std::to_string(iLocation%8) + std::to_string(index/8) + std::to_string(index%8));
                possibility &= ~j;
                j = possibility & ~(possibility&-1);
            }
            R &= ~i;
            i = R & ~(R&-1);
        }
        return movesList;
    }

    string possibleQ(uint64_t occupied, uint64_t Q)
    {
        uint64_t Q = Q;
        string movesList = "";
        uint64_t i = Q & ~(Q&-1);
        uint64_t possibility;
        while (i != 0)
        {
            int iLocation = countTrailingZeros(i);
            possibility = (horizontalAndVerticalMoves(iLocation) | diagonalAndAntiDiagonalMoves(iLocation)) & notMyPieces;
            uint64_t j = possibility & ~(possibility&-1);
            while (j != 0)
            {
                int index = countTrailingZeros(j);
                movesList += (std::to_string(iLocation/8) + std::to_string(iLocation%8) + std::to_string(index/8) + std::to_string(index%8));
                possibility &= ~j;
                j = possibility & ~(possibility&-1);
            }
            Q &= ~i;
            i = Q & ~(Q&-1);
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
            possibility = kingSpan<<(iLocation-9);
        }
        else
        {
            possibility = kingSpan>>(9-iLocation);
        }
        if (iLocation%8<4)
        {
            possibility &= ~filesGH&notMyPieces;
        }
        else
        {
            possibility &= ~filesAB&notMyPieces
        }

        if (boardGeneration.whiteToMove) {
            uint64_t safeSquares = ~unsafeForWhite(boardGeneration.WP, boardGeneration.WN, boardGeneration.WB, boardGeneration.WR, boardGeneration.WQ, boardGeneration.WK, boardGeneration.BP, boardGeneration.BN, boardGeneration.BB, boardGeneration.BR, boardGeneration.BQ, boardGeneration.BK);
            uint64_t nonWhiteOccupiedSquares = ~(boardGeneration.BP|boardGeneration.BN|boardGeneration.BB|boardGeneration.BR|boardGeneration.BQ);
            possibility = possibility & safeSquares & nonWhiteOccupiedSquares;
        } else {
            uint64_t safeSquares = ~unsafeForBlack(boardGeneration.WP, boardGeneration.WN, boardGeneration.WB, boardGeneration.WR, boardGeneration.WQ, boardGeneration.WK, boardGeneration.BP, boardGeneration.BN, boardGeneration.BB, boardGeneration.BR, boardGeneration.BQ, boardGeneration.BK);
            uint64_t nonBlackOccupiedSquares = ~(boardGeneration.BP|boardGeneration.BN|boardGeneration.BB|boardGeneration.BR|boardGeneration.BQ);
            possibility = possibility & safeSquares & nonBlackOccupiedSquares;
        }

        uint64_t j = possibility & ~(possibility&-1);

        while (j != 0) {
            int index = countTrailingZeros(j);
            movesList += (std::to_string(iLocation/8) + std::to_string(iLocation%8) + std::to_string(index/8) + std::to_string(index%8));
            possibility &= ~j;
            j = possibility & ~(possibility&-1);
        }

        return movesList;
    }

    string possibleCW(uint64_t WP, uint64_t WN, uint64_t WB, uint64_t WR, uint64_t WQ, uint64_t WK, uint64_t BP, uint64_t BN, uint64_t BB, uint64_t BR, uint64_t BQ, uint64_t BK, bool CWK, bool CWQ) {
        string movesList = "";
        uint64_t unsafe = unsafeForWhite(boardGeneration.WP, boardGeneration.WN, boardGeneration.WB, boardGeneration.WR, boardGeneration.WQ, boardGeneration.WK, boardGeneration.BP, boardGeneration.BN, boardGeneration.BB, boardGeneration.BR, boardGeneration.BQ, boardGeneration.BK);

        bool inCheck = unsafe&WK == 0;
        if (inCheck) {
            if (CWK && (((1<<castleRooks[0]) & WR) != 0)) {
                uint64_t occupiedAndUnsafeSquares = (occupiedSquares | unsafe);
                uint64_t kingSideBishopAndKnight = ((1ULL<<61) | (1ULL<<62));
                bool shortCastleLegal = (occupiedAndUnsafeSquares & kingSideBishopAndKnight) == 0;
                if (shortCastleLegal) {
                    movesList += "7476";
                }
            }

            if (CWQ && (((1ULL<<castleRooks[1]) & WR) != 0)) {
                uint64_t knight = (1ULL<<57);
                uint64_t bishop = (1ULL<<58);
                uint64_t queen = (1ULL<<59);
                bool longCastleLegal = ((occupiedSquares | (unsafe & ~knight)) & (knight | bishop | queen)) == 0;
                if (longCastleLegal) {
                    movesList += "7472";
                }
            }
        }

        return movesList;
    }

    string possibleCB(uint64_t WP, uint64_t WN, uint64_t WB, uint64_t WR, uint64_t WQ, uint64_t WK, uint64_t BP, uint64_t BN, uint64_t BB, uint64_t BR, uint64_t BQ, uint64_t BK, bool CBK, bool CBQ) {
        string movesList = "";
        uint64_t unsafe = unsafeForBlack(boardGeneration.WP, boardGeneration.WN, boardGeneration.WB, boardGeneration.WR, boardGeneration.WQ, boardGeneration.WK, boardGeneration.BP, boardGeneration.BN, boardGeneration.BB, boardGeneration.BR, boardGeneration.BQ, boardGeneration.BK);

        bool inCheck = unsafe&BK == 0;
        if (inCheck) {
            if (CBK && (((1ULL<<castleRooks[2]) & BR) != 0)) {
                uint64_t occupiedAndUnsafeSquares = (occupiedSquares | unsafe);
                uint64_t kingSideBishopAndKnight = ((1ULL<<5) | (1ULL<<6))
                bool shortCastleLegal = (occupiedAndUnsafeSquares & kingSideBishopAndKnight) == 0;
                if (shortCastleLegal) {
                    movesList += "0406";
                }
            }
            if (CBQ && (((1<<castleRooks[3]) & BR) != 0)) {
                uint16_t knight = (1ULL<<1);
                uint64_t bishop = (1ULL<<2);
                uint64_t queen = (1ULL<<3);
                bool longCastleLegal = ((occupiedSquares | (unsafe & ~knight) & (knight | bishop | queen)) == 0;
                if (longCastleLegal) {
                    movesList += "0402";
                }
            }
        }
        return movesList;
    }



    int countLeadingZeros(uint64_t x)
    {
        // Keep shifting x by one until leftmost bit
        // does not become 1.
        int total_bits = sizeof(x) * 8;
        int res = 0;
        while ( !(x & (1 << (total_bits - 1))) )
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

enum Direction { North, Northeast, East, Southeast, South, Southwest, West, Northwest };

#endif // MOVES_H_INCLUDED
