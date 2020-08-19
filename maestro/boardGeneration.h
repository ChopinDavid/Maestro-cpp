#ifndef BOARDGENERATION_H_INCLUDED
#define BOARDGENERATION_H_INCLUDED
#include <cmath>
#include <string>
#include <stdint.h>
#include <ctype.h>
#include "bitboard.h"
using namespace std;

class BoardGeneration
{
private:
    BoardGeneration() {}

public:
    static BoardGeneration &getInstance()
    {
        static BoardGeneration shared;
        return shared;
    }

    uint64_t WP = 0;
    uint64_t WN = 0;
    uint64_t WB = 0;
    uint64_t WR = 0;
    uint64_t WQ = 0;
    uint64_t WK = 0;
    uint64_t BP = 0;
    uint64_t BN = 0;
    uint64_t BB = 0;
    uint64_t BR = 0;
    uint64_t BQ = 0;
    uint64_t BK = 0;
    uint64_t EP = 0;

    uint64_t whitePieces()
    {
        return WP | WN | WB | WR | WQ | WK;
    }

    uint64_t blackPieces()
    {
        return BP | BN | BB | BR | BQ | BK;
    }

    uint64_t occupied()
    {
        return whitePieces() + blackPieces();
    }

    uint64_t empty()
    {
        return ~(WP | WN | WB | WR | WQ | WK | BP | BN | BB | BR | BQ | BK);
    }

    bool whiteToMove = true;
    bool CWK = true;
    bool CWQ = true;
    bool CBK = true;
    bool CBQ = true;

    void initiateStandardChess()
    {
        char board[8][8] = {{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}, {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'}, {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};
        arrayToBitboards(board);
    }

    void arrayToBitboards(char chessBoard[8][8])
    {

        for (int i = 0; i < 64; i++)
        {
            switch (chessBoard[i / 8][i % 8])
            {
            case 'P':
                WP += 1ULL << i;
                break;
            case 'N':
                WN += 1ULL << i;
                break;
            case 'B':
                WB += 1ULL << i;
                break;
            case 'R':
                WR += 1ULL << i;
                break;
            case 'Q':
                WQ += 1ULL << i;
                break;
            case 'K':
                WK += 1ULL << i;
                break;
            case 'p':
                BP += 1ULL << i;
                break;
            case 'n':
                BN += 1ULL << i;
                break;
            case 'b':
                BB += 1ULL << i;
                break;
            case 'r':
                BR += 1ULL << i;
                break;
            case 'q':
                BQ += 1ULL << i;
                break;
            case 'k':
                BK += 1ULL << i;
                break;
            }
        }
    }

    void importFEN(string fenString)
    {
        WP = 0;
        WN = 0;
        WB = 0;
        WR = 0;
        WQ = 0;
        WK = 0;
        BP = 0;
        BN = 0;
        BB = 0;
        BR = 0;
        BQ = 0;
        BK = 0;

        CWK = false;
        CWQ = false;
        CBK = false;
        CBQ = false;

        int boardIndex = 0;
        int charIndex = 0;

        while (fenString[charIndex] != ' ')
        {
            switch (fenString[charIndex])
            {
            case 'P':
                WP |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'N':
                WN |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'B':
                WB |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'R':
                WR |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'Q':
                WQ |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'K':
                WK |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'p':
                BP |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'n':
                BN |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'b':
                BB |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'r':
                BR |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'q':
                BQ |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'k':
                BK |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case '/':
                break;
            case '1':
                boardIndex += 1;
                break;
            case '2':
                boardIndex += 2;
                break;
            case '3':
                boardIndex += 3;
                break;
            case '4':
                boardIndex += 4;
                break;
            case '5':
                boardIndex += 5;
                break;
            case '6':
                boardIndex += 6;
                break;
            case '7':
                boardIndex += 7;
                break;
            case '8':
                boardIndex += 8;
                break;
            default:
                break;
            }
            charIndex++;
        }
        charIndex++;
        whiteToMove = fenString[charIndex] == 'w';

        charIndex += 2;

        while (fenString[charIndex] != ' ')
        {
            switch (fenString[charIndex])
            {
            case '-':
                break;
            case 'K':
                CWK = true;
                break;
            case 'Q':
                CWQ = true;
                break;
            case 'k':
                CBK = true;
                break;
            case 'q':
                CBQ = true;
                break;
            }
            charIndex++;
        }
        charIndex++;
        if (fenString[charIndex] != '-')
        {
            EP = fileMasks8[int(tolower(fenString[charIndex])) - 97];
        }
    }

    string boardString()
    {
        string boardString;
        char chessBoard[8][8] = {{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

        for (int i = 0; i < 64; i++)
        {
            if (((WP >> i) & 1) == 1)
            {
                chessBoard[i / 8][i % 8] = 'P';
            }
            if (((WN >> i) & 1) == 1)
            {
                chessBoard[i / 8][i % 8] = 'N';
            }
            if (((WB >> i) & 1) == 1)
            {
                chessBoard[i / 8][i % 8] = 'B';
            }
            if (((WR >> i) & 1) == 1)
            {
                chessBoard[i / 8][i % 8] = 'R';
            }
            if (((WQ >> i) & 1) == 1)
            {
                chessBoard[i / 8][i % 8] = 'Q';
            }
            if (((WK >> i) & 1) == 1)
            {
                chessBoard[i / 8][i % 8] = 'K';
            }

            if (((BP >> i) & 1) == 1)
            {
                chessBoard[i / 8][i % 8] = 'p';
            }
            if (((BN >> i) & 1) == 1)
            {
                chessBoard[i / 8][i % 8] = 'n';
            }
            if (((BB >> i) & 1) == 1)
            {
                chessBoard[i / 8][i % 8] = 'b';
            }
            if (((BR >> i) & 1) == 1)
            {
                chessBoard[i / 8][i % 8] = 'r';
            }
            if (((BQ >> i) & 1) == 1)
            {
                chessBoard[i / 8][i % 8] = 'q';
            }
            if (((BK >> i) & 1) == 1)
            {
                chessBoard[i / 8][i % 8] = 'k';
            }
        }

        for (int i = 0; i < 8; i++)
        {
            string rankString;
            for (int j = 0; j < 8; j++)
            {
                rankString += "[ ";
                rankString += chessBoard[i][j];
                rankString += " ]";
            }

            boardString += rankString;
            if (i != 7)
            {
                boardString += "\n";
            }
        }

        return boardString;
    }
};

#endif // BOARDGENERATION_H_INCLUDED
