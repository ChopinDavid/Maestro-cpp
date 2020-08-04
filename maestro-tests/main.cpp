#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../maestro/boardGeneration.h"
#include "../maestro/moves.h"

TEST_CASE("importFEN works")
{
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
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

TEST_CASE("horizontal and vertical move generation works")
{
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.importFEN("8/3P4/8/p2R4/8/3P4/8/8 w - - 0 1");
    REQUIRE(moves.horizontalAndVerticalMoves(27) == 38504235008);

    boardGeneration.importFEN("8/8/8/8/3R4/8/8/8 w - - 0 1");
    REQUIRE(moves.horizontalAndVerticalMoves(35) == 578722409201797128);

    boardGeneration.importFEN("8/8/8/8/4R3/8/8/8 w - - 0 1");
    REQUIRE(moves.horizontalAndVerticalMoves(36) == 1157443723186933776);

    boardGeneration.importFEN("8/3p4/3p4/3p4/3R4/8/8/8 w - - 0 1");
    REQUIRE(moves.horizontalAndVerticalMoves(35) == 578722409201270784);
}

TEST_CASE("covered horizontal and vertical squares works")
{
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.importFEN("8/8/3P4/3P4/1ppRPP2/3p4/3p4/8 w - - 0 1");
    REQUIRE(moves.coveredHorizontalAndVericalSquares(35) == 8882126585856);
}

TEST_CASE("diagonal and anti-diagonal move generation works")
{
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.importFEN("8/8/5P2/8/8/8/1B6/2p5 w - - 0 1");
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(49) == 360293502375952384);

    boardGeneration.importFEN("8/8/8/8/3p1p2/4B3/3p1p2/8 w - - 0 1");
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(44) == 11259170867118080);

    boardGeneration.importFEN("8/8/8/8/3P1P2/4B3/3P1P2/8 w - - 0 1");
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(44) == 0);

    boardGeneration.importFEN("8/8/8/8/3B4/8/8/8 w - - 0 1");
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(35) == 4693335752243822976);

    boardGeneration.importFEN("8/8/8/8/4B3/8/8/8 w - - 0 1");
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(36) == 9386671504487645697);

    boardGeneration.importFEN("8/8/8/4p3/3p4/2p5/1B6/8 w - - 0 1");
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(49) == 360293467747778560);
}

TEST_CASE("covered diagonal and anti-diagonal squares works")
{
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.importFEN("8/8/1P3P2/2P1P3/3B4/2p1p3/1p3p2/8 w - - 0 1");
    REQUIRE(moves.coveredDiagonalAndAntiDiagonalSquares(35) == 21990568099840);

    boardGeneration.importFEN("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    REQUIRE(moves.coveredDiagonalAndAntiDiagonalSquares(2) == 2560);

    boardGeneration.importFEN("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    REQUIRE(moves.coveredDiagonalAndAntiDiagonalSquares(3) == 2102272);
}

TEST_CASE("white pawn move generation works")
{
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.importFEN("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    REQUIRE(moves.possibleWP() == "574760506151625263536454665660406141624263436444664656WE");

    boardGeneration.importFEN("8/8/2p1p3/3P4/8/8/8/8 b - - 0 1");
    REQUIRE(moves.possibleWP() == "332433223323");

    boardGeneration.importFEN("rnbqkbnr/pp1ppppp/8/8/2pPP3/5P2/PPP3PP/RNBQKBNR b KQkq d3 0 3");
    REQUIRE(moves.possibleWP() == "433344345545605061516252665667576040614166466747");

    boardGeneration.importFEN("8/8/8/3PpP2/8/8/8/8 w - e6 0 1");
    REQUIRE(moves.possibleWP() == "3323352534WE54WE");

    boardGeneration.importFEN("1n1n4/2P5/8/8/8/8/8/8 b - - 0 1");
    REQUIRE(moves.possibleWP() == "23QP23RP23BP23NP21QP21RP21BP21NP22QP22RP22BP22NP");

    boardGeneration.importFEN("1n1n4/2P5/8/4pP2/8/8/8/8 w - e6 0 1");
    REQUIRE(moves.possibleWP() == "352523QP23RP23BP23NP21QP21RP21BP21NP22QP22RP22BP22NP54WE");
}

TEST_CASE("black pawn move generation works")
{
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.importFEN("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    REQUIRE(moves.possibleBP() == "112112221323142417272030364611311232133314341737");

    boardGeneration.importFEN("8/8/8/3p4/2P1P3/8/8/8 w - - 0 1");
    REQUIRE(moves.possibleBP() == "334233443343");

    boardGeneration.importFEN("rnbqkbnr/pp1ppppp/8/8/2pPP3/5P2/PPP3PP/RNBQKBNR b KQkq d3 0 3");
    REQUIRE(moves.possibleBP() == "10201121132314241525162617274252103011311333143415351636173723WE");

    boardGeneration.importFEN("8/8/8/8/2pPp3/8/8/8 w - d3 0 1");
    REQUIRE(moves.possibleBP() == "4252445443WE23WE");

    boardGeneration.importFEN("8/8/8/8/8/8/2p5/1N1N4 b - - 0 1");
    REQUIRE(moves.possibleBP() == "21QP21RP21BP21NP23QP23RP23BP23NP22QP22RP22BP22NP");

    boardGeneration.importFEN("8/8/8/8/5pP1/8/2p5/1N1N4 b - g3 0 1");
    REQUIRE(moves.possibleBP() == "455521QP21RP21BP21NP23QP23RP23BP23NP22QP22RP22BP22NP56WE");
}

TEST_CASE("knight move generation works")
{
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.importFEN("k7/8/8/8/8/5N2/8/K6N w - - 0 1");
    REQUIRE(moves.possibleN() == "5534553655435547556355675574557677567765");

    boardGeneration.importFEN("N6K/8/2N5/8/8/8/8/7k w - - 0 1");
    REQUIRE(moves.possibleN() == "0012002122012203221022142230223422412243");

    boardGeneration.importFEN("N6K/8/8/8/8/8/8/N6k w - - 0 1");
    REQUIRE(moves.possibleN() == "0012002170517062");
}

TEST_CASE("bishop move generation works")
{
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.importFEN("8/8/8/2p1P3/3B4/4p3/8/8 w - - 0 1");
    REQUIRE(moves.possibleB() == "35263542354435493556");
}

TEST_CASE("rook move generation works")
{
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.importFEN("8/8/8/3p4/3Rp3/8/8/8 w - - 0 1");
    REQUIRE(moves.possibleR() == "35273532353335343536354335513559");
}

TEST_CASE("queen move generation works")
{
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.importFEN("8/8/8/3p4/3Qp3/8/8/8 w - - 0 1");
    REQUIRE(moves.possibleQ() == "350735083514351735213526352735283532353335343536354235433544354935513553355635593562");
}

TEST_CASE("king move generation works")
{
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.importFEN("3kr3/8/8/8/3P4/3K4/8/8 w - - 0 1");
    REQUIRE(moves.possibleK() == "5342535253625363");

    boardGeneration.importFEN("3k4/8/8/5p2/4p3/3K4/8/8 w - - 0 1");
    REQUIRE(moves.possibleK() == "5342534353525354536253635364");

    boardGeneration.importFEN("8/8/8/3k4/3p4/8/8/2RK4 b - - 0 1");
    REQUIRE(moves.possibleK() == "3323332433343344");

    boardGeneration.importFEN("8/8/8/3k4/2P5/1P6/8/3K4 b - - 0 1");
    REQUIRE(moves.possibleK() == "3322332333243332333433433344");
}

TEST_CASE("unsafe for white calculation works")
{
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.importFEN("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    REQUIRE(moves.unsafeForWhite() == 688587275901);

    boardGeneration.importFEN("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    REQUIRE(moves.unsafeForWhite() == 1626148437886);
}

TEST_CASE("unsafe for black calculation works")
{
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.importFEN("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    REQUIRE(moves.unsafeForBlack() == 13726900470852616192U);
}

TEST_CASE("white castling calculation works") {
    BoardGeneration &boardGeneration = BoardGeneration::getInstance();
    Moves &moves = Moves::getInstance();

    boardGeneration.importFEN("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    REQUIRE(moves.possibleCW() == "7476");

    boardGeneration.importFEN("r1b2rk1/ppppnpp1/5q1p/2b1p3/2B1P3/2NP1N2/PPPQ1PPP/R3K2R w KQ - 2 9");
    REQUIRE(moves.possibleCW() == "74767472");

    boardGeneration.importFEN("r1b2rk1/pp1pnpp1/2p2q1p/2b1p3/2B1P3/2NP1N2/PPPQKPPP/R6R w - - 0 10");
    REQUIRE(moves.possibleCW() == "");

    boardGeneration.importFEN("r1b2rk1/ppppnp2/6p1/2b1N1qp/2B1P3/2NP2P1/PPP1QP1P/R3K2R w KQ - 0 12");
    REQUIRE(moves.possibleCW() == "7476");

    boardGeneration.importFEN("r1b2rk1/ppppnp2/5qp1/2b1N2p/2B1P3/2NP1PP1/PPP1Q2P/R3K2R w KQ - 0 13");
    REQUIRE(moves.possibleCW() == "7472");

    boardGeneration.importFEN("r1b2rk1/ppppnp2/6pq/2b1N2p/2B1P3/2NP1PP1/PPP1Q2P/R3K2R w KQ - 1 13");
    REQUIRE(moves.possibleCW() == "");
}