#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../maestro/boardGeneration.h"
#include "../maestro/moves.h"

TEST_CASE("importFEN works") {
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

TEST_CASE("horizontal and vertical move generation works") {
    Moves& moves = Moves::getInstance();

    moves.occupiedSquares = 8796109801472;
    moves.opponentSquares = 16777216;
    REQUIRE(moves.horizontalAndVerticalMoves(27) == 38504235008);

    moves.occupiedSquares = 0;
    moves.opponentSquares = 0;
    REQUIRE(moves.horizontalAndVerticalMoves(35) == 578722409201797128);

    moves.occupiedSquares = 0;
    moves.opponentSquares = 0;
    REQUIRE(moves.horizontalAndVerticalMoves(36) == 1157443723186933776);

    moves.occupiedSquares = 134744064;
    moves.opponentSquares = 134744064;
    REQUIRE(moves.horizontalAndVerticalMoves(35) == 578722409201270784);

}

TEST_CASE("covered horizontal and vertical squares works") {
    Moves& moves = Moves::getInstance();

    moves.occupiedSquares = 2260827969683456;
    moves.opponentSquares = 2260621676511232;
    REQUIRE(moves.coveredHorizontalAndVericalSquares(35) == 8882126585856);
}

TEST_CASE("diagonal and anti-diagonal move generation works") {
    Moves& moves = Moves::getInstance();

    moves.occupiedSquares = 288230376153808896;
    moves.opponentSquares = 288230376151711744;
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(49) == 360293502375952384);

    moves.occupiedSquares = 11259170867118080;
    moves.opponentSquares = 11259170867118080;
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(44) == 11259170867118080);

    moves.occupiedSquares = 11259170867118080;
    moves.opponentSquares = 0;
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(44) == 0);

    moves.occupiedSquares = 0;
    moves.opponentSquares = 0;
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(35) == 4693335752243822976);

    moves.occupiedSquares = 0;
    moves.opponentSquares = 0;
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(36) == 9386671504487645697);

    moves.occupiedSquares = 4432674684928;
    moves.opponentSquares = 4432674684928;
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(49) == 360293467747778560);
}

TEST_CASE("covered diagonal and anti-diagonal squares works") {
    Moves& moves = Moves::getInstance();

    moves.occupiedSquares = 9592139778490368;
    moves.opponentSquares = 9592139440717824;
    REQUIRE(moves.coveredDiagonalAndAntiDiagonalSquares(35) == 21990568099840);

    moves.occupiedSquares = 9214224101724495870;
    moves.opponentSquares = 9214224100648550400;
    REQUIRE(moves.coveredDiagonalAndAntiDiagonalSquares(2) == 2560);
}

TEST_CASE("pawn move generation works") {
    
}

TEST_CASE("knight move generation works") {
    
}

TEST_CASE("bishop move generation works") {
    Moves& moves = Moves::getInstance();

    moves.occupiedSquares = 17592521588736;
    moves.opponentSquares = 17592253153280;
    REQUIRE(moves.possibleB(35) == "35263542354435493556");
}

TEST_CASE("rook move generation works") {
    Moves& moves = Moves::getInstance();

    moves.occupiedSquares = 68853694464;
    moves.opponentSquares = 68853694464;
    REQUIRE(moves.possibleR(35) == "35273532353335343536354335513559");
}

TEST_CASE("queen move generation works") {
    Moves& moves = Moves::getInstance();

    moves.occupiedSquares = 68853694464;
    moves.opponentSquares = 68853694464;
    REQUIRE(moves.possibleQ(35) == "350735083514351735213526352735283532353335343536354235433544354935513553355635593562");
}

TEST_CASE("king move generation works") {
    
}

TEST_CASE("unsafe for white calculation works") {
    BoardGeneration& boardGeneration = BoardGeneration::getInstance();
    boardGeneration.importFEN("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    Moves& moves = Moves::getInstance();
    REQUIRE(moves.unsafeForWhite() == 688587275901);
}

TEST_CASE("unsafe for black calculation works") {
    BoardGeneration& boardGeneration = BoardGeneration::getInstance();
    boardGeneration.importFEN("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    Moves& moves = Moves::getInstance();
    REQUIRE(moves.unsafeForBlack() == 13726900470852616192U);
}