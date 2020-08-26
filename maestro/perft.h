#ifndef PERFT_H_INCLUDED
#define PERFT_H_INCLUDED

#include "moves.h"
#include "board.h"
#include <string>
#include <stdint.h>

class Perft
{

private:
    Perft() {}

public:
    static Perft &getInstance()
    {
        static Perft shared;
        return shared;
    }

    Moves &moves = Moves::getInstance();

    // string moveToAlgebra(string move, bool whiteToMove)
    // {
    //     string moveString = "";
    //     if (move[3] == 'P')
    //     {
    //         //Pawn promotion
    //         if (whiteToMove)
    //         {
    //             moveString = move[0].intToAlphabeticalValue() + '7' + move[1].intToAlphabeticalValue() + '8';
    //         }
    //         else
    //         {
    //             moveString = move[0].intToAlphabeticalValue() + '2' + move[1].intToAlphabeticalValue() + '1';
    //         }
    //     }
    //     else if (move[2] == 'W')
    //     {
    //         //en passant white
    //         moveString = move[0].intToAlphabeticalValue() + '5' + move[1].intToAlphabeticalValue() + '6';
    //     }
    //     else if (move[2] == 'B')
    //     {
    //         //en passant black
    //         moveString = move[0].intToAlphabeticalValue() + '4' + move[1].intToAlphabeticalValue() + '3';
    //     }
    //     else
    //     {
    //         //"normal" move
    //         moveString += move[1].intToAlphabeticalValue();
    //         moveString += int(move[0] - 40);
    //         moveString += move[3].intToAlphabeticalValue();
    //         moveString += int(move[2] - 40);
    //     }

    //     return moveString
    // }

    int perftTotalMoveCounter = 0;
    int perftMoveCounter = 0;
    int perftMaxDepth = 5;

    uint64_t perft(Board board, int depth)
    {
        string possibleMoves;
        if (board.getWhiteToMove())
        {
            possibleMoves = moves.pseudoLegalMovesW(board);
        }
        else
        {
            possibleMoves = moves.pseudoLegalMovesB(board);
        }

        uint64_t nodes = 0;

        if (depth == 0)
        {
            return 1ULL;
        }

        for (int i = 0; i < possibleMoves.length(); i += 4)
        {
            string singleMoveString = "";
            for (int j = 0; j < 4; j++)
            {
                singleMoveString += possibleMoves[i + j];
            }
            Board tBoard = moves.makeMoveAll(board, singleMoveString);
            uint64_t unsafe = moves.unsafeForBlack(tBoard);
            if (!((!tBoard.getWhiteToMove() && (moves.unsafeForWhite(tBoard) & tBoard.getWK()) != 0) || (tBoard.getWhiteToMove() && (moves.unsafeForBlack(tBoard) & tBoard.getBK()) != 0)))
            {
                nodes += perft(tBoard, depth - 1);
            } else {
                cout << "aa" << endl;
            }
        }
        return nodes;
    }
};

#endif // PERFT_H_INCLUDED