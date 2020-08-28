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
            uint64_t unsafe;
            if (tBoard.getWhiteToMove())
            {
                unsafe = moves.unsafeForWhite(tBoard);
            }
            else
            {
                unsafe = moves.unsafeForBlack(tBoard);
            }

            bool a = tBoard.getWhiteToMove();
            uint64_t b = moves.unsafeForWhite(tBoard);
            uint64_t c = tBoard.getWK();
            uint64_t d = moves.unsafeForBlack(tBoard);
            uint64_t e = tBoard.getBK();
            if (!((!a && (b & c) != 0) || (a && (d & e) != 0)))
            {
                nodes += perft(tBoard, depth - 1);
            }
        }
        return nodes;
    }

    void divide(Board board, int depth)
    {
        string movesString;
        if (board.getWhiteToMove())
        {
            movesString = moves.pseudoLegalMovesW(board);
        }
        else
        {
            movesString = moves.pseudoLegalMovesB(board);
        }

        for (int i = 0; i < movesString.length(); i += 4)
        {
            string moveString = "";
            moveString += movesString[i];
            moveString += movesString[i + 1];
            moveString += movesString[i + 2];
            moveString += movesString[i + 3];
            Board tBoard = moves.makeMoveAll(board, moveString);
            int movesCount = perft(tBoard, depth);
            cout << moves.convertMoveToAlgebra(moveString, board) << " " << movesCount << endl;
        }
    }
};

#endif // PERFT_H_INCLUDED