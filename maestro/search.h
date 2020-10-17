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
#include <chrono>
using namespace std;
using namespace std::chrono;
class Search
{
public:
    static std::pair<string, int> minimax(Board node, int depth, int alpha, int beta, string move, string firstMove, milliseconds breakOffTime)
    {
        if (breakOffTime != std::chrono::milliseconds(0))
        {
            milliseconds currentTime = duration_cast<milliseconds>(
                system_clock::now().time_since_epoch());
            if (currentTime > breakOffTime)
            {
                return std::make_pair("", 0);
            }
        }

        Moves &moves = Moves::getInstance();
        if (moves.isCheckmate(node))
        {
            if (node.getWhiteToMove())
            {
                return std::make_pair(firstMove, INT_MIN);
            }
            else
            {
                return std::make_pair(firstMove, INT_MAX);
            }
        }
        if (depth == 0)
        {
            return std::make_pair(firstMove, Rating::getCentipawnValue(node));
        }
        if (node.getWhiteToMove())
        {
            string bestMove = firstMove;
            int bestValue = INT_MIN;
            string pseudoLegalMoves = moves.pseudoLegalMovesW(node);
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
                if ((moves.unsafeForWhite(tNode) & tNode.getWK()) == 0)
                {
                    std::pair<string, int> ab;
                    if (firstMove == "")
                    {
                        ab = minimax(tNode, depth - 1, alpha, beta, individualMoveString, individualMoveString, breakOffTime);
                    }
                    else
                    {
                        ab = minimax(tNode, depth - 1, alpha, beta, individualMoveString, firstMove, breakOffTime);
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
                        ab = minimax(tNode, depth - 1, alpha, beta, individualMoveString, individualMoveString, breakOffTime);
                    }
                    else
                    {
                        ab = minimax(tNode, depth - 1, alpha, beta, individualMoveString, firstMove, breakOffTime);
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
            return std::make_pair(bestMove, bestValue);
        }
    }

    static std::pair<string, int> iterativeDeepeningSearch(Board initialPosition, int maxDepth, milliseconds maxSearchTime)
    {
        std::pair<string, int> bestMove;
        milliseconds startTime = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch());

        for (int currentDepth = 1; currentDepth <= maxDepth; currentDepth++)
        {

            milliseconds currentTime = duration_cast<milliseconds>(
                system_clock::now().time_since_epoch());
            if (currentTime > startTime + maxSearchTime)
            {
                return bestMove;
            }
            std::pair<string, int> bestMoveFoundFromMinimax = minimax(initialPosition, currentDepth, INT_MIN, INT_MAX, "", "", startTime + maxSearchTime);
            if (bestMoveFoundFromMinimax.first != "") {
                bestMove = bestMoveFoundFromMinimax;
            }
        }
        return bestMove;
    }
};
#endif // SEARCH_H_INCLUDED