#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

#include "moves.h"
#include "board.h"
#include "search.h"

void logInput(std::ostream &ucilog, std::string input)
{
    ucilog << "I " << input << std::endl;
    ucilog.flush();
}

void tellUCI(std::ostream &ucilog, std::string output)
{
    std::cout << output;
    ucilog << "O " << output;
    ucilog.flush();
}

vector<string> split(string s)
{
    std::stringstream ss(s);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vstrings(begin, end);
    return vstrings;
}

int main()
{
    std::ofstream ucilog;
    ucilog.open("uci.log");

    std::string command; // The line of command read
    std::string token;   // A word from command

    Moves &moves = Moves::getInstance();

    Board board = Board::initiateStandardChess();

    while (1)
    {
        // 0. INITIALIZE
        token.clear();

        // 1. GET INPUT FROM UCI
        std::getline(std::cin, command);
        logInput(ucilog, command);
        std::istringstream is(command);
        is >> std::skipws >> token;

        // 2. ACT BASED ON INPUT
        if (token == "quit")
        {
            break;
        }
        else if (token == "uci")
        {
            tellUCI(ucilog, "id name Maestro\n");
            tellUCI(ucilog, "id author dvdutch\n");
            tellUCI(ucilog, "uciok\n");
            // tellUCI("copyprotection ok\n");
        }
        else if (token == "isready")
        {
            tellUCI(ucilog, "readyok\n");
        }
        else if (token == "go")
        {
            // logInput(ucilog, "Board before engine move: ");
            // logInput(ucilog, board.fen());
            // string selectedMove = Search::alphabeta(board, 5, INT_MIN, INT_MAX, "", "").first;
            // tellUCI(ucilog, "bestmove " + moves.convertMoveToAlgebraUCI(selectedMove, board) + "\n");
            // tellUCI(ucilog, "222222222222222222");
            // board = moves.makeMoveAll(board, selectedMove);
            // logInput(ucilog, "Board after engine move: ");
            // logInput(ucilog, board.fen());
        }
        else if (token == "ucinewgame")
        {
            board = Board::initiateStandardChess();
        }
        else if (token == "position")
        {

            vector<string> arguments = split(command);
            string finalArgument = arguments.at(arguments.size() - 1);
            if (finalArgument == "startpos")
            {
                string selectedMove = Search::alphabeta(board, 5, INT_MIN, INT_MAX, "", "").first;
                tellUCI(ucilog, "bestmove " + moves.convertMoveToAlgebraUCI(selectedMove, board) + "\n");
                board = moves.makeMoveAll(board, selectedMove);
            }
            else
            {
                board = moves.makeMoveAll(board, moves.convertMoveToMaestroIntString(finalArgument, board));
                string selectedMove = Search::alphabeta(board, 5, INT_MIN, INT_MAX, "", "").first;
                tellUCI(ucilog, "bestmove " + moves.convertMoveToAlgebraUCI(selectedMove, board) + "\n");
                board = moves.makeMoveAll(board, selectedMove);
            }
        }

        // 3. UNIMPLEMENTED TOKENS
        else if (token == "debug")
        {
        }
        else if (token == "setoption")
        {
        }
        else if (token == "register")
        {
        }
        else if (token == "stop")
        {
        }
        else if (token == "ponderhit")
        {
        }

        // 4. DEBUGGING TOKENS (NOT FOR UCI)
        else if (token == "printfen")
        {
            std::cout << board.fen() << std::endl;
        }
    }

    return 0;
}