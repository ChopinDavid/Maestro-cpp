#include "bitboard.h"
#include <bitset>
#include <algorithm>
using namespace std;

uint64_t fileA = 72340172838076673ULL;
uint64_t fileH = 9259542123273814144ULL;
uint64_t filesAB = 217020518514230019ULL;
uint64_t filesGH = 13889313184910721216ULL;
uint64_t rank1 = 18374686479671623680ULL;
uint64_t rank4 = 1095216660480ULL;
uint64_t rank5 = 4278190080ULL;
uint64_t rank8 = 255ULL;
uint64_t center = 103481868288ULL;
uint64_t extendedCenter = 66229406269440ULL;
uint64_t kingSide = 17361641481138401520ULL;
uint64_t kingSpan = 460039ULL;
uint64_t knightSpan = 43234889994ULL;
uint64_t castleRooks[4] = {63ULL, 56ULL, 7ULL, 0ULL};
uint64_t rankMasks8[8] = {255ULL, 65280ULL, 16711680ULL, 4278190080ULL, 1095216660480ULL, 280375465082880ULL, 71776119061217280ULL, 18374686479671623680ULL};
uint64_t fileMasks8[8] = {72340172838076673ULL, 144680345676153346ULL, 289360691352306692ULL, 578721382704613384ULL, 1157442765409226768ULL, 2314885530818453536ULL, 4629771061636907072ULL, 9259542123273814144ULL};
uint64_t diagonalMasks8[15] = {1ULL, 258ULL, 66052ULL, 16909320ULL, 4328785936ULL, 1108169199648ULL, 283691315109952ULL, 72624976668147840ULL, 145249953336295424ULL, 290499906672525312ULL, 580999813328273408ULL, 1161999622361579520ULL, 2323998145211531264ULL, 4647714815446351872ULL, 9223372036854775808ULL};
uint64_t antiDiagonalMasks8[15] = {128ULL, 32832ULL, 8405024ULL, 2151686160ULL, 550831656968ULL, 141012904183812ULL, 36099303471055874ULL, 9241421688590303745ULL, 4620710844295151872ULL, 2310355422147575808ULL, 1155177711073755136ULL, 577588855528488960ULL, 288794425616760832ULL, 144396663052566528ULL, 72057594037927936ULL};

uint64_t flipDiagA1H8(uint64_t s)
{
    uint64_t t;
    const uint64_t k1 = uint64_t(0x5500550055005500);
    const uint64_t k2 = uint64_t(0x3333000033330000);
    const uint64_t k4 = uint64_t(0x0f0f0f0f00000000);
    t = k4 & (s ^ (s << 28));
    s ^= t ^ (t >> 28);
    t = k2 & (s ^ (s << 14));
    s ^= t ^ (t >> 14);
    t = k1 & (s ^ (s << 7));
    s ^= t ^ (t >> 7);
    return s;
}

uint64_t flipVertical(uint64_t s)
{
    return ((s << 56)) |
           ((s << 40) & uint64_t(0x00ff000000000000)) |
           ((s << 24) & uint64_t(0x0000ff0000000000)) |
           ((s << 8) & uint64_t(0x000000ff00000000)) |
           ((s >> 8) & uint64_t(0x00000000ff000000)) |
           ((s >> 24) & uint64_t(0x0000000000ff0000)) |
           ((s >> 40) & uint64_t(0x000000000000ff00)) |
           ((s >> 56));
}

uint64_t rotate90clockwise(uint64_t s)
{
    return flipVertical(flipDiagA1H8(s));
}

uint64_t rotate90antiClockwise(uint64_t s)
{
    return flipDiagA1H8(flipVertical(s));
}

string convertBitboardToStringRep(uint64_t bitboard)
{
    std::string s = std::bitset<64>(bitboard).to_string();
    reverse(s.begin(), s.end());
    return s;
}