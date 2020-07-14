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

    uint64_t fileMasks8[8] = { 72340172838076673, 144680345676153346, 289360691352306692, 578721382704613384, 1157442765409226768, 2314885530818453536, 4629771061636907072, 9259542123273814144 };
};

#endif // MOVES_H_INCLUDED
