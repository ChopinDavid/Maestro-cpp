#ifndef RATING_H_INCLUDED
#define RATING_H_INCLUDED
#include <cmath>
#include <string>
#include <stdint.h>
#include <ctype.h>
#include "pieceSquareTables.h"
#include "bitboard.h"
#include "board.h"
using namespace std;

class Rating
{
private:
    Rating() {}

public:
    static Rating &getInstance()
    {
        static Rating shared;
        return shared;
    }

    static int evaluate()
    {
        return 0;
    }

    static int getCentipawnValueWhite(Board board)
    {
        int centipawnValue = 0;
        string pawnString = convertBitboardToStringRep(board.getWP());
        for (int i = 0; i < 64; i++)
        {
            char character = pawnString[i];
            if (character == '1')
            {
                centipawnValue += 100;
                centipawnValue += whitePawnSquareTable[i];
            }
        }

        string knightString = convertBitboardToStringRep(board.getWN());
        for (int i = 0; i < 64; i++)
        {
            char character = knightString[i];
            if (character == '1')
            {
                centipawnValue += 416;
                centipawnValue += whiteKnightSquareTable[i];
            }
        }

        string bishopString = convertBitboardToStringRep(board.getWB());
        for (int i = 0; i < 64; i++)
        {
            char character = bishopString[i];
            if (character == '1')
            {
                centipawnValue += 441;
                centipawnValue += whiteBishopSquareTable[i];
            }
        }

        string rookString = convertBitboardToStringRep(board.getWR());
        for (int i = 0; i < 64; i++)
        {
            char character = rookString[i];
            if (character == '1')
            {
                centipawnValue += 663;
                centipawnValue += whiteRookSquareTable[i];
            }
        }

        string queenString = convertBitboardToStringRep(board.getWQ());
        for (int i = 0; i < 64; i++)
        {
            char character = queenString[i];
            if (character == '1')
            {
                centipawnValue += 1292;
                centipawnValue += whiteQueenSquareTable[i];
            }
        }

        string kingString = convertBitboardToStringRep(board.getWK());
        for (int i = 0; i < 64; i++)
        {
            char character = kingString[i];
            if (character == '1')
            {
                if (board.getPhase() == OpeningMiddle) {
                    centipawnValue += whiteKingMiddleSquareTable[i];
                } else {
                    centipawnValue += whiteKingEndSquareTable[i];
                }
            }
        }

        return centipawnValue;
    }

    static int getCentipawnValueBlack(Board board)
    {
        int centipawnValue = 0;
        string pawnString = convertBitboardToStringRep(board.getBP());
        for (int i = 0; i < 64; i++)
        {
            char character = pawnString[i];
            if (character == '1')
            {
                centipawnValue -= 100;
                centipawnValue -= blackPawnSquareTable[i];
            }
        }

        string knightString = convertBitboardToStringRep(board.getBN());
        for (int i = 0; i < 64; i++)
        {
            char character = knightString[i];
            if (character == '1')
            {
                centipawnValue -= 416;
                centipawnValue -= blackKnightSquareTable[i];
            }
        }

        string bishopString = convertBitboardToStringRep(board.getBB());
        for (int i = 0; i < 64; i++)
        {
            char character = bishopString[i];
            if (character == '1')
            {
                centipawnValue -= 441;
                centipawnValue -= blackBishopSquareTable[i];
            }
        }

        string rookString = convertBitboardToStringRep(board.getBR());
        for (int i = 0; i < 64; i++)
        {
            char character = rookString[i];
            if (character == '1')
            {
                centipawnValue -= 663;
                centipawnValue -= blackRookSquareTable[i];
            }
        }

        string queenString = convertBitboardToStringRep(board.getBQ());
        for (int i = 0; i < 64; i++)
        {
            char character = queenString[i];
            if (character == '1')
            {
                centipawnValue -= 1292;
                centipawnValue -= blackQueenSquareTable[i];
            }
        }

        return centipawnValue;
    }

    static int getCentipawnValue(Board board)
    {
        int centipawnValue = getCentipawnValueWhite(board) + getCentipawnValueBlack(board);
        return centipawnValue;
    }
};
#endif // RATING_H_INCLUDED