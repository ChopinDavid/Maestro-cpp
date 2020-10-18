#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED
#include <bitset>
#include <algorithm>
using namespace std;

extern uint64_t fileA;
extern uint64_t fileH;
extern uint64_t filesAB;
extern uint64_t filesGH;
extern uint64_t rank1;
extern uint64_t rank4;
extern uint64_t rank5;
extern uint64_t rank8;
extern uint64_t center;
extern uint64_t extendedCenter;
extern uint64_t kingSide;
extern uint64_t kingSpan;
extern uint64_t knightSpan;
extern uint64_t castleRooks[4];
extern uint64_t rankMasks8[8];
extern uint64_t fileMasks8[8];
extern uint64_t diagonalMasks8[15];
extern uint64_t antiDiagonalMasks8[15];

extern uint64_t flipDiagA1H8(uint64_t s);

extern uint64_t flipVertical(uint64_t s);

extern uint64_t rotate90clockwise(uint64_t s);

extern uint64_t rotate90antiClockwise(uint64_t s);

extern string convertBitboardToStringRep(uint64_t bitboard); // namespace

#endif // #ifndef BITBOARD_H_INCLUDED