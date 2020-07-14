#include <iostream>
#include "boardGeneration.h"

using namespace std;

int main()
{
    BoardGeneration& boardGeneration = BoardGeneration::getInstance();
    boardGeneration.importFEN("1nbqkbn1/rppppppr/8/p6p/P6P/8/RPPPPPPR/1NBQKBN1 w - - 2 5");
    cout << "wp: " << boardGeneration.WP << endl;
    cout << "wR: " << boardGeneration.WR << endl;
    cout << "wN: " << boardGeneration.WN << endl;
    cout << "wB: " << boardGeneration.WB << endl;
    cout << "wQ: " << boardGeneration.WQ << endl;
    cout << "wK: " << boardGeneration.WK << endl;
    cout << "BP: " << boardGeneration.BP << endl;
    cout << "BR: " << boardGeneration.BR << endl;
    cout << "BN: " << boardGeneration.BN << endl;
    cout << "BB: " << boardGeneration.BB << endl;
    cout << "BQ: " << boardGeneration.BQ << endl;
    cout << "BK: " << boardGeneration.BK << endl;
    cout << "EP: " << boardGeneration.EP << endl;
    cout << "CWK: " << boardGeneration.CWK << endl;
    cout << "CBK: " << boardGeneration.CBK << endl;
    cout << "CWQ: " << boardGeneration.CWQ << endl;
    cout << "CBQ: " << boardGeneration.CBQ << endl;
    return 0;
}
