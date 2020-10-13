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
        string depthString;
        for (int i = 6; i > depth; i--)
        {
            depthString = depthString + "   ";
        }
        std::ofstream searchlog;
        searchlog.open("search.log", std::ios_base::app);
        searchlog << depthString << "node: " << node.fen() << endl;
        Moves &moves = Moves::getInstance();
        if (moves.isCheckmate(node))
        {
            if (node.getWhiteToMove())
            {
                searchlog << depthString << "Terminal node: white in checkmate" << endl;
                return std::make_pair(firstMove, INT_MIN);
            }
            else
            {
                searchlog << depthString << "Terminal node: black in checkmate" << endl;
                return std::make_pair(firstMove, INT_MAX);
            }
        }
        if (depth == 0)
        {
            searchlog << depthString << "Terminal node: reached max depth" << endl;
            return std::make_pair(firstMove, Rating::getCentipawnValue(node));
        }
        if (node.getWhiteToMove())
        {
            string bestMove = firstMove;
            int bestValue = INT_MIN;
            string pseudoLegalMoves = moves.pseudoLegalMovesW(node);
            if (pseudoLegalMoves.length() == 0)
            {
                searchlog << depthString << "Terminal node: stalemate" << endl;
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
                    int val = ab.second;
                    string move = ab.first;
                    if (val > bestValue || (val == bestValue && bestMove == ""))
                    {
                        bestValue = val;
                        bestMove = move;
                    }
                    alpha = max(alpha, bestValue);
                    if (alpha >= beta)
                    {
                        break;
                    }
                }
            }
            searchlog << depthString << "best move found: " << bestMove << " " << bestValue << endl;
            return std::make_pair(bestMove, bestValue);
        }
        else
        {
            string bestMove = firstMove;
            int bestValue = INT_MAX;
            string pseudoLegalMoves = moves.pseudoLegalMovesB(node);
            if (pseudoLegalMoves.length() == 0)
            {
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
                    int val = ab.second;
                    string move = ab.first;
                    if (val < bestValue || (val == bestValue && bestMove == ""))
                    {
                        bestValue = ab.second;
                        bestMove = ab.first;
                    }
                    beta = min(beta, bestValue);
                    if (alpha >= beta)
                    {
                        break;
                    }
                }
            }
            searchlog << depthString << "best move found: " << bestMove << " " << bestValue << endl;
            return std::make_pair(bestMove, bestValue);
        }
    }
};
#endif // SEARCH_H_INCLUDED