#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "moves.h"
#include "board.h"

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
            tellUCI(ucilog, "id author David Chopin\n");
            tellUCI(ucilog, "uciok\n");
            // tellUCI("copyprotection ok\n");
        }
        else if (token == "isready")
        {
            tellUCI(ucilog, "readyok\n");
        }
        else if (token == "go")
        {

            // FIXME: Check all options
            // FIXME: Implement PV ordering
            string selectedMove;
            if (board.getWhiteToMove())
            {
                string pseudoLegalMoves = moves.pseudoLegalMovesW(board);
                for (int i = 0; i < pseudoLegalMoves.length(); i += 4)
                {
                    if (selectedMove.empty())
                    {
                        std::string currentMove = pseudoLegalMoves.substr(i, 4);
                        Board tBoard = moves.makeMoveAll(board, currentMove);
                        if ((moves.unsafeForWhite(tBoard) & tBoard.getWK()) == 0)
                        {
                            selectedMove = currentMove;
                            board = tBoard;
                        }
                    }
                }
            } else {
                string pseudoLegalMoves = moves.pseudoLegalMovesB(board);
                for (int i = 0; i < pseudoLegalMoves.length(); i += 4)
                {
                    if (selectedMove.empty())
                    {
                        std::string currentMove = pseudoLegalMoves.substr(i, 4);
                        Board tBoard = moves.makeMoveAll(board, currentMove);
                        if ((moves.unsafeForBlack(tBoard) & tBoard.getBK()) == 0)
                        {
                            selectedMove = currentMove;
                            board = tBoard;
                        }
                    }
                }
            }
            tellUCI(ucilog, "bestmove " + moves.convertMoveToAlgebraUCI(selectedMove, board) + "\n");
        }
        else if (token == "ucinewgame")
        {
            //board = Board::initiateStandardChess();
        }
        else if (token == "position")
        {
            std::string option;
            is >> std::skipws >> option;

            // Find Starting position
            if (option == "startpos")
            {
                //board = Board::initiateStandardChess();
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