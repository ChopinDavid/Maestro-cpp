#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../maestro/boardGeneration.h"

TEST_CASE( "importFEN works") {
    BoardGeneration& boardGeneration = BoardGeneration::getInstance();
    boardGeneration.importFEN("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");

    REQUIRE(boardGeneration.WP == 26880860812738560);
    REQUIRE(boardGeneration.WN == 4755801206503243776);
    REQUIRE(boardGeneration.WB == 2594073385365405696);
    REQUIRE(boardGeneration.WR == 108086391056891904);
    REQUIRE(boardGeneration.WQ == 576460752303423488);
    REQUIRE(boardGeneration.WK == 1152921504606846976);
    REQUIRE(boardGeneration.BP == 1075944960);
    REQUIRE(boardGeneration.BN == 66);
    REQUIRE(boardGeneration.BB == 36);
    REQUIRE(boardGeneration.BR == 384);
    REQUIRE(boardGeneration.BQ == 8);
    REQUIRE(boardGeneration.BK == 16);
    REQUIRE(boardGeneration.EP == 4629771061636907072);
    REQUIRE(boardGeneration.CWK == false);
    REQUIRE(boardGeneration.CBK == true);
    REQUIRE(boardGeneration.CWQ == true);
    REQUIRE(boardGeneration.CBQ == false);
}
