#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED
#include <cmath>
#include <string>
#include <stdint.h>
#include <ctype.h>
#include "bitboard.h"
#include "board.h"
#include "rating.h"
#include "moves.h"
using namespace std;
class Search
{
public:
    static std::pair<string, int> alphabeta(Board node, int depth, int alpha, int beta, string move, string firstMove)
    {
        Moves &moves = Moves::getInstance();
        if (moves.isCheckmate(node)) {
            if (node.getWhiteToMove()) {
                return std::make_pair(firstMove, INT_MIN);
            } else {
                return std::make_pair(firstMove, INT_MAX);
            }
        }
        if (depth == 0)
        {
            return std::make_pair(firstMove, Rating::getCentipawnValue(node));
        }
        if (node.getWhiteToMove())
        {
            string bestMove;
            int value = INT_MIN;
            string pseudoLegalMoves = moves.pseudoLegalMovesW(node);
            if (pseudoLegalMoves.length() == 0) {
                return std::make_pair(firstMove, 0);
            }
            for (int i = 0; i < pseudoLegalMoves.length(); i += 4)
            {
                string individualMoveString;
                individualMoveString += pseudoLegalMoves[i];
                individualMoveString += pseudoLegalMoves[i + 1];
                individualMoveString += pseudoLegalMoves[i + 2];
                individualMoveString += pseudoLegalMoves[i + 3];
                Board tNode = moves.makeMoveAll(node, individualMoveString);
                if ((moves.unsafeForWhite(tNode) & tNode.getWK()) == 0)
                {
                    std::pair<string, int> ab;
                    if (firstMove == "")
                    {
                        ab = alphabeta(tNode, depth - 1, alpha, beta, individualMoveString, individualMoveString);
                    }
                    else
                    {
                        ab = alphabeta(tNode, depth - 1, alpha, beta, individualMoveString, firstMove);
                    }
                    if (ab.second > value)
                    {
                        value = ab.second;
                        bestMove = ab.first;
                    }
                    alpha = max(alpha, value);
                    if (alpha >= beta)
                    {
                        break;
                    }
                }
            }
            return std::make_pair(bestMove, value);
        }
        else
        {
            string bestMove;
            int value = INT_MAX;
            string pseudoLegalMoves = moves.pseudoLegalMovesB(node);
            if (pseudoLegalMoves.length() == 0) {
                return std::make_pair(firstMove, 0);
            }
            for (int i = 0; i < pseudoLegalMoves.length(); i += 4)
            {
                string individualMoveString;
                individualMoveString += pseudoLegalMoves[i];
                individualMoveString += pseudoLegalMoves[i + 1];
                individualMoveString += pseudoLegalMoves[i + 2];
                individualMoveString += pseudoLegalMoves[i + 3];
                Board tNode = moves.makeMoveAll(node, individualMoveString);
                if ((moves.unsafeForBlack(tNode) & tNode.getBK()) == 0)
                {
                    std::pair<string, int> ab;
                    if (firstMove == "")
                    {
                        ab = alphabeta(tNode, depth - 1, alpha, beta, individualMoveString, individualMoveString);
                    }
                    else
                    {
                        ab = alphabeta(tNode, depth - 1, alpha, beta, individualMoveString, firstMove);
                    }
                    if (ab.second < value)
                    {
                        value = ab.second;
                        bestMove = ab.first;
                    }
                    beta = min(beta, value);
                    if (alpha >= beta)
                    {
                        break;
                    }
                }
            }
            return std::make_pair(bestMove, value);
        }
    }
};
#endif // SEARCH_H_INCLUDED