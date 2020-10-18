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
        uint64_t occupied = board.occupied();
        uint64_t fileMask = fileMasks8[s % 8];
        uint64_t o = fileMask & occupied;
        uint64_t r = 1ULL << s;
        uint64_t oUpper = o & (r - 1);
        uint64_t oLower = o & ~(r - 1) & ~r;
        uint64_t upperBounds;
        if (oUpper == 0)
        {
            upperBounds = 0;
        }
        else
        {
            upperBounds = (msb(oUpper) - 1);
        }
        uint64_t lowerBounds;
        if (oLower == 0)
        {
            lowerBounds = 0;
        }
        else
        {
            uint64_t lowerLSB = lsb(oLower);
            lowerBounds = (oUpper - lowerLSB) & ~(lowerLSB | oUpper);
        }
        uint64_t attackMaskFile = (~(upperBounds | lowerBounds) & fileMask) ^ r;

        uint64_t rankMask = rankMasks8[s / 8];
        uint64_t rankMaskRotated = rotate90clockwise(rankMask);
        uint64_t oRotated = rankMaskRotated & rotate90clockwise(occupied);
        uint64_t rRotated = rotate90clockwise(r);
        uint64_t oRotatedUpper;
        if (rRotated < 256) {
            oRotatedUpper = 0;
        } else {
            oRotatedUpper = oRotated & ((rRotated >> 7) - 1);
        }
         
        uint64_t oRotatedLower = oRotated & ~((rRotated << 7) - 1) & ~rRotated;
        uint64_t upperBoundsRotated;
        if (oRotatedUpper == 0)
        {
            upperBoundsRotated = 0;
        }
        else
        {
            upperBoundsRotated = msb(oRotatedUpper) - 1;
        }

        uint64_t lowerBoundsRotated;
        if (oRotatedLower == 0)
        {
            lowerBoundsRotated = 0;
        }
        else
        {
            uint64_t lowerLSB = lsb(oRotatedLower);
            lowerBoundsRotated = (oRotatedUpper - lowerLSB) & ~(lowerLSB | oRotatedUpper);
        }
        uint64_t attackMaskRank = rotate90antiClockwise((~(upperBoundsRotated | lowerBoundsRotated) & rankMaskRotated) ^ rRotated);

        uint64_t result;
        if (board.getWhiteToMove())
        {
            result = (attackMaskFile | attackMaskRank) & ~(board.whitePieces());
        }
        else
        {
            result = (attackMaskFile | attackMaskRank) & ~(board.blackPieces());
        }
        return result;
    }

    uint64_t coveredHorizontalAndVericalSquares(Board board, int s)
    {
        uint64_t occupied = board.occupied();
        uint64_t fileMask = fileMasks8[s % 8];
        uint64_t o = fileMask & occupied;
        uint64_t r = 1ULL << s;
        uint64_t oUpper = o & (r - 1);
        uint64_t oLower = o & ~(r - 1) & ~r;
        uint64_t upperBounds;
        if (oUpper == 0)
        {
            upperBounds = 0;
        }
        else
        {
            upperBounds = (msb(oUpper) - 1);
        }
        uint64_t lowerBounds;
        if (oLower == 0)
        {
            lowerBounds = 0;
        }
        else
        {
            uint64_t lowerLSB = lsb(oLower);
            lowerBounds = (oUpper - lowerLSB) & ~(lowerLSB | oUpper);
        }
        uint64_t attackMaskFile = (~(upperBounds | lowerBounds) & fileMask) ^ r;

        uint64_t rankMask = rankMasks8[s / 8];
        uint64_t rankMaskRotated = rotate90clockwise(rankMask);
        uint64_t oRotated = rankMaskRotated & rotate90clockwise(occupied);
        uint64_t rRotated = rotate90clockwise(r);
        uint64_t oRotatedUpper;
        if (rRotated < 256) {
            oRotatedUpper = 0;
        } else {
            oRotatedUpper = oRotated & ((rRotated >> 7) - 1);
        }
         
        uint64_t oRotatedLower = oRotated & ~((rRotated << 7) - 1) & ~rRotated;
        uint64_t upperBoundsRotated;
        if (oRotatedUpper == 0)
        {
            upperBoundsRotated = 0;
        }
        else
        {
            upperBoundsRotated = msb(oRotatedUpper) - 1;
        }

        uint64_t lowerBoundsRotated;
        if (oRotatedLower == 0)
        {
            lowerBoundsRotated = 0;
        }
        else
        {
            uint64_t lowerLSB = lsb(oRotatedLower);
            lowerBoundsRotated = (oRotatedUpper - lowerLSB) & ~(lowerLSB | oRotatedUpper);
        }
        uint64_t attackMaskRank = rotate90antiClockwise((~(upperBoundsRotated | lowerBoundsRotated) & rankMaskRotated) ^ rRotated);
        return attackMaskFile | attackMaskRank;
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
        string movesList = "";
        uint64_t occupiedSquares = board.occupied();
        uint64_t unsafe = unsafeForWhite(board);
        uint64_t occupiedAndUnsafeSquares = (occupiedSquares | unsafe);

        bool inCheck = (unsafe & board.getWK()) != 0;
        if (!inCheck)
        {
            if (board.getCWK() && (((1ULL << castleRooks[0]) & board.getWR()) != 0))
            {
                bool shortCastleLegal = (occupiedAndUnsafeSquares & 6917529027641081856ULL) == 0;
                if (shortCastleLegal)
                {
                    movesList += "7476";
                }
            }

            if (board.getCWQ() && (((1ULL << castleRooks[1]) & board.getWR()) != 0))
            {
                bool longCastleLegal = ((unsafe & 864691128455135232ULL) == 0) && ((occupiedSquares & 1008806316530991104ULL) == 0);
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
        string movesList = "";
        uint64_t occupiedSquares = board.occupied();
        uint64_t unsafe = unsafeForBlack(board);
        uint64_t occupiedAndUnsafeSquares = (occupiedSquares | unsafe);

        bool inCheck = (unsafe & board.getBK()) != 0;
        if (!inCheck)
        {
            if (board.getCBK() && (((1ULL << castleRooks[2]) & board.getBR()) != 0))
            {
                bool shortCastleLegal = (occupiedAndUnsafeSquares & 96ULL) == 0;
                if (shortCastleLegal)
                {
                    movesList += "0406";
                }
            }
            if (board.getCBQ() && (((1ULL << castleRooks[3]) & board.getBR()) != 0))
            {
                bool longCastleLegal = ((unsafe & 12ULL) == 0) && ((occupiedSquares & 14ULL) == 0);
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
                if (i % 8 < 2)
                {
                    thisKnightsPossibilities |= (span & ~(filesGH));
                }
                else if (i % 8 > 5)
                {
                    thisKnightsPossibilities |= (span & ~(filesAB));
                }
                else
                {
                    thisKnightsPossibilities |= span;
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
                if (i % 8 < 2 || (i < 8 && i < 2))
                {
                    thisKnightsPossibilities |= (span & ~(filesGH));
                }
                else if (i % 8 > 5 || (i < 8 && i > 5))
                {
                    thisKnightsPossibilities |= (span & ~(filesAB));
                }
                else
                {
                    thisKnightsPossibilities |= span;
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

    bool isInCheck(Board board)
    {
        uint64_t unsafe;
        if (board.getWhiteToMove())
        {
            unsafe = unsafeForWhite(board);
        }
        else
        {
            unsafe = unsafeForBlack(board);
        }

        bool whiteIsInCheck = board.getWhiteToMove() && (board.getWK() & unsafeForWhite(board)) != 0;
        bool blackIsInCheck = !board.getWhiteToMove() && (board.getBK() & unsafeForBlack(board)) != 0;
        if (whiteIsInCheck || blackIsInCheck)
        {
            return true;
        }
        else
        {
            return false;
        }
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

    bool isCheckmate(Board board)
    {
        bool isCheckmate = false;
        if (board.getWhiteToMove() && (unsafeForWhite(board) & board.getWK()) != 0)
        {
            isCheckmate = true;
            string pseudoLegalMoves = pseudoLegalMovesW(board);
            for (int i = 0; i < pseudoLegalMoves.length(); i += 4)
            {
                string individualMoveString;
                individualMoveString += pseudoLegalMoves[i];
                individualMoveString += pseudoLegalMoves[i + 1];
                individualMoveString += pseudoLegalMoves[i + 2];
                individualMoveString += pseudoLegalMoves[i + 3];
                Board tBoard = makeMoveAll(board, individualMoveString);
                if ((unsafeForWhite(tBoard) & tBoard.getWK()) == 0)
                {
                    isCheckmate = false;
                }
            }
        }
        if (!board.getWhiteToMove() && (unsafeForBlack(board) & board.getBK()) != 0)
        {
            isCheckmate = true;
            string pseudoLegalMoves = pseudoLegalMovesB(board);
            for (int i = 0; i < pseudoLegalMoves.length(); i += 4)
            {
                string individualMoveString;
                individualMoveString += pseudoLegalMoves[i];
                individualMoveString += pseudoLegalMoves[i + 1];
                individualMoveString += pseudoLegalMoves[i + 2];
                individualMoveString += pseudoLegalMoves[i + 3];
                Board tBoard = makeMoveAll(board, individualMoveString);
                if ((unsafeForBlack(tBoard) & tBoard.getBK()) == 0)
                {
                    isCheckmate = false;
                }
            }
        }
        return isCheckmate;
    }

    uint64_t makeMoveSinglePiece(Board board, uint64_t bitboard, string move, char moveType)
    {
        if (moveType == 'E')
        {
            bitboard = 0ULL;
        }
        if (isdigit(move[3]))
        { //'regular' move
            int start = (int(move[0]) - 48) * 8 + (int(move[1]) - 48);
            int end = (int(move[2]) - 48) * 8 + (int(move[3]) - 48);
            if (move == "7476" && ((1ULL << start) & board.getWK()) != 0 && moveType == 'R')
            {
                //Short white castle
                bitboard &= ~(1ULL << 63);
                bitboard |= (1ULL << 61);
            }
            else if (move == "7472" && ((1ULL << start) & board.getWK()) != 0 && moveType == 'R')
            {
                //Long white castle
                bitboard &= ~(1ULL << 56);
                bitboard |= (1ULL << 59);
            }
            else if (move == "0406" && ((1ULL << start) & board.getBK()) != 0 && moveType == 'r')
            {
                //Short black castle
                bitboard &= ~(1ULL << 7);
                bitboard |= (1ULL << 5);
            }
            else if (move == "0402" && ((1ULL << start) & board.getBK()) != 0 && moveType == 'r')
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
                if (moveType == 'P')
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
                if (moveType == 'p')
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
            cout << "ERROR: Invalid move type: " << move << "move type: " << moveType << endl;
        }
        return bitboard;
    }

    int getNewHalfMoveClock(Board board, string move)
    {
        if (!isdigit(move[3]))
        {
            return 0;
        }
        else
        {
            int start = (int(move[0]) - 48) * 8 + (int(move[1]) - 48);
            int end = (int(move[2]) - 48) * 8 + (int(move[3]) - 48);

            if (((1ULL << start) & board.getWP()) != 0 || ((1ULL << start) & board.getBP()) != 0 || ((1ULL << end) & board.occupied()) != 0)
            {
                return 0;
            }
            else
            {
                return board.getHalfMoveClock() + 1;
            }
        }
    }

    int getNewMoveNumber(Board board, string move)
    {
        if (!board.getWhiteToMove())
        {
            return board.getMoveNumber() + 1;
        }
        else
        {
            return board.getMoveNumber();
        }
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
        int tHalfMoveClock = getNewHalfMoveClock(board, move);
        int tMoveNumber = getNewMoveNumber(board, move);
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

        Board newBoard = Board(tWP, tWN, tWB, tWR, tWQ, tWK, tBP, tBN, tBB, tBR, tBQ, tBK, tEP, tHalfMoveClock, tMoveNumber, tWhiteToMove, tCWK, tCWQ, tCBK, tCBQ);
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

    int convertUint64ToIndex(uint64_t n)
    {
        return log(n) / log(2);
    }

    string convertMoveToAlgebra(string move, Board board)
    {
        //Get start and end bitboards
        int start = (int(move[0]) - 48) * 8 + (int(move[1]) - 48);
        int end = (int(move[2]) - 48) * 8 + (int(move[3]) - 48);

        //Check if move is castle
        if ((move == "0406" && board.getCBK()) || move == "7476" && board.getCWK())
        {
            return "O-O";
        }
        if ((move == "0402" && board.getCBQ()) || move == "7472" && board.getCWQ())
        {
            return "O-O-O";
        }

        string outputString = "";

        if (((1ULL << start) & board.getWN()) != 0 || ((1ULL << start) & board.getBN()) != 0)
        {
            outputString += 'N';
        }
        if (((1ULL << start) & board.getWB()) != 0 || ((1ULL << start) & board.getBB()) != 0)
        {
            outputString += 'B';
        }
        if (((1ULL << start) & board.getWR()) != 0 || ((1ULL << start) & board.getBR()) != 0)
        {
            outputString += 'R';
        }
        if (((1ULL << start) & board.getWQ()) != 0 || ((1ULL << start) & board.getBQ()) != 0)
        {
            outputString += 'Q';
        }
        if (((1ULL << start) & board.getWK()) != 0 || ((1ULL << start) & board.getBK()) != 0)
        {
            outputString += 'K';
        }

        if (isdigit(move[3]))
        {
            switch (move[1])
            {
            case '0':
                outputString += 'a';
                break;
            case '1':
                outputString += 'b';
                break;
            case '2':
                outputString += 'c';
                break;
            case '3':
                outputString += 'd';
                break;
            case '4':
                outputString += 'e';
                break;
            case '5':
                outputString += 'f';
                break;
            case '6':
                outputString += 'g';
                break;
            case '7':
                outputString += 'h';
                break;
            }

            switch (move[0])
            {
            case '0':
                outputString += '8';
                break;
            case '1':
                outputString += '7';
                break;
            case '2':
                outputString += '6';
                break;
            case '3':
                outputString += '5';
                break;
            case '4':
                outputString += '4';
                break;
            case '5':
                outputString += '3';
                break;
            case '6':
                outputString += '2';
                break;
            case '7':
                outputString += '1';
                break;
            }

            if ((board.getWhiteToMove() && ((1ULL << end) & board.blackPieces()) != 0) || (!board.getWhiteToMove() && ((1ULL << end) & board.whitePieces()) != 0))
            {
                outputString += 'x';
            }
            else
            {
                outputString += "-";
            }

            switch (move[3])
            {
            case '0':
                outputString += 'a';
                break;
            case '1':
                outputString += 'b';
                break;
            case '2':
                outputString += 'c';
                break;
            case '3':
                outputString += 'd';
                break;
            case '4':
                outputString += 'e';
                break;
            case '5':
                outputString += 'f';
                break;
            case '6':
                outputString += 'g';
                break;
            case '7':
                outputString += 'h';
                break;
            }

            switch (move[2])
            {
            case '0':
                outputString += '8';
                break;
            case '1':
                outputString += '7';
                break;
            case '2':
                outputString += '6';
                break;
            case '3':
                outputString += '5';
                break;
            case '4':
                outputString += '4';
                break;
            case '5':
                outputString += '3';
                break;
            case '6':
                outputString += '2';
                break;
            case '7':
                outputString += '1';
                break;
            }
        }
        else if (tolower(move[3]) == 'p')
        {
            switch (move[0])
            {
            case '0':
                outputString += 'a';
                break;
            case '1':
                outputString += 'b';
                break;
            case '2':
                outputString += 'c';
                break;
            case '3':
                outputString += 'd';
                break;
            case '4':
                outputString += 'e';
                break;
            case '5':
                outputString += 'f';
                break;
            case '6':
                outputString += 'g';
                break;
            case '7':
                outputString += 'h';
                break;
            }
            if (board.getWhiteToMove())
            {
                outputString += '7';
            }
            else
            {
                outputString += '2';
            }
            if (move[0] == move[1])
            {
                outputString += "-";
                outputString += outputString[0];
            }
            else
            {
                outputString += 'x';
                switch (move[1])
                {
                case '0':
                    outputString += 'a';
                    break;
                case '1':
                    outputString += 'b';
                    break;
                case '2':
                    outputString += 'c';
                    break;
                case '3':
                    outputString += 'd';
                    break;
                case '4':
                    outputString += 'e';
                    break;
                case '5':
                    outputString += 'f';
                    break;
                case '6':
                    outputString += 'g';
                    break;
                case '7':
                    outputString += 'h';
                    break;
                }
            }
            if (board.getWhiteToMove())
            {
                outputString += '8';
            }
            else
            {
                outputString += '1';
            }
            outputString += move[2];
        }
        else if (move[3] == 'E')
        {
            switch (move[0])
            {
            case '0':
                outputString += 'a';
                break;
            case '1':
                outputString += 'b';
                break;
            case '2':
                outputString += 'c';
                break;
            case '3':
                outputString += 'd';
                break;
            case '4':
                outputString += 'e';
                break;
            case '5':
                outputString += 'f';
                break;
            case '6':
                outputString += 'g';
                break;
            case '7':
                outputString += 'h';
                break;
            }
            if (board.getWhiteToMove())
            {
                outputString += "5x";
            }
            else
            {
                outputString += "4x";
            }
            switch (move[1])
            {
            case '0':
                outputString += 'a';
                break;
            case '1':
                outputString += 'b';
                break;
            case '2':
                outputString += 'c';
                break;
            case '3':
                outputString += 'd';
                break;
            case '4':
                outputString += 'e';
                break;
            case '5':
                outputString += 'f';
                break;
            case '6':
                outputString += 'g';
                break;
            case '7':
                outputString += 'h';
                break;
            }
            if (board.getWhiteToMove())
            {
                outputString += '6';
            }
            else
            {
                outputString += '3';
            }
        }

        Board tBoard = makeMoveAll(board, move);

        if (isInCheck(tBoard))
        {
            outputString += '+';
        }

        return outputString;
    }

    string convertMoveToAlgebraUCI(string move, Board board)
    {
        //Get start and end bitboards
        int start = (int(move[0]) - 48) * 8 + (int(move[1]) - 48);
        int end = (int(move[2]) - 48) * 8 + (int(move[3]) - 48);

        string outputString = "";

        if (isdigit(move[3]))
        {
            switch (move[1])
            {
            case '0':
                outputString += 'a';
                break;
            case '1':
                outputString += 'b';
                break;
            case '2':
                outputString += 'c';
                break;
            case '3':
                outputString += 'd';
                break;
            case '4':
                outputString += 'e';
                break;
            case '5':
                outputString += 'f';
                break;
            case '6':
                outputString += 'g';
                break;
            case '7':
                outputString += 'h';
                break;
            }

            switch (move[0])
            {
            case '0':
                outputString += '8';
                break;
            case '1':
                outputString += '7';
                break;
            case '2':
                outputString += '6';
                break;
            case '3':
                outputString += '5';
                break;
            case '4':
                outputString += '4';
                break;
            case '5':
                outputString += '3';
                break;
            case '6':
                outputString += '2';
                break;
            case '7':
                outputString += '1';
                break;
            }

            switch (move[3])
            {
            case '0':
                outputString += 'a';
                break;
            case '1':
                outputString += 'b';
                break;
            case '2':
                outputString += 'c';
                break;
            case '3':
                outputString += 'd';
                break;
            case '4':
                outputString += 'e';
                break;
            case '5':
                outputString += 'f';
                break;
            case '6':
                outputString += 'g';
                break;
            case '7':
                outputString += 'h';
                break;
            }

            switch (move[2])
            {
            case '0':
                outputString += '8';
                break;
            case '1':
                outputString += '7';
                break;
            case '2':
                outputString += '6';
                break;
            case '3':
                outputString += '5';
                break;
            case '4':
                outputString += '4';
                break;
            case '5':
                outputString += '3';
                break;
            case '6':
                outputString += '2';
                break;
            case '7':
                outputString += '1';
                break;
            }
        }
        else if (tolower(move[3]) == 'p')
        {
            switch (move[0])
            {
            case '0':
                outputString += 'a';
                break;
            case '1':
                outputString += 'b';
                break;
            case '2':
                outputString += 'c';
                break;
            case '3':
                outputString += 'd';
                break;
            case '4':
                outputString += 'e';
                break;
            case '5':
                outputString += 'f';
                break;
            case '6':
                outputString += 'g';
                break;
            case '7':
                outputString += 'h';
                break;
            }
            if (!board.getWhiteToMove())
            {
                outputString += '7';
            }
            else
            {
                outputString += '2';
            }
            if (move[0] == move[1])
            {
                outputString += outputString[0];
            }
            else
            {
                switch (move[1])
                {
                case '0':
                    outputString += 'a';
                    break;
                case '1':
                    outputString += 'b';
                    break;
                case '2':
                    outputString += 'c';
                    break;
                case '3':
                    outputString += 'd';
                    break;
                case '4':
                    outputString += 'e';
                    break;
                case '5':
                    outputString += 'f';
                    break;
                case '6':
                    outputString += 'g';
                    break;
                case '7':
                    outputString += 'h';
                    break;
                }
            }
            if (!board.getWhiteToMove())
            {
                outputString += '8';
            }
            else
            {
                outputString += '1';
            }
            outputString += tolower(move[2]);
        }
        else if (move[3] == 'E')
        {
            switch (move[0])
            {
            case '0':
                outputString += 'a';
                break;
            case '1':
                outputString += 'b';
                break;
            case '2':
                outputString += 'c';
                break;
            case '3':
                outputString += 'd';
                break;
            case '4':
                outputString += 'e';
                break;
            case '5':
                outputString += 'f';
                break;
            case '6':
                outputString += 'g';
                break;
            case '7':
                outputString += 'h';
                break;
            }
            if (board.getWhiteToMove())
            {
                outputString += "5";
            }
            else
            {
                outputString += "4";
            }
            switch (move[1])
            {
            case '0':
                outputString += 'a';
                break;
            case '1':
                outputString += 'b';
                break;
            case '2':
                outputString += 'c';
                break;
            case '3':
                outputString += 'd';
                break;
            case '4':
                outputString += 'e';
                break;
            case '5':
                outputString += 'f';
                break;
            case '6':
                outputString += 'g';
                break;
            case '7':
                outputString += 'h';
                break;
            }
            if (board.getWhiteToMove())
            {
                outputString += '6';
            }
            else
            {
                outputString += '3';
            }
        }
        return outputString;
    }

    string convertMoveToMaestroIntString(string move, Board board)
    {
        string strippedString;
        int startSquare = (8 - int(move[1] - 48)) * 8 + convertLetterToFileInt(move[0]);
        uint64_t startSquareBinary = 1ULL << startSquare;
        int endSquare = (8 - int(move[3] - 48)) * 8 + convertLetterToFileInt(move[2]);
        uint64_t endSquareBinary = 1ULL << endSquare;
        bool isEP = (board.getWhiteToMove() && (startSquareBinary & board.getWP()) != 0 && move[0] != move[2] && (endSquareBinary & board.occupied()) == 0) || (!board.getWhiteToMove() && (startSquareBinary & board.getBP()) != 0 && move[0] != move[2] && (endSquareBinary & board.occupied()) == 0);

        if (move.length() == 5)
        {
            switch (move[0])
            {
            case 'a':
                strippedString += '0';
                break;
            case 'b':
                strippedString += '1';
                break;
            case 'c':
                strippedString += '2';
                break;
            case 'd':
                strippedString += '3';
                break;
            case 'e':
                strippedString += '4';
                break;
            case 'f':
                strippedString += '5';
                break;
            case 'g':
                strippedString += '6';
                break;
            case 'h':
                strippedString += '7';
                break;
            }
            switch (move[2])
            {
            case 'a':
                strippedString += '0';
                break;
            case 'b':
                strippedString += '1';
                break;
            case 'c':
                strippedString += '2';
                break;
            case 'd':
                strippedString += '3';
                break;
            case 'e':
                strippedString += '4';
                break;
            case 'f':
                strippedString += '5';
                break;
            case 'g':
                strippedString += '6';
                break;
            case 'h':
                strippedString += '7';
                break;
            }
            strippedString += toupper(move[4]);
            if (board.getWhiteToMove())
            {
                strippedString += 'P';
            }
            else
            {
                strippedString += 'p';
            }
        }
        else if (isEP)
        {
            strippedString += ('0' + convertLetterToFileInt(move[0]));
            strippedString += ('0' + convertLetterToFileInt(move[2]));
            if (board.getWhiteToMove())
            {
                strippedString += "WE";
            }
            else
            {
                strippedString += "BE";
            }
        }
        else
        {
            switch (move[1])
            {
            case '1':
                strippedString += '7';
                break;
            case '2':
                strippedString += '6';
                break;
            case '3':
                strippedString += '5';
                break;
            case '4':
                strippedString += '4';
                break;
            case '5':
                strippedString += '3';
                break;
            case '6':
                strippedString += '2';
                break;
            case '7':
                strippedString += '1';
                break;
            case '8':
                strippedString += '0';
                break;
            }
            switch (move[0])
            {
            case 'a':
                strippedString += '0';
                break;
            case 'b':
                strippedString += '1';
                break;
            case 'c':
                strippedString += '2';
                break;
            case 'd':
                strippedString += '3';
                break;
            case 'e':
                strippedString += '4';
                break;
            case 'f':
                strippedString += '5';
                break;
            case 'g':
                strippedString += '6';
                break;
            case 'h':
                strippedString += '7';
                break;
            }
            switch (move[3])
            {
            case '1':
                strippedString += '7';
                break;
            case '2':
                strippedString += '6';
                break;
            case '3':
                strippedString += '5';
                break;
            case '4':
                strippedString += '4';
                break;
            case '5':
                strippedString += '3';
                break;
            case '6':
                strippedString += '2';
                break;
            case '7':
                strippedString += '1';
                break;
            case '8':
                strippedString += '0';
                break;
            }
            switch (move[2])
            {
            case 'a':
                strippedString += '0';
                break;
            case 'b':
                strippedString += '1';
                break;
            case 'c':
                strippedString += '2';
                break;
            case 'd':
                strippedString += '3';
                break;
            case 'e':
                strippedString += '4';
                break;
            case 'f':
                strippedString += '5';
                break;
            case 'g':
                strippedString += '6';
                break;
            case 'h':
                strippedString += '7';
                break;
            }
        }
        return strippedString;
    }

    int convertLetterToFileInt(char letter)
    {
        switch (letter)
        {
        case 'a':
            return 0;
            break;
        case 'b':
            return 1;
            break;
        case 'c':
            return 2;
            break;
        case 'd':
            return 3;
            break;
        case 'e':
            return 4;
            break;
        case 'f':
            return 5;
            break;
        case 'g':
            return 6;
            break;
        case 'h':
            return 7;
            break;
        default:
            return 0;
            break;
        }
    }

    uint64_t msb(uint64_t bb)
    {
        if (bb == 0)
            return 0;

        uint64_t msb = 0;
        bb = bb / 2;
        while (bb != 0)
        {
            bb = bb / 2;
            msb++;
        }

        return (1ULL << msb);
    }

    uint64_t lsb(uint64_t bb) {
        return ( bb & ~(bb-1) );
    }
};

#endif // MOVES_H_INCLUDED
