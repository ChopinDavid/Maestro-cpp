#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED
#include <bitset>
#include <algorithm>
using namespace std;

uint64_t fileA = 72340172838076673U;
uint64_t fileH = 9259542123273814144U;
uint64_t filesAB = 217020518514230019U;
uint64_t filesGH = 13889313184910721216U;
uint64_t rank1 = 18374686479671623680U;
uint64_t rank4 = 1095216660480U;
uint64_t rank5 = 4278190080U;
uint64_t rank8 = 255U;
uint64_t center = 103481868288U;
uint64_t extendedCenter = 66229406269440U;
uint64_t kingSide = 17361641481138401520U;
uint64_t kingSpan = 460039U;
uint64_t knightSpan = 43234889994U;
uint64_t castleRooks[4] = {63U, 56U, 7U, 0U};
uint64_t rankMasks8[8] = {255U, 65280U, 16711680U, 4278190080U, 1095216660480U, 280375465082880U, 71776119061217280U, 18374686479671623680U};
uint64_t fileMasks8[8] = {72340172838076673U, 144680345676153346U, 289360691352306692U, 578721382704613384U, 1157442765409226768U, 2314885530818453536U, 4629771061636907072U, 9259542123273814144U};
uint64_t diagonalMasks8[15] = {1U, 258U, 66052U, 16909320U, 4328785936U, 1108169199648U, 283691315109952U, 72624976668147840U, 145249953336295424U, 290499906672525312U, 580999813328273408U, 1161999622361579520U, 2323998145211531264U, 4647714815446351872U, 9223372036854775808U};
uint64_t antiDiagonalMasks8[15] = {128U, 32832U, 8405024U, 2151686160U, 550831656968U, 141012904183812U, 36099303471055874U, 9241421688590303745U, 4620710844295151872U, 2310355422147575808U, 1155177711073755136U, 577588855528488960U, 288794425616760832U, 144396663052566528U, 72057594037927936U};

string convertBitboardToStringRep(uint64_t bitboard)
{
    std::string s = std::bitset<64>(bitboard).to_string();
    reverse(s.begin(), s.end());
    return s;
} // namespace

#endif // #ifndef BITBOARD_H_INCLUDED