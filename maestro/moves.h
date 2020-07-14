#ifndef MOVES_H_INCLUDED
#define MOVES_H_INCLUDED

class Moves
{
private:
    Moves() {}

public:
    static Moves& getInstance()
    {
        static Moves shared;
        return shared;
    }

    uint64_t fileA = 72340172838076673;
    uint64_t fileH = 9259542123273814144;
    uint64_t filesAB = 217020518514230019;
    uint64_t filesGH = 13889313184910721216;
    uint64_t rank1 = 18374686479671623680;
    uint64_t rank4 = 1095216660480;
    uint64_t rank5 = 4278190080;
    uint64_t rank8 = 255;
    uint64_t center = 103481868288;
    uint64_t extendedCenter = 66229406269440;
    uint64_t kingSide = 17361641481138401520;
    uint64_t kingSpan = 460039;
    uint64_t knightSpan = 43234889994;
    uint64_t notMyPieces;
    uint64_t myPieces;
    uint64_t emptySquares;
    uint64_t occupiedSquares;

    uint64_t castleRooks = {63, 56, 7, 0};
    uint64_t rankMasks8[8] = { 255, 65280, 16711680, 4278190080, 1095216660480, 280375465082880, 71776119061217280, 18374686479671623680 };
    uint64_t fileMasks8[8] = { 72340172838076673, 144680345676153346, 289360691352306692, 578721382704613384, 1157442765409226768, 2314885530818453536, 4629771061636907072, 9259542123273814144 };
    uint64_t diagonalMasks8[8] = { 1, 258, 66052, 16909320, 4328785936, 1108169199648, 283691315109952, 72624976668147840, 145249953336295424, 290499906672525312, 580999813328273408, 1161999622361579520, 2323998145211531264, 4647714815446351872, 9223372036854775808 };
    uint64_t antiDiagonalMasks8[8] = { 128, 32832, 8405024, 2151686160, 550831656968, 141012904183812, 36099303471055874, 9241421688590303745, 4620710844295151872, 2310355422147575808, 1155177711073755136, 577588855528488960, 288794425616760832, 144396663052566528, 72057594037927936 };
};

#endif // MOVES_H_INCLUDED
