#ifndef MOVES_H_INCLUDED
#define MOVES_H_INCLUDED

#include "boardGeneration.h"
#include "bitboard.h"
#include <stdint.h>
#include <iostream>
#include <string>
#include <limits.h>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <cstring>
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
private:
    Moves() {}

public:
    static Moves &getInstance()
    {
        static Moves shared;
        return shared;
    }

    BoardGeneration &boardGeneration = BoardGeneration::getInstance();

    uint64_t notMyPieces;
    uint64_t myPieces;
    uint64_t occupiedSquares;
    uint64_t opponentSquares;

    uint64_t horizontalAndVerticalMoves(int s)
    {
        myPieces = occupiedSquares & ~opponentSquares;
        uint64_t sBinary = 1ULL << s;
        uint64_t rankMask = rankMasks8[s / 8];
        uint64_t fileMask = fileMasks8[s % 8];
        uint64_t rookMask = (rankMask | fileMask) & ~(sBinary);
        uint64_t blockersMask = occupiedSquares & ~(sBinary);
        uint64_t maskedBlockers = rookMask & blockersMask;
        uint64_t possibleMoves = 0;
        Direction direction = North;
        uint64_t testingSquare = sBinary >> 8;

        while (direction == North)
        {
            if (((testingSquare & maskedBlockers & myPieces) != 0) || testingSquare <= 0)
            {
                testingSquare = sBinary << 1;
                direction = East;
            }
            else
            {
                possibleMoves |= testingSquare;
                if (testingSquare >= 256 && ((testingSquare & maskedBlockers) == 0))
                {
                    testingSquare >>= 8;
                }
                else
                {
                    testingSquare = 0;
                }
            }
        }

        while (direction == East)
        {
            if (((testingSquare & maskedBlockers & myPieces) != 0) | (testingSquare & fileA) != 0)
            {
                testingSquare = sBinary << 8;
                direction = South;
            }
            else
            {
                possibleMoves |= testingSquare;
                if (((testingSquare & maskedBlockers) == 0))
                {
                    testingSquare <<= 1;
                }
                else
                {
                    testingSquare = fileA;
                }
            }
        }

        while (direction == South)
        {
            if (((testingSquare & maskedBlockers & myPieces) != 0) || testingSquare >= UINT64_MAX)
            {
                testingSquare = sBinary >> 1;
                direction = West;
            }
            else
            {
                possibleMoves |= testingSquare;
                if (testingSquare <= 36028797018963968 && ((testingSquare & maskedBlockers) == 0))
                {
                    testingSquare <<= 8;
                }
                else
                {
                    testingSquare = UINT64_MAX;
                }
            }
        }

        while (testingSquare != 0)
        {
            if (((testingSquare & maskedBlockers & myPieces) != 0) | (testingSquare & fileH) != 0)
            {
                testingSquare = 0;
            }
            else
            {
                possibleMoves |= testingSquare;
                if ((testingSquare & maskedBlockers) == 0)
                {
                    testingSquare >>= 1;
                }
                else
                {
                    testingSquare = 0;
                }
            }
        }

        return possibleMoves;
    }

    uint64_t coveredHorizontalAndVericalSquares(int s)
    {
        myPieces = occupiedSquares & ~opponentSquares;
        uint64_t sBinary = 1ULL << s;
        uint64_t rankMask = rankMasks8[s / 8];
        uint64_t fileMask = fileMasks8[s % 8];
        uint64_t rookMask = (rankMask | fileMask) & ~(sBinary);
        uint64_t blockersMask = occupiedSquares & ~(sBinary);
        uint64_t maskedBlockers = rookMask & blockersMask;
        uint64_t coveredSquares = 0;
        Direction direction = North;
        uint64_t testingSquare = sBinary >> 8;

        while (direction == North)
        {
            if (testingSquare <= 0)
            {
                testingSquare = sBinary << 1;
                direction = East;
            }
            else if (((testingSquare & maskedBlockers) != 0))
            {
                coveredSquares |= testingSquare;
                testingSquare = sBinary << 1;
                direction = East;
            }
            else
            {
                coveredSquares |= testingSquare;
                testingSquare >>= 8;
            }
        }

        while (direction == East)
        {
            if ((testingSquare & fileA) != 0)
            {
                testingSquare = sBinary << 8;
                direction = South;
            }
            else if (((testingSquare & maskedBlockers) != 0))
            {
                coveredSquares |= testingSquare;
                testingSquare = sBinary << 8;
                direction = South;
            }
            else
            {
                coveredSquares |= testingSquare;
                testingSquare <<= 1;
            }
        }

        while (direction == South)
        {
            if (testingSquare >= UINT64_MAX || testingSquare == 0)
            {
                testingSquare = sBinary >> 1;
                direction = West;
            }
            else if (((testingSquare & maskedBlockers) != 0))
            {
                coveredSquares |= testingSquare;
                testingSquare = sBinary >> 1;
                direction = West;
            }
            else
            {
                coveredSquares |= testingSquare;
                testingSquare <<= 8;
            }
        }

        while (testingSquare != 0)
        {
            if ((testingSquare & fileH) != 0)
            {
                testingSquare = 0;
            }
            else if (((testingSquare & maskedBlockers) != 0))
            {
                coveredSquares |= testingSquare;
                testingSquare = 0;
            }
            else
            {
                coveredSquares |= testingSquare;
                testingSquare >>= 1;
            }
        }

        return coveredSquares;
    }

    uint64_t diagonalAndAntiDiagonalMoves(int s)
    {
        myPieces = occupiedSquares & ~opponentSquares;
        uint64_t sBinary = 1ULL << s;
        uint64_t diagonalMask = diagonalMasks8[s % 8 + s / 8];
        uint64_t antiDiagonalMask = antiDiagonalMasks8[(7 - s % 8) + s / 8];
        uint64_t bishopMask = (diagonalMask | antiDiagonalMask) & ~(sBinary);
        uint64_t blockersMask = occupiedSquares & ~(sBinary);
        uint64_t maskedBlockers = bishopMask & blockersMask;
        uint64_t possibleMoves = 0;
        Direction direction = Northeast;
        uint64_t testingSquare = sBinary >> 7;

        while (direction == Northeast)
        {
            if (((testingSquare & maskedBlockers & myPieces) != 0) || (testingSquare & fileA) != 0 || testingSquare <= 0)
            {
                testingSquare = sBinary << 9;
                direction = Southeast;
            }
            else
            {
                possibleMoves |= testingSquare;
                if (testingSquare >= 256 && ((testingSquare & maskedBlockers) == 0))
                {
                    testingSquare >>= 7;
                }
                else
                {
                    testingSquare = 0;
                }
            }
        }

        while (direction == Southeast)
        {
            if (((testingSquare & maskedBlockers & myPieces) != 0) || ((testingSquare & fileA) != 0) || testingSquare >= UINT64_MAX)
            {
                testingSquare = sBinary << 7;
                direction = Southwest;
            }
            else
            {
                possibleMoves |= testingSquare;
                if (testingSquare <= 18014398509481984 && ((testingSquare & maskedBlockers) == 0))
                {
                    testingSquare <<= 9;
                }
                else
                {
                    testingSquare = UINT64_MAX;
                }
            }
        }

        while (direction == Southwest)
        {
            if (((testingSquare & maskedBlockers & myPieces) != 0) || ((testingSquare & fileH) != 0) || testingSquare >= UINT64_MAX)
            {
                testingSquare = sBinary >> 9;
                direction = Northwest;
            }
            else
            {
                possibleMoves |= testingSquare;
                if (testingSquare <= 72057594037927936 && ((testingSquare & maskedBlockers) == 0))
                {
                    testingSquare <<= 7;
                }
                else
                {
                    testingSquare = UINT64_MAX;
                }
            }
        }

        while (testingSquare != 0)
        {
            if (((testingSquare & maskedBlockers & myPieces) != 0) || ((testingSquare & fileH) != 0) || testingSquare <= 0)
            {
                testingSquare = 0;
            }
            else
            {
                possibleMoves |= testingSquare;
                if (testingSquare >= 512 && ((testingSquare & maskedBlockers) == 0))
                {
                    testingSquare >>= 9;
                }
                else
                {
                    testingSquare = 0;
                }
            }
        }

        return possibleMoves;
    }

    uint64_t coveredDiagonalAndAntiDiagonalSquares(int s)
    {
        myPieces = occupiedSquares & ~opponentSquares;
        uint64_t sBinary = 1ULL << s;
        uint64_t diagonalMask = diagonalMasks8[s % 8 + s / 8];
        uint64_t antiDiagonalMask = antiDiagonalMasks8[(7 - s % 8) + s / 8];
        uint64_t bishopMask = (diagonalMask | antiDiagonalMask) & ~(sBinary);
        uint64_t blockersMask = occupiedSquares & ~(sBinary);
        uint64_t maskedBlockers = bishopMask & blockersMask;
        uint64_t coveredSquares = 0;
        Direction direction = Northeast;
        uint64_t testingSquare = sBinary >> 7;

        while (direction == Northeast)
        {

            if (((testingSquare & fileA) != 0) || testingSquare <= 0)
            {
                testingSquare = sBinary << 9;
                direction = Southeast;
            }
            else
            {
                coveredSquares |= testingSquare;
                if ((testingSquare & maskedBlockers) != 0)
                {
                    testingSquare = sBinary << 9;
                    direction = Southeast;
                }
                else if (testingSquare >= 256)
                {
                    testingSquare >>= 7;
                }
                else
                {
                    testingSquare = 0;
                }
            }
        }

        while (direction == Southeast)
        {
            if (((testingSquare & fileA) != 0) || testingSquare >= UINT64_MAX || testingSquare == 0)
            {
                testingSquare = sBinary << 7;
                direction = Southwest;
            }
            else
            {
                coveredSquares |= testingSquare;
                if ((testingSquare & maskedBlockers) != 0)
                {
                    testingSquare = sBinary << 7;
                    direction = Southwest;
                }
                else if (testingSquare <= 18014398509481984)
                {
                    testingSquare <<= 9;
                }
                else
                {
                    testingSquare = UINT64_MAX;
                }
            }
        }

        while (direction == Southwest)
        {

            if (((testingSquare & fileH) != 0) || testingSquare >= UINT64_MAX || testingSquare == 0)
            {
                testingSquare = sBinary >> 9;
                direction = Northwest;
            }
            else
            {
                coveredSquares |= testingSquare;
                if ((testingSquare & maskedBlockers) != 0)
                {
                    testingSquare = sBinary >> 9;
                    direction = Northwest;
                }
                else if (testingSquare <= 72057594037927936)
                {
                    testingSquare <<= 7;
                }
                else
                {
                    testingSquare = UINT64_MAX;
                }
            }
        }

        while (testingSquare != 0)
        {
            if (((testingSquare & fileH) != 0) || testingSquare <= 0)
            {
                testingSquare = 0;
            }
            else
            {
                coveredSquares |= testingSquare;
                if ((testingSquare & maskedBlockers) != 0)
                {
                    testingSquare = 0;
                }
                else if (testingSquare >= 512)
                {
                    testingSquare >>= 9;
                }
                else
                {
                    testingSquare = 0;
                }
            }
        }

        return coveredSquares;
    }

    string possibleWP()
    {
        string moveList = "";
        uint64_t possibility;

        //first, look at right-captures
        uint64_t pawnMoves = (boardGeneration.WP >> 7) & boardGeneration.blackPieces() & ~rank8 & ~fileA;
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
        pawnMoves = (boardGeneration.WP >> 9) & boardGeneration.blackPieces() & ~rank8 & ~fileH;
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
        pawnMoves = (boardGeneration.WP >> 8) & boardGeneration.empty() & ~rank8;
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
        pawnMoves = (boardGeneration.WP >> 16) & boardGeneration.empty() & (boardGeneration.empty() >> 8) & rank4;
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
        pawnMoves = (boardGeneration.WP >> 7) & boardGeneration.occupied() & boardGeneration.blackPieces() & rank8 & ~fileA;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    string movementString = to_string(i % 8 - 1) + to_string(i % 8);
                    moveList += (movementString + "QP" + movementString + "RP" + movementString + "BP" + movementString + "NP");
                }
            }
        }

        //now, promoting, capturing left
        pawnMoves = (boardGeneration.WP >> 9) & boardGeneration.occupied() & boardGeneration.blackPieces() & rank8 & ~fileH;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    string movementString = to_string(i % 8 + 1) + to_string(i % 8);
                    moveList += (movementString + "QP" + movementString + "RP" + movementString + "BP" + movementString + "NP");
                }
            }
        }

        //now, promoting, moving 1 forward
        pawnMoves = (boardGeneration.WP >> 8) & boardGeneration.empty() & rank8;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    string movementString = to_string(i % 8) + to_string(i % 8);
                    moveList += (movementString + "QP" + movementString + "RP" + movementString + "BP" + movementString + "NP");
                }
            }
        }

        //En passant
        possibility = (boardGeneration.WP << 1) & boardGeneration.BP & rank5 & ~fileA & boardGeneration.EP;
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (to_string(index % 8 - 1) + to_string(index % 8) + "WE");
        }

        possibility = (boardGeneration.WP >> 1) & boardGeneration.BP & rank5 & ~fileH & boardGeneration.EP;
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (to_string(index % 8 + 1) + to_string(index % 8) + "WE");
        }

        return moveList;
    }

    string possibleBP()
    {
        string moveList = "";
        uint64_t possibility;

        //first, look at right-captures
        uint64_t pawnMoves = (boardGeneration.BP << 7) & boardGeneration.whitePieces() & ~rank1 & ~fileH;
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
        pawnMoves = (boardGeneration.BP << 9) & boardGeneration.whitePieces() & ~rank1 & ~fileA;
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
        pawnMoves = (boardGeneration.BP << 8) & boardGeneration.empty() & ~rank1;
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
        pawnMoves = (boardGeneration.BP << 16) & boardGeneration.empty() & (boardGeneration.empty() << 8) & rank5;
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
        pawnMoves = (boardGeneration.BP << 7) & boardGeneration.occupied() & boardGeneration.whitePieces() & rank1 & ~fileH;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    string movementString = to_string(i % 8 + 1) + to_string(i % 8);
                    moveList += (movementString + "QP" + movementString + "RP" + movementString + "BP" + movementString + "NP");
                }
            }
        }

        //now, promoting, capturing left
        pawnMoves = (boardGeneration.BP << 9) & boardGeneration.occupied() & boardGeneration.whitePieces() & rank1 & ~fileA;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    string movementString = to_string(i % 8 - 1) + to_string(i % 8);
                    moveList += (movementString + "QP" + movementString + "RP" + movementString + "BP" + movementString + "NP");
                }
            }
        }

        //now, promoting, moving 1 forward
        pawnMoves = (boardGeneration.BP << 8) & boardGeneration.empty() & rank1;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    string movementString = to_string(i % 8) + to_string(i % 8);
                    moveList += (movementString + "QP" + movementString + "RP" + movementString + "BP" + movementString + "NP");
                }
            }
        }

        //En passant
        possibility = (boardGeneration.BP >> 1) & boardGeneration.WP & rank4 & ~fileH & boardGeneration.EP;
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (to_string(index % 8 + 1) + to_string(index % 8) + "WE");
        }

        possibility = (boardGeneration.BP << 1) & boardGeneration.WP & rank4 & ~fileA & boardGeneration.EP;
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (to_string(index % 8 - 1) + to_string(index % 8) + "WE");
        }

        return moveList;
    }

    string possibleN(uint64_t N)
    {
        string movesList = "";
        string binaryN = convertBitboardToStringRep(N);

        for (int i = 0; i < strlen(binaryN.c_str()); i++)
        {
            char character = binaryN[i];
            if (character == '1')
            {
                uint64_t thisKnightsPossibilities = 0;
                uint64_t span;
                if (i < 18) {
                    span = knightSpan >> 18 - i;
                } else {
                    span = knightSpan << i - 18;
                }
                if (i % 8 < 5)
                {
                    thisKnightsPossibilities |= (span & ~(filesGH));
                }
                else
                {
                    thisKnightsPossibilities |= (span & ~(filesAB));
                }
                string thisKnightsPossibilitiesBinary = convertBitboardToStringRep(thisKnightsPossibilities);
                for (int j = 0; j < strlen(binaryN.c_str()); j++)
                {
                    char character = thisKnightsPossibilitiesBinary[j];
                    if (character == '1')
                    {
                        movesList += (to_string(i / 8) + to_string(i % 8) + to_string(j / 8) + to_string(j % 8));
                    }
                }
            }
        }

        return movesList;
    }

    string possibleB(uint64_t B)
    {
        uint64_t possibleDestinations = diagonalAndAntiDiagonalMoves(B);
        string pos = convertStartAndPossibleDestinationsToMovesString(B, possibleDestinations);
        return pos;
    }

    string possibleR(uint64_t R)
    {
        uint64_t possibleDestinations = horizontalAndVerticalMoves(R);
        return convertStartAndPossibleDestinationsToMovesString(R, possibleDestinations);
        ;
    }

    string possibleQ(int Q)
    {
        uint64_t hv = horizontalAndVerticalMoves(Q);
        uint64_t da = diagonalAndAntiDiagonalMoves(Q);
        uint64_t possibleDestinations = hv | da;

        return convertStartAndPossibleDestinationsToMovesString(Q, possibleDestinations);
    }

    string possibleK(uint64_t K)
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
            possibility &= ~filesGH & ~(boardGeneration.whitePieces());
        }
        else
        {
            possibility &= ~filesAB & ~(boardGeneration.whitePieces());
        }

        if (boardGeneration.whiteToMove)
        {
            uint64_t safeSquares = ~unsafeForWhite();
            uint64_t nonWhiteOccupiedSquares = ~(boardGeneration.WP | boardGeneration.WN | boardGeneration.WB | boardGeneration.WR | boardGeneration.WQ);
            possibility = possibility & safeSquares & nonWhiteOccupiedSquares;
        }
        else
        {
            uint64_t safeSquares = ~unsafeForBlack();
            uint64_t nonBlackOccupiedSquares = ~(boardGeneration.BP | boardGeneration.BN | boardGeneration.BB | boardGeneration.BR | boardGeneration.BQ);
            possibility = possibility & safeSquares & nonBlackOccupiedSquares;
        }

        string binaryPossibilities = convertBitboardToStringRep(possibility);

        for (int i = 0; i < strlen(binaryPossibilities.c_str()); i++)
        {
            char character = binaryPossibilities[i];
            if (character == '1')
            {
                movesList += (to_string(convertUint64ToIndex(K) / 8) + to_string(convertUint64ToIndex(K) % 8) + to_string(i / 8) + to_string(i % 8));
            }
        }

        return movesList;
    }

    string possibleCW(uint64_t WP, uint64_t WN, uint64_t WB, uint64_t WR, uint64_t WQ, uint64_t WK, uint64_t BP, uint64_t BN, uint64_t BB, uint64_t BR, uint64_t BQ, uint64_t BK, bool CWK, bool CWQ)
    {
        string movesList = "";
        uint64_t unsafe = unsafeForWhite();

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
        uint64_t unsafe = unsafeForBlack();

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

    uint64_t unsafeForWhite()
    {
        occupiedSquares = boardGeneration.occupied();
        opponentSquares = boardGeneration.whitePieces();
        uint64_t unsafe = 0;
        occupiedSquares = boardGeneration.WP | boardGeneration.WN | boardGeneration.WB | boardGeneration.WR | boardGeneration.WQ | boardGeneration.WK | boardGeneration.BP | boardGeneration.BN | boardGeneration.BB | boardGeneration.BR | boardGeneration.BQ | boardGeneration.BK;
        //pawn
        uint64_t rightCaptures = ((boardGeneration.BP << 7) & ~fileH);
        uint64_t leftCaptures = ((boardGeneration.BP << 9) & ~fileA);
        uint64_t pawnCaptures = rightCaptures | leftCaptures;
        unsafe = pawnCaptures;
        uint64_t possibility;
        //knight
        uint64_t knightCaptures = 0;
        uint64_t i = boardGeneration.BN & ~(boardGeneration.BN - 1);
        while (i != 0)
        {
            int iLocation = countTrailingZeros(i);
            if (iLocation > 18)
            {
                knightCaptures = knightSpan << (iLocation - 18);
            }
            else
            {
                knightCaptures = knightSpan >> (18 - iLocation);
            }
            if (iLocation % 8 < 4)
            {
                knightCaptures &= ~filesGH;
            }
            else
            {
                knightCaptures &= ~filesAB;
            }
            unsafe |= knightCaptures;
            boardGeneration.BN &= ~i;
            i = boardGeneration.BN & ~(boardGeneration.BN - 1);
        }
        //bishop/queen
        uint64_t bishopQueenCaptures = 0;
        uint64_t QB = boardGeneration.BQ | boardGeneration.BB;
        string QBBinary = convertBitboardToStringRep(QB);
        for (int i = 0; i < strlen(QBBinary.c_str()); i++)
        {
            char character = QBBinary[i];
            if (character == '1')
            {
                uint64_t covered = coveredDiagonalAndAntiDiagonalSquares(i);
                bishopQueenCaptures |= covered;
            }
        }
        unsafe |= bishopQueenCaptures;
        //rook/queen
        uint64_t rookQueenCaptures = 0;
        uint64_t QR = boardGeneration.BQ | boardGeneration.BR;
        string QRBinary = convertBitboardToStringRep(QR);
        for (int i = 0; i < strlen(QRBinary.c_str()); i++)
        {
            char character = QRBinary[i];
            if (character == '1')
            {
                uint64_t covered = coveredHorizontalAndVericalSquares(i);
                rookQueenCaptures |= covered;
            }
        }
        unsafe |= rookQueenCaptures;
        //king
        uint64_t kingCaptures = 0;
        int iLocation = countTrailingZeros(boardGeneration.BK);
        if (iLocation > 9)
        {
            kingCaptures = kingSpan << (iLocation - 9);
        }
        else
        {
            kingCaptures = kingSpan >> (9 - iLocation);
        }
        if (iLocation % 8 < 4)
        {
            kingCaptures &= ~filesGH;
        }
        else
        {
            kingCaptures &= ~filesAB;
        }
        unsafe |= kingCaptures;
        return unsafe;
    }

    uint64_t unsafeForBlack()
    {
        occupiedSquares = boardGeneration.occupied();
        opponentSquares = boardGeneration.blackPieces();
        uint64_t unsafe = 0;
        occupiedSquares = boardGeneration.WP | boardGeneration.WN | boardGeneration.WB | boardGeneration.WR | boardGeneration.WQ | boardGeneration.WK | boardGeneration.BP | boardGeneration.BN | boardGeneration.BB | boardGeneration.BR | boardGeneration.BQ | boardGeneration.BK;
        //pawn
        uint64_t rightCaptures = ((boardGeneration.WP >> 7) & ~fileA);
        uint64_t leftCaptures = ((boardGeneration.WP >> 9) & ~fileH);
        uint64_t pawnCaptures = rightCaptures | leftCaptures;
        unsafe = pawnCaptures;
        uint64_t possibility;
        //knight
        uint64_t knightCaptures = 0;
        uint64_t i = boardGeneration.WN & ~(boardGeneration.WN - 1);
        while (i != 0)
        {
            int iLocation = countTrailingZeros(i);
            if (iLocation > 18)
            {
                knightCaptures = knightSpan << (iLocation - 18);
            }
            else
            {
                knightCaptures = knightSpan >> (18 - iLocation);
            }
            if (iLocation % 8 < 4)
            {
                knightCaptures &= ~filesGH;
            }
            else
            {
                knightCaptures &= ~filesAB;
            }
            unsafe |= knightCaptures;
            boardGeneration.WN &= ~i;
            i = boardGeneration.WN & ~(boardGeneration.WN - 1);
        }

        //bishop/queen
        uint64_t bishopQueenCaptures = 0;
        uint64_t QB = boardGeneration.WQ | boardGeneration.WB;
        string QBBinary = convertBitboardToStringRep(QB);
        for (int i = 0; i < strlen(QBBinary.c_str()); i++)
        {
            char character = QBBinary[i];
            if (character == '1')
            {
                uint64_t covered = coveredDiagonalAndAntiDiagonalSquares(i);
                bishopQueenCaptures |= covered;
            }
        }
        unsafe |= bishopQueenCaptures;
        //rook/queen
        uint64_t rookQueenCaptures = 0;
        uint64_t QR = boardGeneration.WQ | boardGeneration.WR;
        string QRBinary = convertBitboardToStringRep(QR);
        for (int i = 0; i < strlen(QRBinary.c_str()); i++)
        {
            char character = QRBinary[i];
            if (character == '1')
            {
                uint64_t covered = coveredHorizontalAndVericalSquares(i);
                rookQueenCaptures |= covered;
            }
        }
        unsafe |= rookQueenCaptures;
        //king
        uint64_t kingCaptures = 0;
        int iLocation = countTrailingZeros(boardGeneration.WK);
        if (iLocation > 9)
        {
            kingCaptures = kingSpan << (iLocation - 9);
        }
        else
        {
            kingCaptures = kingSpan >> (9 - iLocation);
        }
        if (iLocation % 8 < 4)
        {
            kingCaptures &= ~filesGH;
        }
        else
        {
            kingCaptures &= ~filesAB;
        }
        unsafe |= kingCaptures;
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

    uint64_t bitSwapped(uint64_t i)
    {
        uint64_t v = i;
        uint64_t s = sizeof(v) * CHAR_BIT;
        uint64_t mask = 0;
        while (s > 1)
        {
            s = s >> 1;
            mask ^= mask << s;
            v = ((v >> s) & mask) | ((v << s) & ~mask);
        }
        return v;
    }

    string convertStartAndPossibleDestinationsToMovesString(int s, uint64_t pd)
    {
        string movesList = "";

        std::string binaryPossibilities = convertBitboardToStringRep(pd);
        for (int i = 0; i < strlen(binaryPossibilities.c_str()); i++)
        {
            char character = binaryPossibilities[i];

            if (character == '1')
            {

                string start = to_string(s);
                if (strlen(start.c_str()) == 1)
                {
                    start = "0" + start;
                }
                string end = to_string(i);
                if (strlen(end.c_str()) == 1)
                {
                    end = "0" + end;
                }

                movesList = movesList + start + end;
            }
        }
        return movesList;
    }

    string convertBitboardToStringRep(uint64_t bitboard)
    {
        string bitBoardString = std::bitset<64>(bitboard).to_string();

        //Pad binaryPossibilities string to 64 chars
        std::stringstream ss;
        ss << std::setw(64) << std::setfill('0') << bitBoardString;
        bitBoardString = ss.str();

        reverse(bitBoardString.begin(), bitBoardString.end());
        return bitBoardString;
    }

    int convertUint64ToIndex(uint64_t n)
    {
        return log(n) / log(2);
    }
};

#endif // MOVES_H_INCLUDED
