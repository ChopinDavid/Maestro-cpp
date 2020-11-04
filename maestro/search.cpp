#include "search.h"
#include "logger.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>

Zobrist Search::zobrist = Zobrist();
std::unordered_map<uint64_t, vector<pair<string, int>>> Search::hashMap = {};

std::vector<std::string> Search::split(const std::string &str, int splitLength)
{
    int NumSubstrings = str.length() / splitLength;
    std::vector<std::string> ret;

    for (auto i = 0; i < NumSubstrings; i++)
    {
        ret.push_back(str.substr(i * splitLength, splitLength));
    }

    // If there are leftover characters, create a shorter item at the end.
    if (str.length() % splitLength != 0)
    {
        ret.push_back(str.substr(splitLength * NumSubstrings));
    }

    return ret;
}

std::pair<string, int> Search::minimax(Board node, int depth, int alpha, int beta, string move, string firstMove, milliseconds breakOffTime)
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
        vector<pair<string, int>> storedMoves = hashMap[zobrist.hash(node)];
        cout << to_string(storedMoves.size()) << endl;
        vector<string> pseudoLegalMoves;
        if (storedMoves.size() != 0)
        {
            for (int i = 0; i < storedMoves.size(); i++)
            {
                pseudoLegalMoves.push_back(storedMoves.at(i).first);
            }
        }
        else
        {
            pseudoLegalMoves = Search::split(moves.pseudoLegalMovesW(node), 4);
        }
        if (pseudoLegalMoves.size() == 0)
        {
            return std::make_pair(firstMove, 0);
        }
        vector<pair<string, int>> movesVector;
        for (int i = 0; i < pseudoLegalMoves.size(); i++)
        {
            cout << pseudoLegalMoves.at(i) << endl;
        }
        for (int i = 0; i < pseudoLegalMoves.size(); i++)
        {
            string individualMoveString = pseudoLegalMoves.at(i);
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
                movesVector.push_back(std::make_pair(individualMoveString, val));
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
        sort(movesVector.begin(), movesVector.end(), [](pair<string, int> a, pair<string, int> b) {
            if (a.second == b.second)
            {
                return (a.first < b.first);
            }
            return (a.second > b.second);
        });
        hashMap[zobrist.hash(node)] = movesVector;
        return std::make_pair(bestMove, bestValue);
    }
    else
    {
        string bestMove = firstMove;
        int bestValue = INT_MAX;
        vector<pair<string, int>> storedMoves = hashMap[zobrist.hash(node)];
        cout << to_string(storedMoves.size()) << endl;
        vector<string> pseudoLegalMoves;
        if (storedMoves.size() != 0)
        {
            for (int i = 0; i < storedMoves.size(); i++)
            {
                pseudoLegalMoves.push_back(storedMoves.at(i).first);
            }
        }
        else
        {
            pseudoLegalMoves = Search::split(moves.pseudoLegalMovesW(node), 4);
        }
        if (pseudoLegalMoves.size() == 0)
        {
            return std::make_pair(firstMove, 0);
        }
        vector<pair<string, int>> movesVector;
        for (int i = 0; i < pseudoLegalMoves.size(); i++)
        {
            cout << pseudoLegalMoves.at(i) << endl;
        }
        for (int i = 0; i < pseudoLegalMoves.size(); i++)
        {
            string individualMoveString = pseudoLegalMoves.at(i);
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
                movesVector.push_back(std::make_pair(individualMoveString, val));
                if (val < bestValue || (val == bestValue && bestMove == ""))
                {
                    bestValue = val;
                    bestMove = move;
                }
                alpha = min(alpha, bestValue);
                if (alpha >= beta)
                {
                    break;
                }
            }
                }
        sort(movesVector.begin(), movesVector.end(), [](pair<string, int> a, pair<string, int> b) {
            if (a.second == b.second)
            {
                return (a.first < b.first);
            }
            return (a.second > b.second);
        });
        hashMap[zobrist.hash(node)] = movesVector;
        return std::make_pair(bestMove, bestValue);
    }
}

std::pair<string, int> Search::iterativeDeepeningSearch(Board initialPosition, int maxDepth, milliseconds maxSearchTime)
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
        if (bestMoveFoundFromMinimax.first != "")
        {
            bestMove = bestMoveFoundFromMinimax;
        }
    }
    return bestMove;
}