#include "board.h"

Board Board::initiateStandardChess()
{
    return Board(71776119061217280U, 4755801206503243776U, 2594073385365405696U, 9295429630892703744U, 576460752303423488U, 1152921504606846976U, 65280U, 66U, 36U, 129U, 8U, 16U, 0ULL, 0, 1, true, true, true, true, true);
}

Board Board::from(string FEN)
{
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
    int tHalfMoveClock = 0;
    int tMoveNumber = 0;
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
        charIndex += 3;
    }
    else
    {
        charIndex += 2;
    }
    tHalfMoveClock = int(FEN[charIndex] - 48);

    charIndex += 2;
    tMoveNumber = int(FEN[charIndex] - 48);

    return Board(tWP, tWN, tWB, tWR, tWQ, tWK, tBP, tBN, tBB, tBR, tBQ, tBK, tEP, tHalfMoveClock, tMoveNumber, tWhiteToMove, tCWK, tCWQ, tCBK, tCBQ);
}

uint64_t Board::getWP()
{
    return WP;
}

uint64_t Board::getWN()
{
    return WN;
}

uint64_t Board::getWB()
{
    return WB;
}

uint64_t Board::getWR()
{
    return WR;
}

uint64_t Board::getWQ()
{
    return WQ;
}

uint64_t Board::getWK()
{
    return WK;
}

uint64_t Board::getBP()
{
    return BP;
}

uint64_t Board::getBN()
{
    return BN;
}

uint64_t Board::getBB()
{
    return BB;
}

uint64_t Board::getBR()
{
    return BR;
}

uint64_t Board::getBQ()
{
    return BQ;
}

uint64_t Board::getBK()
{
    return BK;
}

uint64_t Board::getEP()
{
    return EP;
}

int Board::getHalfMoveClock()
{
    return halfMoveClock;
}

int Board::getMoveNumber()
{
    return moveNumber;
}

bool Board::getWhiteToMove()
{
    return whiteToMove;
}

bool Board::getCWK()
{
    return CWK;
}

bool Board::getCWQ()
{
    return CWQ;
}

bool Board::getCBK()
{
    return CBK;
}

bool Board::getCBQ()
{
    return CBQ;
}

Phase Board::getPhase()
{
    return phase;
}

uint64_t Board::whitePieces()
{
    return WP | WN | WB | WR | WQ | WK;
}

uint64_t Board::blackPieces()
{
    return BP | BN | BB | BR | BQ | BK;
}

uint64_t Board::occupied()
{
    return whitePieces() + blackPieces();
}

uint64_t Board::empty()
{
    return ~(WP | WN | WB | WR | WQ | WK | BP | BN | BB | BR | BQ | BK);
}

string Board::boardString()
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

string Board::fen()
{
    std::string FEN;
    int emptySquareCount = 0;

    for (int i = 0; i < 64; i++)
    {
        uint64_t currentSquareBitboard = 1ULL << i;
        if ((occupied() & currentSquareBitboard) == 0)
        {
            emptySquareCount++;
            if (i % 8 == 7)
            {
                FEN += char(emptySquareCount + 48);
                emptySquareCount = 0;
            }
        }
        else
        {
            if (emptySquareCount != 0)
            {
                FEN += char(emptySquareCount + 48);
                emptySquareCount = 0;
            }
        }

        if ((currentSquareBitboard & WP) != 0)
        {
            FEN += 'P';
        }
        else if ((currentSquareBitboard & WN) != 0)
        {
            FEN += 'N';
        }
        else if ((currentSquareBitboard & WB) != 0)
        {
            FEN += 'B';
        }
        else if ((currentSquareBitboard & WR) != 0)
        {
            FEN += 'R';
        }
        else if ((currentSquareBitboard & WQ) != 0)
        {
            FEN += 'Q';
        }
        else if ((currentSquareBitboard & WK) != 0)
        {
            FEN += 'K';
        }
        else if ((currentSquareBitboard & BP) != 0)
        {
            FEN += 'p';
        }
        else if ((currentSquareBitboard & BN) != 0)
        {
            FEN += 'n';
        }
        else if ((currentSquareBitboard & BB) != 0)
        {
            FEN += 'b';
        }
        else if ((currentSquareBitboard & BR) != 0)
        {
            FEN += 'r';
        }
        else if ((currentSquareBitboard & BQ) != 0)
        {
            FEN += 'q';
        }
        else if ((currentSquareBitboard & BK) != 0)
        {
            FEN += 'k';
        }

        if (i % 8 == 7 && i != 63)
        {
            FEN += '/';
        }
    }

    FEN += ' ';
    if (whiteToMove)
    {
        FEN += 'w';
    }
    else
    {
        FEN += 'b';
    }

    FEN += ' ';
    //  no castling available
    if (!(CWK || CWQ || CBK || CBQ))
    {
        FEN += '-';
    }
    else
    {
        if (CWK)
        {
            FEN += 'K';
        }
        if (CWQ)
        {
            FEN += 'Q';
        }
        if (CBK)
        {
            FEN += 'k';
        }
        if (CBQ)
        {
            FEN += 'q';
        }
    }

    FEN += ' ';
    if (EP != 0)
    {
        uint64_t EPTargetSquareBitboard;
        if (whiteToMove)
        {
            EPTargetSquareBitboard = EP & rank5;
        }
        else
        {
            EPTargetSquareBitboard = EP & rank4;
        }
        int EPTargetSquare = log(EPTargetSquareBitboard) / log(2);
        int rank = EPTargetSquare % 8;
        int file = EPTargetSquare / 8;
        switch (rank)
        {
        case 0:
            FEN += 'a';
            break;
        case 1:
            FEN += 'b';
            break;
        case 2:
            FEN += 'c';
            break;
        case 3:
            FEN += 'd';
            break;
        case 4:
            FEN += 'e';
            break;
        case 5:
            FEN += 'f';
            break;
        case 6:
            FEN += 'g';
            break;
        case 7:
            FEN += 'h';
            break;
        }

        whiteToMove ? FEN += '6' : FEN += '4';
    }
    else
    {
        FEN += '-';
    }

    FEN += ' ';
    FEN += std::to_string(halfMoveClock);
    FEN += ' ';
    FEN += std::to_string(moveNumber);

    return FEN;
}

Phase Board::calculatePhase()
{
    std::bitset<64> WQBits = WQ;
    std::bitset<64> BQBits = BQ;
    std::bitset<64> WNBits = WN;
    std::bitset<64> WBBits = WB;
    std::bitset<64> WRBits = WR;
    std::bitset<64> BNBits = BN;
    std::bitset<64> BBBits = BB;
    std::bitset<64> BRBits = BR;
    Phase phase;
    int minorAndRookCount = WNBits.count() + WBBits.count() + WRBits.count() + BNBits.count() + BBBits.count() + BRBits.count();
    (WQBits.count() == 0 && BQBits.count() == 0 && minorAndRookCount < 7) || (WQBits.count() + BQBits.count() == 1 && minorAndRookCount < 5) || (WQBits.count() + BQBits.count() == 2 && minorAndRookCount < 3) ? phase = End : phase = OpeningMiddle;
    return phase;
}