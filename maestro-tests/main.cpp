#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../maestro/board.h"
#include "../maestro/moves.h"

//Move generation tests
TEST_CASE("importFEN works")
{
    Board board = Board::from("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");

    REQUIRE(board.getWP() == 26880860812738560);
    REQUIRE(board.getWN() == 4755801206503243776);
    REQUIRE(board.getWB() == 2594073385365405696);
    REQUIRE(board.getWR() == 108086391056891904);
    REQUIRE(board.getWQ() == 576460752303423488);
    REQUIRE(board.getWK() == 1152921504606846976);
    REQUIRE(board.getBP() == 1075944960);
    REQUIRE(board.getBN() == 66);
    REQUIRE(board.getBB() == 36);
    REQUIRE(board.getBR() == 384);
    REQUIRE(board.getBQ() == 8);
    REQUIRE(board.getBK() == 16);
    REQUIRE(board.getEP() == 4629771061636907072);
    REQUIRE(board.getCWK() == false);
    REQUIRE(board.getCBK() == true);
    REQUIRE(board.getCWQ() == true);
    REQUIRE(board.getCBQ() == false);
}

TEST_CASE("drawArray works") {
    Board board = Board::from("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    REQUIRE(board.boardString() == "[   ][ n ][ b ][ q ][ k ][ b ][ n ][ r ]\n[ r ][ p ][ p ][ p ][ p ][   ][   ][ p ]\n[ p ][   ][   ][   ][   ][ p ][   ][   ]\n[   ][   ][   ][   ][   ][ P ][ p ][   ]\n[   ][   ][   ][   ][   ][   ][   ][   ]\n[   ][   ][   ][   ][   ][   ][   ][ P ]\n[ P ][ P ][ P ][ P ][ P ][   ][ P ][ R ]\n[ R ][ N ][ B ][ Q ][ K ][ B ][ N ][   ]");

    
    REQUIRE(board.boardString() == "[   ][ n ][ b ][ q ][ k ][ b ][ n ][ r ]\n[ r ][ p ][ p ][ p ][ p ][   ][   ][ p ]\n[ p ][   ][   ][   ][   ][ p ][   ][   ]\n[   ][   ][   ][   ][   ][ P ][ p ][   ]\n[   ][   ][   ][   ][   ][   ][   ][   ]\n[   ][   ][   ][   ][   ][   ][   ][ P ]\n[ P ][ P ][ P ][ P ][ P ][   ][ P ][ R ]\n[ R ][ N ][ B ][ Q ][ K ][ B ][ N ][   ]");
}

TEST_CASE("horizontal and vertical move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("8/3P4/8/p2R4/8/3P4/8/8 w - - 0 1");
    REQUIRE(moves.horizontalAndVerticalMoves(board, 27) == 38504235008);

    board = Board::from("8/8/8/8/3R4/8/8/8 w - - 0 1");
    REQUIRE(moves.horizontalAndVerticalMoves(board, 35) == 578722409201797128);

    board = Board::from("8/8/8/8/4R3/8/8/8 w - - 0 1");
    REQUIRE(moves.horizontalAndVerticalMoves(board, 36) == 1157443723186933776);

    board = Board::from("8/3p4/3p4/3p4/3R4/8/8/8 w - - 0 1");
    REQUIRE(moves.horizontalAndVerticalMoves(board, 35) == 578722409201270784);
}

TEST_CASE("covered horizontal and vertical squares works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("8/8/3P4/3P4/1ppRPP2/3p4/3p4/8 w - - 0 1");
    REQUIRE(moves.coveredHorizontalAndVericalSquares(board, 35) == 8882126585856);

    board = Board::from("rnbqk2r/ppppppbp/5np1/8/2PP4/2N2N2/PP2PPPP/R1BQKB1R b KQkq - 3 4");
    REQUIRE(moves.coveredHorizontalAndVericalSquares(board, 63) == 6953557824660045824);
}

TEST_CASE("diagonal and anti-diagonal move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("8/8/5P2/8/8/8/1B6/2p5 w - - 0 1");
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(board, 49) == 360293502375952384);

    board = Board::from("8/8/8/8/3p1p2/4B3/3p1p2/8 w - - 0 1");
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(board, 44) == 11259170867118080);

    board = Board::from("8/8/8/8/3P1P2/4B3/3P1P2/8 w - - 0 1");
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(board, 44) == 0);

    board = Board::from("8/8/8/8/3B4/8/8/8 w - - 0 1");
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(board, 35) == 4693335752243822976);

    board = Board::from("8/8/8/8/4B3/8/8/8 w - - 0 1");
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(board, 36) == 9386671504487645697ULL);

    board = Board::from("8/8/8/4p3/3p4/2p5/1B6/8 w - - 0 1");
    REQUIRE(moves.diagonalAndAntiDiagonalMoves(board, 49) == 360293467747778560);
}

TEST_CASE("covered diagonal and anti-diagonal squares works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("8/8/1P3P2/2P1P3/3B4/2p1p3/1p3p2/8 w - - 0 1");
    REQUIRE(moves.coveredDiagonalAndAntiDiagonalSquares(board, 35) == 21990568099840);

    board = Board::from("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    REQUIRE(moves.coveredDiagonalAndAntiDiagonalSquares(board, 2) == 2560);

    board = Board::from("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    REQUIRE(moves.coveredDiagonalAndAntiDiagonalSquares(board, 3) == 2102272);
}

TEST_CASE("white pawn move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    REQUIRE(moves.possibleWP(board) == "574760506151625263536454665660406141624263436444664656WE");

    board = Board::from("8/8/2p1p3/3P4/8/8/8/8 b - - 0 1");
    REQUIRE(moves.possibleWP(board) == "332433223323");

    board = Board::from("rnbqkbnr/pp1ppppp/8/8/2pPP3/5P2/PPP3PP/RNBQKBNR b KQkq d3 0 3");
    REQUIRE(moves.possibleWP(board) == "433344345545605061516252665667576040614166466747");

    board = Board::from("8/8/8/3PpP2/8/8/8/8 w - e6 0 1");
    REQUIRE(moves.possibleWP(board) == "3323352534WE54WE");

    board = Board::from("1n1n4/2P5/8/8/8/8/8/8 b - - 0 1");
    REQUIRE(moves.possibleWP(board) == "23QP23RP23BP23NP21QP21RP21BP21NP22QP22RP22BP22NP");

    board = Board::from("1n1n4/2P5/8/4pP2/8/8/8/8 w - e6 0 1");
    REQUIRE(moves.possibleWP(board) == "352523QP23RP23BP23NP21QP21RP21BP21NP22QP22RP22BP22NP54WE");
}

TEST_CASE("black pawn move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    REQUIRE(moves.possibleBP(board) == "112112221323142417272030364611311232133314341737");

    board = Board::from("8/8/8/3p4/2P1P3/8/8/8 w - - 0 1");
    REQUIRE(moves.possibleBP(board) == "334233443343");

    board = Board::from("rnbqkbnr/pp1ppppp/8/8/2pPP3/5P2/PPP3PP/RNBQKBNR b KQkq d3 0 3");
    REQUIRE(moves.possibleBP(board) == "10201121132314241525162617274252103011311333143415351636173723WE");

    board = Board::from("8/8/8/8/2pPp3/8/8/8 w - d3 0 1");
    REQUIRE(moves.possibleBP(board) == "4252445443WE23WE");

    board = Board::from("8/8/8/8/8/8/2p5/1N1N4 b - - 0 1");
    REQUIRE(moves.possibleBP(board) == "21Qp21Rp21Bp21Np23Qp23Rp23Bp23Np22Qp22Rp22Bp22Np");

    board = Board::from("8/8/8/8/5pP1/8/2p5/1N1N4 b - g3 0 1");
    REQUIRE(moves.possibleBP(board) == "455521Qp21Rp21Bp21Np23Qp23Rp23Bp23Np22Qp22Rp22Bp22Np56WE");
}

TEST_CASE("knight move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("k7/8/8/8/8/5N2/8/K6N w - - 0 1");
    REQUIRE(moves.possibleN(board) == "5534553655435547556355675574557677567765");

    board = Board::from("N6K/8/2N5/8/8/8/8/7k w - - 0 1");
    REQUIRE(moves.possibleN(board) == "0012002122012203221022142230223422412243");

    board = Board::from("N6K/8/8/8/8/8/8/N6k w - - 0 1");
    REQUIRE(moves.possibleN(board) == "0012002170517062");

    board = Board::from("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    REQUIRE(moves.possibleN(board) == "5534553655435547557671507152");
}

TEST_CASE("bishop move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("8/8/8/2p1P3/3B4/4p3/8/8 w - - 0 1");
    REQUIRE(moves.possibleB(board) == "43324352435443614370");

    board = Board::from("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    REQUIRE(moves.possibleB(board) == "421542204224423142334251425342644275");
}

TEST_CASE("rook move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("8/8/8/3p4/3Rp3/8/8/8 w - - 0 1");
    REQUIRE(moves.possibleR(board) == "43334340434143424344435343634373");
}

TEST_CASE("queen move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("8/8/8/3p4/3Qp3/8/8/8 w - - 0 1");
    REQUIRE(moves.possibleQ(board) == "430743104316432143254332433343344340434143424344435243534354436143634365437043734376");
}

TEST_CASE("king move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("3kr3/8/8/8/3P4/3K4/8/8 w - - 0 1");
    REQUIRE(moves.possibleK(board) == "5342535253625363");

    board = Board::from("3k4/8/8/5p2/4p3/3K4/8/8 w - - 0 1");
    REQUIRE(moves.possibleK(board) == "5342534353525354536253635364");

    board = Board::from("8/8/8/3k4/3p4/8/8/2RK4 b - - 0 1");
    REQUIRE(moves.possibleK(board) == "3323332433343344");

    board = Board::from("8/8/8/3k4/2P5/1P6/8/3K4 b - - 0 1");
    REQUIRE(moves.possibleK(board) == "3322332333243332333433433344");
}

TEST_CASE("unsafe for white calculation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    REQUIRE(moves.unsafeForWhite(board) == 688587275901);

    board = Board::from("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    REQUIRE(moves.unsafeForWhite(board) == 1626148437886);
}

TEST_CASE("unsafe for black calculation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    REQUIRE(moves.unsafeForBlack(board) == 13726900470852616192U);
}

TEST_CASE("white castling calculation works") {
    Moves &moves = Moves::getInstance();

    Board board = Board::from("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    REQUIRE(moves.possibleCW(board) == "7476");

    board = Board::from("r1b2rk1/ppppnpp1/5q1p/2b1p3/2B1P3/2NP1N2/PPPQ1PPP/R3K2R w KQ - 2 9");
    REQUIRE(moves.possibleCW(board) == "74767472");

    board = Board::from("r1b2rk1/pp1pnpp1/2p2q1p/2b1p3/2B1P3/2NP1N2/PPPQKPPP/R6R w - - 0 10");
    REQUIRE(moves.possibleCW(board) == "");

    board = Board::from("r1b2rk1/ppppnp2/6p1/2b1N1qp/2B1P3/2NP2P1/PPP1QP1P/R3K2R w KQ - 0 12");
    REQUIRE(moves.possibleCW(board) == "7476");

    board = Board::from("r1b2rk1/ppppnp2/5qp1/2b1N2p/2B1P3/2NP1PP1/PPP1Q2P/R3K2R w KQ - 0 13");
    REQUIRE(moves.possibleCW(board) == "7472");

    board = Board::from("r1b2rk1/ppppnp2/6pq/2b1N2p/2B1P3/2NP1PP1/PPP1Q2P/R3K2R w KQ - 1 13");
    REQUIRE(moves.possibleCW(board) == "");
}

TEST_CASE("black castling calculation works") {
    Moves &moves = Moves::getInstance();

    Board board = Board::from("rnbqk2r/ppppppbp/5np1/8/2PP4/2N2N2/PP2PPPP/R1BQKB1R b KQkq - 3 4");
    REQUIRE(moves.possibleCB(board) == "0406");

    board = Board::from("r3kb1r/pppqpp1p/2np1np1/3P1b2/2P5/2N1PN2/PP1QBPPP/R1B2RK1 b kq - 4 9");
    REQUIRE(moves.possibleCB(board) == "0402");

    board = Board::from("r3k2r/pppqppbp/2np1np1/3P1b2/2P5/2NQPN2/PP2BPPP/R1B2RK1 b kq - 6 10");
    REQUIRE(moves.possibleCB(board) == "04060402");

    board = Board::from("r3k2r/p1pqppbp/Qpnp1np1/2PP4/6b1/2N1PN2/PP2BPPP/R1B2RK1 b kq - 2 12");
    REQUIRE(moves.possibleCB(board) == "0406");

    board = Board::from("r3k2r/p1pqpp1p/1pnp1bpQ/3Pnb2/2P5/1PN1PN2/P3BPPP/R1B2RK1 b kq - 3 14");
    REQUIRE(moves.possibleCB(board) == "0402");

    board = Board::from("r3k2r/N1p1pp1p/1pnpqbpQ/3P1b2/2P5/1P1nPN2/P3BPPP/R1B2RK1 b kq - 0 16");
    REQUIRE(moves.possibleCB(board) == "");

    board = Board::from("r6r/pppkppbp/2np1np1/1QPPqb2/8/1PN1PN2/P3BPPP/R1B2RK1 b - - 0 13");
    REQUIRE(moves.possibleCB(board) == "");
}

TEST_CASE("pseudolegal white move generation works") {
    Moves &moves = Moves::getInstance();

    Board board = Board::from("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    REQUIRE(moves.pseudoLegalMovesW(board) == "605061516252635366566757604061416343664667475534553655435547557671507152421542204224423142334251425342644275777577767364746474757476");

    board = Board::from("2B5/4k2K/P7/7b/2r2pP1/3P1QpP/5Rn1/2R1r3 w - - 0 1");
    REQUIRE(moves.pseudoLegalMovesW(board) == "463753422010463653435747021102130224023565606561656265636564656665757242725272627270727172737274550055115522553355445545555455565564556655731706170717161727");

    board = Board::from("8/4P3/8/PkPpBr2/RpN2p2/1PP2R1K/8/n7 w - d6 0 1");
    REQUIRE(moves.pseudoLegalMovesW(board) == "52413020322244QP44RP44BP44NP23WE4221422342504254426142633401340734123416342334253443344540414050406040705545555355545556556555755746574757665767");
}

TEST_CASE("pseudolegal black move generation works") {
    Moves &moves = Moves::getInstance();

    Board board = Board::from("rnb2rk1/p5bp/2pn1q2/1p3ppN/3P4/2NB1P2/PPQ2BPP/R3R1K1 b - - 1 19");
    REQUIRE(moves.pseudoLegalMovesB(board) == "102017272232314135453646103001130120230423112315234223440211021302200224160716270503050405152503251425152524252625272534254306070615");

    board = Board::from("rnb1k2r/pp1pb2p/1q2pnp1/5p2/2pPPP2/5NPP/PPP3BK/RNBQ1R2 b kq d3 0 9");
    REQUIRE(moves.pseudoLegalMovesB(board) == "354410201323172724342636425210301333173723WE0120012225062533253725442546140314051423143214411450070507062103211221202122212321302131213221412143215121610403040504150406");
}

TEST_CASE("move making works") {
    
    Moves &moves = Moves::getInstance();

    Board board = Board::from("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/3P1N2/PPP2PPP/RNBQK2R b KQkq - 0 4");
    REQUIRE(moves.makeMoveSinglePiece(board, board.getBB(), "0532") == 67108868);

    board = Board::from("rnbqkbnr/ppp1ppp1/8/2Pp3p/8/8/PP1PPPPP/RNBQKBNR w KQkq d6 0 3");
    REQUIRE(moves.makeMoveSinglePiece(board, board.getBP(), "23WE") == 2147514112);
    REQUIRE(moves.makeMoveSinglePiece(board, board.getWP(), "23WE") == 70650219154898944);

    board = Board::from("k1r1r3/3P4/8/8/8/8/8/K7 w - - 0 1");
    REQUIRE(moves.makeMoveSinglePiece(board, board.getWQ(), "33QP") == 8);

    board = Board::from("k1r1r3/3P4/8/8/8/8/8/K7 w - - 0 1");
    REQUIRE(moves.makeMoveSinglePiece(board, board.getWQ(), "32QP") == 4);

    board = Board::from("k1r1r3/3P4/8/8/8/8/8/K7 w - - 0 1");
    REQUIRE(moves.makeMoveSinglePiece(board, board.getWQ(), "34QP") == 16);

    board = Board::from("k7/8/8/8/8/8/3p4/K1R1R3 b - - 0 1");
    REQUIRE(moves.makeMoveSinglePiece(board, board.getBQ(), "33Qp") == 576460752303423488);

    board = Board::from("k7/8/8/8/8/8/3p4/K1R1R3 b - - 0 1");
    REQUIRE(moves.makeMoveSinglePiece(board, board.getBQ(), "32Qp") == 288230376151711744);

    board = Board::from("k7/8/8/8/8/8/3p4/K1R1R3 b - - 0 1");
    REQUIRE(moves.makeMoveSinglePiece(board, board.getBQ(), "34Qp") == 1152921504606846976);

    board = Board::initiateStandardChess();
    REQUIRE(moves.makeMoveSinglePiece(board, board.getWP(), "6444") == 67272588153323520);

    board = Board::from("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    board = moves.makeMoveAll(board, "7476");
    cout << "a" << endl;
    REQUIRE(board.boardString() == "[ r ][   ][ b ][ q ][ k ][   ][ n ][ r ]\n[ p ][ p ][ p ][ p ][   ][ p ][ p ][ p ]\n[   ][   ][ n ][   ][   ][   ][   ][   ]\n[   ][   ][ b ][   ][ p ][   ][   ][   ]\n[   ][   ][ B ][   ][ P ][   ][   ][   ]\n[   ][   ][   ][   ][   ][ N ][   ][   ]\n[ P ][ P ][ P ][ P ][   ][ P ][ P ][ P ]\n[ R ][ N ][ B ][ Q ][   ][ R ][ K ][   ]");

    board = Board::from("r2q1rk1/ppp2ppp/2npbn2/2b1p1B1/2B1P3/2NP1N2/PPPQ1PPP/R3K2R w KQ - 2 8");
    board = moves.makeMoveAll(board, "7472");
    REQUIRE(board.boardString() == "[ r ][   ][   ][ q ][   ][ r ][ k ][   ]\n[ p ][ p ][ p ][   ][   ][ p ][ p ][ p ]\n[   ][   ][ n ][ p ][ b ][ n ][   ][   ]\n[   ][   ][ b ][   ][ p ][   ][ B ][   ]\n[   ][   ][ B ][   ][ P ][   ][   ][   ]\n[   ][   ][ N ][ P ][   ][ N ][   ][   ]\n[ P ][ P ][ P ][ Q ][   ][ P ][ P ][ P ]\n[   ][   ][ K ][ R ][   ][   ][   ][ R ]");

    board = Board::from("rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 4");
    board = moves.makeMoveAll(board, "0406");
    REQUIRE(board.boardString() == "[ r ][ n ][ b ][ q ][   ][ r ][ k ][   ]\n[ p ][ p ][ p ][ p ][   ][ p ][ p ][ p ]\n[   ][   ][   ][   ][   ][ n ][   ][   ]\n[   ][   ][ b ][   ][ p ][   ][   ][   ]\n[   ][   ][ B ][   ][ P ][   ][   ][   ]\n[   ][   ][   ][   ][   ][ N ][   ][   ]\n[ P ][ P ][ P ][ P ][   ][ P ][ P ][ P ]\n[ R ][ N ][ B ][ Q ][   ][ R ][ K ][   ]");
}