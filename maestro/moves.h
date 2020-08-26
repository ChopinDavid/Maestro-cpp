#ifndef MOVES_H_INCLUDED
#define MOVES_H_INCLUDED

#include "board.h"
#include "bitboard.h"
#include "direction.h"
#include <stdint.h>
#include <iostream>
#include <string>
#include <limits.h>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <bitset>
#include <algorithm>
using namespace std;

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

    uint64_t horizontalAndVerticalMoves(Board board, int s)
    {
        uint64_t myPieces;
        if (board.getWhiteToMove())
        {
            myPieces = board.whitePieces();
        }
        else
        {
            myPieces = board.blackPieces();
        }

        uint64_t sBinary = 1ULL << s;
        uint64_t rankMask = rankMasks8[s / 8];
        uint64_t fileMask = fileMasks8[s % 8];
        uint64_t rookMask = (rankMask | fileMask) & ~(sBinary);
        uint64_t blockersMask = board.occupied() & ~(sBinary);
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
            if (((testingSquare & maskedBlockers & myPieces) != 0) | (testingSquare & fileA) != 0 || testingSquare == 0)
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
            if (((testingSquare & maskedBlockers & myPieces) != 0) || testingSquare >= UINT64_MAX || testingSquare == 0)
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

    uint64_t coveredHorizontalAndVericalSquares(Board board, int s)
    {
        uint64_t myPieces;
        if (board.getWhiteToMove())
        {
            myPieces = board.whitePieces();
        }
        else
        {
            myPieces = board.blackPieces();
        }
        uint64_t sBinary = 1ULL << s;
        uint64_t rankMask = rankMasks8[s / 8];
        uint64_t fileMask = fileMasks8[s % 8];
        uint64_t rookMask = (rankMask | fileMask) & ~(sBinary);
        uint64_t blockersMask = board.occupied() & ~(sBinary);
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

            if ((testingSquare & fileA) != 0 || testingSquare == 0)
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

    uint64_t diagonalAndAntiDiagonalMoves(Board board, int s)
    {
        uint64_t myPieces;
        if (board.getWhiteToMove())
        {
            myPieces = board.whitePieces();
        }
        else
        {
            myPieces = board.blackPieces();
        }
        uint64_t sBinary = 1ULL << s;
        uint64_t diagonalMask = diagonalMasks8[s % 8 + s / 8];
        uint64_t antiDiagonalMask = antiDiagonalMasks8[(7 - s % 8) + s / 8];
        uint64_t bishopMask = (diagonalMask | antiDiagonalMask) & ~(sBinary);
        uint64_t blockersMask = board.occupied() & ~(sBinary);
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
            if (((testingSquare & maskedBlockers & myPieces) != 0) || ((testingSquare & fileA) != 0) || testingSquare >= UINT64_MAX || testingSquare == 0)
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
            if (((testingSquare & maskedBlockers & myPieces) != 0) || ((testingSquare & fileH) != 0) || testingSquare >= UINT64_MAX || testingSquare == 0)
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

    uint64_t coveredDiagonalAndAntiDiagonalSquares(Board board, int s)
    {
        uint64_t myPieces;
        if (board.getWhiteToMove())
        {
            myPieces = board.whitePieces();
        }
        else
        {
            myPieces = board.blackPieces();
        }
        uint64_t sBinary = 1ULL << s;
        uint64_t diagonalMask = diagonalMasks8[s % 8 + s / 8];
        uint64_t antiDiagonalMask = antiDiagonalMasks8[(7 - s % 8) + s / 8];
        uint64_t bishopMask = (diagonalMask | antiDiagonalMask) & ~(sBinary);
        uint64_t blockersMask = board.occupied() & ~(sBinary);
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

    string possibleWP(Board board)
    {
        string moveList = "";
        uint64_t possibility;

        //first, look at right-captures
        uint64_t pawnMoves = (board.getWP() >> 7) & board.blackPieces() & ~rank8 & ~fileA;
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
        pawnMoves = (board.getWP() >> 9) & board.blackPieces() & ~rank8 & ~fileH;
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
        pawnMoves = (board.getWP() >> 8) & board.empty() & ~rank8;
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
        pawnMoves = (board.getWP() >> 16) & board.empty() & (board.empty() >> 8) & rank4;
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
        pawnMoves = (board.getWP() >> 7) & board.occupied() & board.blackPieces() & rank8 & ~fileA;
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
        pawnMoves = (board.getWP() >> 9) & board.occupied() & board.blackPieces() & rank8 & ~fileH;
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
        pawnMoves = (board.getWP() >> 8) & board.empty() & rank8;
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
        possibility = (board.getWP() << 1) & board.getBP() & rank5 & ~fileA & board.getEP();
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (to_string(index % 8 - 1) + to_string(index % 8) + "WE");
        }

        possibility = (board.getWP() >> 1) & board.getBP() & rank5 & ~fileH & board.getEP();
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (to_string(index % 8 + 1) + to_string(index % 8) + "WE");
        }

        return moveList;
    }

    string possibleBP(Board board)
    {
        string moveList = "";
        uint64_t possibility;

        //first, look at right-captures
        uint64_t pawnMoves = (board.getBP() << 7) & board.whitePieces() & ~rank1 & ~fileH;
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
        pawnMoves = (board.getBP() << 9) & board.whitePieces() & ~rank1 & ~fileA;
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
        pawnMoves = (board.getBP() << 8) & board.empty() & ~rank1;
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
        pawnMoves = (board.getBP() << 16) & board.empty() & (board.empty() << 8) & rank5;
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
        pawnMoves = (board.getBP() << 7) & board.occupied() & board.whitePieces() & rank1 & ~fileH;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    string movementString = to_string(i % 8 + 1) + to_string(i % 8);
                    moveList += (movementString + "Qp" + movementString + "Rp" + movementString + "Bp" + movementString + "Np");
                }
            }
        }

        //now, promoting, capturing left
        pawnMoves = (board.getBP() << 9) & board.occupied() & board.whitePieces() & rank1 & ~fileA;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    string movementString = to_string(i % 8 - 1) + to_string(i % 8);
                    moveList += (movementString + "Qp" + movementString + "Rp" + movementString + "Bp" + movementString + "Np");
                }
            }
        }

        //now, promoting, moving 1 forward
        pawnMoves = (board.getBP() << 8) & board.empty() & rank1;
        if (pawnMoves != 0)
        {
            for (int i = countTrailingZeros(pawnMoves); i < 64 - countLeadingZeros(pawnMoves); i++)
            {
                if (((pawnMoves >> i) & 1) == 1)
                {
                    string movementString = to_string(i % 8) + to_string(i % 8);
                    moveList += (movementString + "Qp" + movementString + "Rp" + movementString + "Bp" + movementString + "Np");
                }
            }
        }

        //En passant
        possibility = (board.getBP() >> 1) & board.getWP() & rank4 & ~fileH & board.getEP();
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (to_string(index % 8 + 1) + to_string(index % 8) + "BE");
        }

        possibility = (board.getBP() << 1) & board.getWP() & rank4 & ~fileA & board.getEP();
        if (possibility != 0)
        {
            int index = countTrailingZeros(possibility);
            moveList += (to_string(index % 8 - 1) + to_string(index % 8) + "BE");
        }

        return moveList;
    }

    string possibleN(Board board)
    {
        uint64_t N;
        uint64_t friendlyPieces;
        if (board.getWhiteToMove())
        {
            N = board.getWN();
            friendlyPieces = board.whitePieces();
        }
        else
        {
            N = board.getBN();
            friendlyPieces = board.blackPieces();
        }
        string movesList = "";
        string binaryN = convertBitboardToStringRep(N);

        for (int i = 63; i >= 0; i--)
        {
            int y = 63 - i;
            char character = binaryN[y];
            if (character == '1')
            {
                uint64_t thisKnightsPossibilities = 0;
                uint64_t span;
                if (y < 18)
                {
                    span = knightSpan >> 18 - y;
                }
                else
                {
                    span = knightSpan << y - 18;
                }
                if (y % 8 < 2 || (y < 8 && y < 2))
                {
                    thisKnightsPossibilities |= (span & ~(filesGH));
                }
                else if (y % 8 > 5 || (y < 8 && y > 5))
                {
                    thisKnightsPossibilities |= (span & ~(filesAB));
                }
                else
                {
                    thisKnightsPossibilities |= span;
                }
                thisKnightsPossibilities &= ~friendlyPieces;
                string thisKnightsPossibilitiesBinary = convertBitboardToStringRep(thisKnightsPossibilities);
                for (int j = 0; j < strlen(binaryN.c_str()); j++)
                {
                    char character = thisKnightsPossibilitiesBinary[j];
                    if (character == '1')
                    {
                        movesList += (to_string(y / 8) + to_string(y % 8) + to_string(j / 8) + to_string(j % 8));
                    }
                }
            }
        }

        return movesList;
    }

    string possibleB(Board board)
    {
        uint64_t B;
        if (board.getWhiteToMove())
        {
            B = board.getWB();
        }
        else
        {
            B = board.getBB();
        }

        string movesList = "";
        string binaryB = convertBitboardToStringRep(B);

        for (int i = 0; i < strlen(binaryB.c_str()); i++)
        {
            char character = binaryB[i];
            if (character == '1')
            {
                uint64_t possibleDestinations = diagonalAndAntiDiagonalMoves(board, i);
                movesList += convertStartAndPossibleDestinationsToMovesString(i, possibleDestinations);
            }
        }
        return movesList;
    }

    string possibleR(Board board)
    {
        uint64_t R;
        if (board.getWhiteToMove())
        {
            R = board.getWR();
        }
        else
        {
            R = board.getBR();
        }

        string movesList = "";
        string binaryR = convertBitboardToStringRep(R);

        for (int i = 0; i < strlen(binaryR.c_str()); i++)
        {
            char character = binaryR[i];
            if (character == '1')
            {
                uint64_t possibleDestinations = horizontalAndVerticalMoves(board, i);
                movesList += convertStartAndPossibleDestinationsToMovesString(i, possibleDestinations);
            }
        }
        return movesList;
    }

    string possibleQ(Board board)
    {
        uint64_t Q;
        if (board.getWhiteToMove())
        {
            Q = board.getWQ();
        }
        else
        {
            Q = board.getBQ();
        }

        string movesList = "";
        string binaryQ = convertBitboardToStringRep(Q);

        for (int i = 0; i < strlen(binaryQ.c_str()); i++)
        {
            char character = binaryQ[i];
            if (character == '1')
            {
                uint64_t hv = horizontalAndVerticalMoves(board, i);
                uint64_t da = diagonalAndAntiDiagonalMoves(board, i);
                uint64_t possibleDestinations = hv | da;
                movesList += convertStartAndPossibleDestinationsToMovesString(i, possibleDestinations);
            }
        }
        return movesList;
    }

    string possibleK(Board board)
    {
        uint64_t K;
        uint64_t friendlyPieces;
        if (board.getWhiteToMove())
        {
            K = board.getWK();
            friendlyPieces = board.whitePieces();
        }
        else
        {
            K = board.getBK();
            friendlyPieces = board.blackPieces();
        }
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
            possibility &= ~filesGH & ~(friendlyPieces);
        }
        else
        {
            possibility &= ~filesAB & ~(friendlyPieces);
        }

        if (board.getWhiteToMove())
        {
            uint64_t safeSquares = ~unsafeForWhite(board);
            uint64_t nonWhiteOccupiedSquares = ~(board.getWP() | board.getWN() | board.getWB() | board.getWR() | board.getWQ());
            possibility = possibility & safeSquares & nonWhiteOccupiedSquares;
        }
        else
        {
            uint64_t safeSquares = ~unsafeForBlack(board);
            uint64_t nonBlackOccupiedSquares = ~(board.getBP() | board.getBN() | board.getBB() | board.getBR() | board.getBQ());
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

    string possibleCW(Board board)
    {
        uint64_t occupiedSquares = board.occupied();
        string movesList = "";
        uint64_t unsafe = unsafeForWhite(board);

        bool inCheck = (unsafe & board.getWK()) != 0;
        if (!inCheck)
        {
            if (board.getCWK() && (((1ULL << castleRooks[0]) & board.getWR()) != 0))
            {
                uint64_t occupiedAndUnsafeSquares = (occupiedSquares | unsafe);
                uint64_t kingSideBishopAndKnight = ((1ULL << 61) | (1ULL << 62));
                bool shortCastleLegal = (occupiedAndUnsafeSquares & kingSideBishopAndKnight) == 0;
                if (shortCastleLegal)
                {
                    movesList += "7476";
                }
            }

            if (board.getCWQ() && (((1ULL << castleRooks[1]) & board.getWR()) != 0))
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

    string possibleCB(Board board)
    {
        uint64_t occupiedSquares = board.occupied();
        string movesList = "";
        uint64_t unsafe = unsafeForBlack(board);

        bool inCheck = (unsafe & board.getBK()) != 0;
        if (!inCheck)
        {
            if (board.getCBK() && (((1ULL << castleRooks[2]) & board.getBR()) != 0))
            {
                uint64_t occupiedAndUnsafeSquares = (occupiedSquares | unsafe);
                uint64_t kingSideBishopAndKnight = ((1ULL << 5) | (1ULL << 6));
                bool shortCastleLegal = (occupiedAndUnsafeSquares & kingSideBishopAndKnight) == 0;
                if (shortCastleLegal)
                {
                    movesList += "0406";
                }
            }
            if (board.getCBQ() && (((1ULL << castleRooks[3]) & board.getBR()) != 0))
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

    uint64_t unsafeForWhite(Board board)
    {
        uint64_t occupiedSquares = board.occupied();
        uint64_t opponentSquares = board.whitePieces();
        uint64_t unsafe = 0;
        //pawn
        uint64_t rightCaptures = ((board.getBP() << 7) & ~fileH);
        uint64_t leftCaptures = ((board.getBP() << 9) & ~fileA);
        uint64_t pawnCaptures = rightCaptures | leftCaptures;
        unsafe = pawnCaptures;
        uint64_t possibility;
        //knight
        uint64_t knightCaptures = 0;
        string binaryN = convertBitboardToStringRep(board.getBN());

        for (int i = 0; i < strlen(binaryN.c_str()); i++)
        {
            char character = binaryN[i];
            if (character == '1')
            {
                uint64_t thisKnightsPossibilities = 0;
                uint64_t span;
                if (i < 18)
                {
                    span = knightSpan >> 18 - i;
                }
                else
                {
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
                unsafe |= thisKnightsPossibilities;
            }
        }
        //bishop/queen
        uint64_t bishopQueenCaptures = 0;
        uint64_t QB = board.getBQ() | board.getBB();
        string QBBinary = convertBitboardToStringRep(QB);
        for (int i = 0; i < strlen(QBBinary.c_str()); i++)
        {
            char character = QBBinary[i];
            if (character == '1')
            {
                uint64_t covered = coveredDiagonalAndAntiDiagonalSquares(board, i);
                bishopQueenCaptures |= covered;
            }
        }
        unsafe |= bishopQueenCaptures;
        //rook/queen
        uint64_t rookQueenCaptures = 0;
        uint64_t QR = board.getBQ() | board.getBR();
        string QRBinary = convertBitboardToStringRep(QR);
        for (int i = 0; i < strlen(QRBinary.c_str()); i++)
        {
            char character = QRBinary[i];
            if (character == '1')
            {
                uint64_t covered = coveredHorizontalAndVericalSquares(board, i);
                rookQueenCaptures |= covered;
            }
        }
        unsafe |= rookQueenCaptures;
        //king
        uint64_t kingCaptures = 0;
        int iLocation = countTrailingZeros(board.getBK());
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

    uint64_t unsafeForBlack(Board board)
    {
        uint64_t occupiedSquares = board.occupied();
        uint64_t opponentSquares = board.blackPieces();
        uint64_t unsafe = 0;
        //pawn
        uint64_t rightCaptures = ((board.getWP() >> 7) & ~fileA);
        uint64_t leftCaptures = ((board.getWP() >> 9) & ~fileH);
        uint64_t pawnCaptures = rightCaptures | leftCaptures;
        unsafe = pawnCaptures;
        uint64_t possibility;
        //knight
        uint64_t knightCaptures = 0;
        string binaryN = convertBitboardToStringRep(board.getWN());

        for (int i = 0; i < strlen(binaryN.c_str()); i++)
        {
            char character = binaryN[i];
            if (character == '1')
            {
                uint64_t thisKnightsPossibilities = 0;
                uint64_t span;
                if (i < 18)
                {
                    span = knightSpan >> 18 - i;
                }
                else
                {
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
                unsafe |= thisKnightsPossibilities;
            }
        }
        //bishop/queen
        uint64_t bishopQueenCaptures = 0;
        uint64_t QB = board.getWQ() | board.getWB();
        string QBBinary = convertBitboardToStringRep(QB);
        for (int i = 0; i < strlen(QBBinary.c_str()); i++)
        {
            char character = QBBinary[i];
            if (character == '1')
            {
                uint64_t covered = coveredDiagonalAndAntiDiagonalSquares(board, i);
                bishopQueenCaptures |= covered;
            }
        }
        unsafe |= bishopQueenCaptures;
        //rook/queen
        uint64_t rookQueenCaptures = 0;
        uint64_t QR = board.getWQ() | board.getWR();
        string QRBinary = convertBitboardToStringRep(QR);
        for (int i = 0; i < strlen(QRBinary.c_str()); i++)
        {
            char character = QRBinary[i];
            if (character == '1')
            {
                uint64_t covered = coveredHorizontalAndVericalSquares(board, i);
                rookQueenCaptures |= covered;
            }
        }
        unsafe |= rookQueenCaptures;
        //king
        uint64_t kingCaptures = 0;
        int iLocation = countTrailingZeros(board.getWK());
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

    string pseudoLegalMovesW(Board board)
    {
        string list = possibleWP(board) + possibleN(board) + possibleB(board) + possibleR(board) + possibleQ(board) + possibleK(board) + possibleCW(board);
        return list;
    }

    string pseudoLegalMovesB(Board board)
    {
        string list = possibleBP(board) + possibleN(board) + possibleB(board) + possibleR(board) + possibleQ(board) + possibleK(board) + possibleCB(board);
        return list;
    }

    uint64_t makeMoveSinglePiece(Board board, uint64_t bitboard, string move, char moveType)
    {
        if (isdigit(move[3]))
        { //'regular' move
            int start = (int(move[0]) - 48) * 8 + (int(move[1]) - 48);
            int end = (int(move[2]) - 48) * 8 + (int(move[3]) - 48);
            if (move == "7476" && ((1ULL << start) & board.getWK()) != 0 && bitboard == board.getWR())
            {
                //Short white castle
                bitboard &= ~(1ULL << 63);
                bitboard |= (1ULL << 61);
            }
            else if (move == "7472" && ((1ULL << start) & board.getWK()) != 0 && bitboard == board.getWR())
            {
                //Long white castle
                bitboard &= ~(1ULL << 56);
                bitboard |= (1ULL << 59);
            }
            else if (move == "0406" && ((1ULL << start) & board.getBK()) != 0 && bitboard == board.getBR())
            {
                //Short black castle
                bitboard &= ~(1ULL << 7);
                bitboard |= (1ULL << 5);
            }
            else if (move == "0402" && ((1ULL << start) & board.getBK()) != 0 && bitboard == board.getBR())
            {
                //Long black castle
                bitboard &= ~(1ULL << 0);
                bitboard |= (1ULL << 3);
            }
            else if ((board.getWhiteToMove() && moveType == 'E' && ((1ULL << start) & board.getWP()) != 0 && move[0] == '6' && move[2] == '4') || (!board.getWhiteToMove() && moveType == 'E' && ((1ULL << start) & board.getBP()) != 0 && move[0] == '1' && move[2] == '3'))
            {
                bitboard = fileMasks8[int(move[1]) - 48];
            }
            else if ((bitboard >> start) & 1 == 1)
            {
                bitboard &= ~(1ULL << start);
                bitboard |= (1ULL << end);
            }
            else
            {
                bitboard &= ~(1ULL << end);
            }
        }
        else if (tolower(move[3]) == 'p')
        { //pawn promotion
            int start;
            int end;
            if (isupper(move[3]))
            {
                start = countTrailingZeros(fileMasks8[int(move[0]) - 48] & rankMasks8[1]);
                end = countTrailingZeros(fileMasks8[int(move[1]) - 48] & rankMasks8[0]);
                if ((bitboard & (1ULL << end)) != 0)
                {
                    bitboard &= ~(1ULL << end);
                }
                if (bitboard == board.getWP())
                {
                    bitboard &= ~(1ULL << start);
                }
                if ((move[2] == 'Q' && moveType == 'Q') || (move[2] == 'R' && moveType == 'R') || (move[2] == 'B' && moveType == 'B') || (move[2] == 'N' && moveType == 'N'))
                {
                    bitboard |= (1ULL << end);
                }
            }
            else
            {
                start = countTrailingZeros(fileMasks8[int(move[0]) - 48] & rankMasks8[6]);
                end = countTrailingZeros(fileMasks8[int(move[1]) - 48] & rankMasks8[7]);
                if ((bitboard & (1ULL << end)) != 0)
                {
                    bitboard &= ~(1ULL << end);
                }
                if (bitboard == board.getBP())
                {
                    bitboard &= ~(1ULL << start);
                }
                if ((move[2] == 'Q' && moveType == 'q') || (move[2] == 'R' && moveType == 'r') || (move[2] == 'B' && moveType == 'b') || (move[2] == 'N' && moveType == 'n'))
                {
                    bitboard |= (1ULL << end);
                }
            }
        }
        else if (move[3] == 'E')
        { //en passant
            int start;
            int end;
            if (move[2] == 'W')
            {
                start = countTrailingZeros(fileMasks8[int(move[0] - 48)] & rankMasks8[3]);
                end = countTrailingZeros(fileMasks8[int(move[1] - 48)] & rankMasks8[2]);
                bitboard &= ~(fileMasks8[int(move[1]) - 48] & rankMasks8[3]);
            }
            else
            {
                start = countTrailingZeros(fileMasks8[int(move[0]) - 48] & rankMasks8[4]);
                end = countTrailingZeros(fileMasks8[int(move[1]) - 48] & rankMasks8[5]);
                bitboard &= ~(fileMasks8[int(move[1]) - 48] & rankMasks8[4]);
            }
            if (((bitboard >> start) & 1) == 1)
            {
                bitboard &= ~(1ULL << start);
                bitboard |= (1ULL << end);
            }
        }
        else
        {
            cout << "ERROR: Invalid move type" << endl;
        }
        return bitboard;
    }

    Board makeMoveAll(Board board, string move)
    {
        uint64_t tWP = makeMoveSinglePiece(board, board.getWP(), move, 'P');
        uint64_t tWN = makeMoveSinglePiece(board, board.getWN(), move, 'N');
        uint64_t tWB = makeMoveSinglePiece(board, board.getWB(), move, 'B');
        uint64_t tWR = makeMoveSinglePiece(board, board.getWR(), move, 'R');
        uint64_t tWQ = makeMoveSinglePiece(board, board.getWQ(), move, 'Q');
        uint64_t tWK = makeMoveSinglePiece(board, board.getWK(), move, 'K');
        uint64_t tBP = makeMoveSinglePiece(board, board.getBP(), move, 'p');
        uint64_t tBN = makeMoveSinglePiece(board, board.getBN(), move, 'n');
        uint64_t tBB = makeMoveSinglePiece(board, board.getBB(), move, 'b');
        uint64_t tBR = makeMoveSinglePiece(board, board.getBR(), move, 'r');
        uint64_t tBQ = makeMoveSinglePiece(board, board.getBQ(), move, 'q');
        uint64_t tBK = makeMoveSinglePiece(board, board.getBK(), move, 'k');
        uint64_t tEP = makeMoveSinglePiece(board, board.getEP(), move, 'E');

        bool tWhiteToMove = !board.getWhiteToMove();
        bool tCWK = true;
        bool tCWQ = true;
        bool tCBK = true;
        bool tCBQ = true;

        if (!board.getCWK() || tWK != 1152921504606846976 || (tWR & 9223372036854775808U) == 0)
        {
            tCWK = false;
        }

        if (!board.getCWQ() || tWK != 1152921504606846976 || (tWR & 72057594037927936) == 0)
        {
            tCWQ = false;
        }

        if (!board.getCBK() || tBK != 16 || (tBR & 128) == 0)
        {
            tCBK = false;
        }

        if (!board.getCBQ() || tBK != 16 || (tBR & 1) == 0)
        {
            tCBQ = false;
        }

        Board newBoard = Board(tWP, tWN, tWB, tWR, tWQ, tWK, tBP, tBN, tBB, tBR, tBQ, tBK, tEP, tWhiteToMove, tCWK, tCWQ, tCBK, tCBQ);
        return newBoard;
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
                string start = to_string(s / 8) + to_string(s % 8);
                string end = to_string(i / 8) + to_string(i % 8);
                movesList = movesList + start + end;
            }
        }
        return movesList;
    }

    string convertBitboardToStringRep(uint64_t bitboard)
    {
        std::string s = std::bitset<64>(bitboard).to_string();
        reverse(s.begin(), s.end());
        return s;
    }

    int convertUint64ToIndex(uint64_t n)
    {
        return log(n) / log(2);
    }
};

#endif // MOVES_H_INCLUDED
