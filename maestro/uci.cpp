#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
            board = Board::initiateStandardChess();
            tellUCI(ucilog, "readyok\n");
        }
        else if (token == "go")
        {
            logInput(ucilog, "Board before engine move: ");
            logInput(ucilog, board.fen());
            string selectedMove = Search::alphabeta(board, 5, INT_MIN, INT_MAX, "", "").first;
            tellUCI(ucilog, "bestmove " + moves.convertMoveToAlgebraUCI(selectedMove, board) + "\n");
            board = moves.makeMoveAll(board, selectedMove);
            logInput(ucilog, "Board after engine move: ");
            logInput(ucilog, board.fen());
        }
        else if (token == "ucinewgame")
        {
            //board = Board::initiateStandardChess();
        }
        else if (token == "position")
        {
            std::string option;
            is >> std::skipws >> option;

            logInput(ucilog, "Board before player move: ");
            logInput(ucilog, board.fen());
            string lastMoveString;
            if (command[command.length() - 5] != ' ') {
                lastMoveString += command[command.length() - 5];
            }
            lastMoveString += command[command.length() - 4];
            lastMoveString += command[command.length() - 3];
            lastMoveString += command[command.length() - 2];
            lastMoveString += command[command.length() - 1];
            board = moves.makeMoveAll(board, moves.convertMoveToMaestroIntString(lastMoveString, board));
            logInput(ucilog, "Board after player move: ");
            logInput(ucilog, board.fen());
            // Find Starting position
            if (option == "startpos")
            {
                //board = moves.makeMoveAll(board, lastMoveString);
            }
            else if (option == "fen")
            { // FEN is given
                board.from(option);
            }
            else
            {
                return 0;
            }

            // Check if there are any moves given
            is >> std::skipws >> option;
            if (option == "moves")
            {
                cout << option << endl;
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