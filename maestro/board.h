#ifndef BOARDGENERATION_H_INCLUDED
#define BOARDGENERATION_H_INCLUDED
#include <cmath>
#include <string>
#include <stdint.h>
#include <ctype.h>
#include "bitboard.h"
using namespace std;

class Board
{
private:
    uint64_t WP;
    uint64_t WN;
    uint64_t WB;
    uint64_t WR;
    uint64_t WQ;
    uint64_t WK;
    uint64_t BP;
    uint64_t BN;
    uint64_t BB;
    uint64_t BR;
    uint64_t BQ;
    uint64_t BK;
    uint64_t EP;
    bool whiteToMove;
    bool CWK;
    bool CWQ;
    bool CBK;
    bool CBQ;

public:
    Board(uint64_t WP, uint64_t WN, uint64_t WB, uint64_t WR, uint64_t WQ, uint64_t WK, uint64_t BP, uint64_t BN, uint64_t BB, uint64_t BR, uint64_t BQ, uint64_t BK, uint64_t EP, bool whiteToMove, bool CWK, bool CWQ, bool CBK, bool CBQ) : WP(WP), WN(WN), WB(WB), WR(WR), WQ(WQ), WK(WK), BP(BP), BN(BN), BB(BB), BR(BR), BQ(BQ), BK(BK), EP(EP), whiteToMove(whiteToMove), CWK(CWK), CWQ(CWQ), CBK(CBK), CBQ(CBQ) {};

    static Board initiateStandardChess()
    {
        return Board(71776119061217280U, 4755801206503243776U, 2594073385365405696U, 9295429630892703744U, 576460752303423488U, 1152921504606846976U, 65280U, 66U, 36U, 129U, 8U, 16U, 0ULL, true, true, true, true, true);
    }
    static Board from(string FEN) {
        uint64_t tWP = 0;
        uint64_t tWN = 0;
        uint64_t tWB = 0;
        uint64_t tWR = 0;
        uint64_t tWQ = 0;
        uint64_t tWK = 0;
        uint64_t tBP = 0;
        uint64_t tBN = 0;
        uint64_t tBB = 0;
        uint64_t tBR = 0;
        uint64_t tBQ = 0;
        uint64_t tBK = 0;
        uint64_t tEP = 0;

        bool tWhiteToMove = false;
        bool tCWK = false;
        bool tCWQ = false;
        bool tCBK = false;
        bool tCBQ = false;

        int boardIndex = 0;
        int charIndex = 0;

        while (FEN[charIndex] != ' ')
        {
            switch (FEN[charIndex])
            {
            case 'P':
                tWP |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'N':
                tWN |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'B':
                tWB |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'R':
                tWR |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'Q':
                tWQ |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'K':
                tWK |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'p':
                tBP |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'n':
                tBN |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'b':
                tBB |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'r':
                tBR |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'q':
                tBQ |= (1ULL << boardIndex);
                boardIndex++;
                break;
            case 'k':
                tBK |= (1ULL << boardIndex);
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
        tWhiteToMove = FEN[charIndex] == 'w';

        charIndex += 2;

        while (FEN[charIndex] != ' ')
        {
            switch (FEN[charIndex])
            {
            case '-':
                break;
            case 'K':
                tCWK = true;
                break;
            case 'Q':
                tCWQ = true;
                break;
            case 'k':
                tCBK = true;
                break;
            case 'q':
                tCBQ = true;
                break;
            }
            charIndex++;
        }
        charIndex++;
        if (FEN[charIndex] != '-')
        {
            tEP = fileMasks8[int(tolower(FEN[charIndex])) - 97];
        }

        return Board(tWP, tWN, tWB, tWR, tWQ, tWK, tBP, tBN, tBB, tBR, tBQ, tBK, tEP, tWhiteToMove, tCWK, tCWQ, tCBK, tCBQ);
    }

    uint64_t getWP() {
        return WP;
    }

    uint64_t getWN() {
        return WN;
    }

    uint64_t getWB() {
        return WB;
    }

    uint64_t getWR() {
        return WR;
    }

    uint64_t getWQ() {
        return WQ;
    }

    uint64_t getWK() {
        return WK;
    }

    uint64_t getBP() {
        return BP;
    }

    uint64_t getBN() {
        return BN;
    }

    uint64_t getBB() {
        return BB;
    }

    uint64_t getBR() {
        return BR;
    }

    uint64_t getBQ() {
        return BQ;
    }

    uint64_t getBK() {
        return BK;
    }

    uint64_t getEP() {
        return EP;
    }

    bool getWhiteToMove() {
        return whiteToMove;
    }

    bool getCWK() {
        return CWK;
    }

    bool getCWQ() {
        return CWQ;
    }

    bool getCBK() {
        return CBK;
    }

    bool getCBQ() {
        return CBQ;
    }

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
