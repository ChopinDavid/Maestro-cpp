#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../maestro/board.h"
#include "../maestro/moves.h"
#include "../maestro/perft.h"
#include "../maestro/rating.h"
#include "../maestro/phase.h"
#include "../maestro/search.h"

//Move generation tests
// TEST_CASE("importFEN works")
// {
//     Board board = Board::from("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");

//     CHECK(board.getWP() == 26880860812738560);
//     CHECK(board.getWN() == 4755801206503243776);
//     CHECK(board.getWB() == 2594073385365405696);
//     CHECK(board.getWR() == 108086391056891904);
//     CHECK(board.getWQ() == 576460752303423488);
//     CHECK(board.getWK() == 1152921504606846976);
//     CHECK(board.getBP() == 1075944960);
//     CHECK(board.getBN() == 66);
//     CHECK(board.getBB() == 36);
//     CHECK(board.getBR() == 384);
//     CHECK(board.getBQ() == 8);
//     CHECK(board.getBK() == 16);
//     CHECK(board.getEP() == 4629771061636907072);
//     CHECK(board.getCWK() == false);
//     CHECK(board.getCBK() == true);
//     CHECK(board.getCWQ() == true);
//     CHECK(board.getCBQ() == false);
// }

// TEST_CASE("drawArray works")
// {
//     Board board = Board::from("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
//     CHECK(board.boardString() == "[   ][ n ][ b ][ q ][ k ][ b ][ n ][ r ]\n[ r ][ p ][ p ][ p ][ p ][   ][   ][ p ]\n[ p ][   ][   ][   ][   ][ p ][   ][   ]\n[   ][   ][   ][   ][   ][ P ][ p ][   ]\n[   ][   ][   ][   ][   ][   ][   ][   ]\n[   ][   ][   ][   ][   ][   ][   ][ P ]\n[ P ][ P ][ P ][ P ][ P ][   ][ P ][ R ]\n[ R ][ N ][ B ][ Q ][ K ][ B ][ N ][   ]");

//     CHECK(board.boardString() == "[   ][ n ][ b ][ q ][ k ][ b ][ n ][ r ]\n[ r ][ p ][ p ][ p ][ p ][   ][   ][ p ]\n[ p ][   ][   ][   ][   ][ p ][   ][   ]\n[   ][   ][   ][   ][   ][ P ][ p ][   ]\n[   ][   ][   ][   ][   ][   ][   ][   ]\n[   ][   ][   ][   ][   ][   ][   ][ P ]\n[ P ][ P ][ P ][ P ][ P ][   ][ P ][ R ]\n[ R ][ N ][ B ][ Q ][ K ][ B ][ N ][   ]");
// }

TEST_CASE("horizontal and vertical move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("8/3P4/8/p2R4/8/3P4/8/8 w - - 0 1");
    CHECK(moves.horizontalAndVerticalMoves(board, 27) == 38504235008);

    board = Board::from("8/8/8/8/3R4/8/8/8 w - - 0 1");
    CHECK(moves.horizontalAndVerticalMoves(board, 35) == 578722409201797128);

    board = Board::from("8/8/8/8/4R3/8/8/8 w - - 0 1");
    CHECK(moves.horizontalAndVerticalMoves(board, 36) == 1157443723186933776);

    board = Board::from("8/3p4/3p4/3p4/3R4/8/8/8 w - - 0 1");
    CHECK(moves.horizontalAndVerticalMoves(board, 35) == 578722409201270784);

    board = Board::from("8/8/8/3R4/3p4/3p4/3p4/8 w - - 0 1");
    CHECK(moves.horizontalAndVerticalMoves(board, 27) == 38504237064);

    board = Board::from("8/8/8/3Rppp1/8/8/8/8 w - - 0 1");
    CHECK(moves.horizontalAndVerticalMoves(board, 27) == 578721382956271624);

    board = Board::from("8/8/8/pppR4/8/8/8/8 w - - 0 1");
    CHECK(moves.horizontalAndVerticalMoves(board, 27) == 578721386664036360);

    board = Board::from("rnb1k2r/pp1pb2p/1q2pnp1/5p2/2pPPP2/5NPP/PPP3BK/RNBQ1R2 b kq d3 0 9");
    CHECK(moves.horizontalAndVerticalMoves(board, 7) == 96);
    CHECK(moves.horizontalAndVerticalMoves(board, 17) == 565157601017856);
    CHECK(moves.horizontalAndVerticalMoves(board, 0) == 0);
}

TEST_CASE("covered horizontal and vertical squares works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("8/8/3P4/3P4/1ppRPP2/3p4/3p4/8 w - - 0 1");
    CHECK(moves.coveredHorizontalAndVericalSquares(board, 35) == 8882126585856);

    board = Board::from("rnbqk2r/ppppppbp/5np1/8/2PP4/2N2N2/PP2PPPP/R1BQKB1R b KQkq - 3 4");
    CHECK(moves.coveredHorizontalAndVericalSquares(board, 63) == 6953557824660045824);
}

TEST_CASE("diagonal and anti-diagonal move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("8/8/5P2/8/8/8/1B6/2p5 w - - 0 1");
    CHECK(moves.diagonalAndAntiDiagonalMoves(board, 49) == 360293502375952384);

    board = Board::from("8/8/8/8/3p1p2/4B3/3p1p2/8 w - - 0 1");
    CHECK(moves.diagonalAndAntiDiagonalMoves(board, 44) == 11259170867118080);

    board = Board::from("8/8/8/8/3P1P2/4B3/3P1P2/8 w - - 0 1");
    CHECK(moves.diagonalAndAntiDiagonalMoves(board, 44) == 0);

    board = Board::from("8/8/8/8/3B4/8/8/8 w - - 0 1");
    CHECK(moves.diagonalAndAntiDiagonalMoves(board, 35) == 4693335752243822976);

    board = Board::from("8/8/8/8/4B3/8/8/8 w - - 0 1");
    CHECK(moves.diagonalAndAntiDiagonalMoves(board, 36) == 9386671504487645697ULL);

    board = Board::from("8/8/8/4p3/3p4/2p5/1B6/8 w - - 0 1");
    CHECK(moves.diagonalAndAntiDiagonalMoves(board, 49) == 360293467747778560);
}

TEST_CASE("covered diagonal and anti-diagonal squares works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("8/8/1P3P2/2P1P3/3B4/2p1p3/1p3p2/8 w - - 0 1");
    CHECK(moves.coveredDiagonalAndAntiDiagonalSquares(board, 35) == 21990568099840);

    board = Board::from("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    CHECK(moves.coveredDiagonalAndAntiDiagonalSquares(board, 2) == 2560);

    board = Board::from("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    CHECK(moves.coveredDiagonalAndAntiDiagonalSquares(board, 3) == 2102272);
}

TEST_CASE("white pawn move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    CHECK(moves.possibleWP(board) == "574760506151625263536454665660406141624263436444664656WE");

    board = Board::from("8/8/2p1p3/3P4/8/8/8/8 b - - 0 1");
    CHECK(moves.possibleWP(board) == "332433223323");

    board = Board::from("rnbqkbnr/pp1ppppp/8/8/2pPP3/5P2/PPP3PP/RNBQKBNR b KQkq d3 0 3");
    CHECK(moves.possibleWP(board) == "433344345545605061516252665667576040614166466747");

    board = Board::from("8/8/8/3PpP2/8/8/8/8 w - e6 0 1");
    CHECK(moves.possibleWP(board) == "3323352534WE54WE");

    board = Board::from("1n1n4/2P5/8/8/8/8/8/8 b - - 0 1");
    CHECK(moves.possibleWP(board) == "23QP23RP23BP23NP21QP21RP21BP21NP22QP22RP22BP22NP");

    board = Board::from("1n1n4/2P5/8/4pP2/8/8/8/8 w - e6 0 1");
    CHECK(moves.possibleWP(board) == "352523QP23RP23BP23NP21QP21RP21BP21NP22QP22RP22BP22NP54WE");

    board = Board::from("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ");
    CHECK(moves.possibleWP(board) == "60506151625266566757604061416646674732QP32RP32BP32NP");
}

TEST_CASE("black pawn move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    CHECK(moves.possibleBP(board) == "112112221323142417272030364611311232133314341737");

    board = Board::from("8/8/8/3p4/2P1P3/8/8/8 w - - 0 1");
    CHECK(moves.possibleBP(board) == "334233443343");

    board = Board::from("rnbqkbnr/pp1ppppp/8/8/2pPP3/5P2/PPP3PP/RNBQKBNR b KQkq d3 0 3");
    CHECK(moves.possibleBP(board) == "10201121132314241525162617274252103011311333143415351636173723BE");

    board = Board::from("8/8/8/8/2pPp3/8/8/8 w - d3 0 1");
    CHECK(moves.possibleBP(board) == "4252445443BE23BE");

    board = Board::from("8/8/8/8/8/8/2p5/1N1N4 b - - 0 1");
    CHECK(moves.possibleBP(board) == "21Qp21Rp21Bp21Np23Qp23Rp23Bp23Np22Qp22Rp22Bp22Np");

    board = Board::from("8/8/8/8/5pP1/8/2p5/1N1N4 b - g3 0 1");
    CHECK(moves.possibleBP(board) == "455521Qp21Rp21Bp21Np23Qp23Rp23Bp23Np22Qp22Rp22Bp22Np56BE");
}

TEST_CASE("knight move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("k7/8/8/8/8/5N2/8/K6N w - - 0 1");
    CHECK(moves.possibleN(board) == "5534553655435547556355675574557677567765");

    board = Board::from("N6K/8/2N5/8/8/8/8/7k w - - 0 1");
    CHECK(moves.possibleN(board) == "0012002122012203221022142230223422412243");

    board = Board::from("N6K/8/8/8/8/8/8/N6k w - - 0 1");
    CHECK(moves.possibleN(board) == "0012002170517062");

    board = Board::from("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    CHECK(moves.possibleN(board) == "5534553655435547557671507152");

    board = Board::from("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    CHECK(moves.possibleN(board) == "34133415342234263442344634535231524052715273");

    board = Board::from("rnbqkbnr/pppppppp/8/8/8/7N/PPPPPPPP/RNBQKB1R b KQkq - 1 1");
    CHECK(moves.possibleN(board) == "0120012206250627");
}

TEST_CASE("bishop move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("8/8/8/2p1P3/3B4/4p3/8/8 w - - 0 1");
    CHECK(moves.possibleB(board) == "43324352435443614370");

    board = Board::from("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    CHECK(moves.possibleB(board) == "421542204224423142334251425342644275");
}

TEST_CASE("rook move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("8/8/8/3p4/3Rp3/8/8/8 w - - 0 1");
    CHECK(moves.possibleR(board) == "43334340434143424344435343634373");
}

TEST_CASE("queen move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("8/8/8/3p4/3Qp3/8/8/8 w - - 0 1");
    CHECK(moves.possibleQ(board) == "430743104316432143254332433343344340434143424344435243534354436143634365437043734376");
}

TEST_CASE("king move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("3kr3/8/8/8/3P4/3K4/8/8 w - - 0 1");
    CHECK(moves.possibleK(board) == "5342535253625363");

    board = Board::from("3k4/8/8/5p2/4p3/3K4/8/8 w - - 0 1");
    CHECK(moves.possibleK(board) == "5342534353525354536253635364");

    board = Board::from("8/8/8/3k4/3p4/8/8/2RK4 b - - 0 1");
    CHECK(moves.possibleK(board) == "3323332433343344");

    board = Board::from("8/8/8/3k4/2P5/1P6/8/3K4 b - - 0 1");
    CHECK(moves.possibleK(board) == "3322332333243332333433433344");

    board = Board::from("8/7p/p5pb/4k3/P1pPn3/8/P5PP/1rB2RK1 b - d3 0 28");
    CHECK(moves.possibleK(board) == "3423342434333443");
}

TEST_CASE("unsafe for white calculation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    CHECK(moves.unsafeForWhite(board) == 688587275901);

    board = Board::from("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    CHECK(moves.unsafeForWhite(board) == 1626148437886);

    board = Board(0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 9007199254740992ULL, 0ULL, 4503599627370496ULL, 0ULL, 0ULL, 536870912ULL, 262144ULL, 0ULL, 0, 1, true, false, false, false, false);
    CHECK(moves.unsafeForWhite(board) == 5054424685706128932);

    board = Board(47006389830156288ULL, 4398046511168ULL, 6755399441055744ULL, 9295429630892703744ULL, 35184372088832ULL, 1152921504606846976ULL, 18014407237840128ULL, 2228224ULL, 81920ULL, 129ULL, 0ULL, 16ULL, 0ULL, 0, 1, false, true, true, true, true);
    CHECK(moves.unsafeForWhite(board) == 11569903391551175679ULL);
}

TEST_CASE("unsafe for black calculation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("1nbqkbnr/rpppp2p/p4p2/5Pp1/8/7P/PPPPP1PR/RNBQKBN1 w Qk g6 0 5");
    // CHECK(moves.unsafeForBlack(board) == 13726900470852616192U);

    board = Board(65020788339638272ULL, 4398046515200ULL, 6755399441055744ULL, 9295429630892703744ULL, 35184372088832ULL, 1152921504606846976ULL, 140746216713472ULL, 2228224ULL, 81920ULL, 129ULL, 0ULL, 16ULL, 0ULL, 0, 1, false, true, true, true, true);
    CHECK(moves.unsafeForBlack(board) == 9149624999897006148ULL);
}

TEST_CASE("white castling calculation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    CHECK(moves.possibleCW(board) == "7476");

    board = Board::from("r1b2rk1/ppppnpp1/5q1p/2b1p3/2B1P3/2NP1N2/PPPQ1PPP/R3K2R w KQ - 2 9");
    CHECK(moves.possibleCW(board) == "74767472");

    board = Board::from("r1b2rk1/pp1pnpp1/2p2q1p/2b1p3/2B1P3/2NP1N2/PPPQKPPP/R6R w - - 0 10");
    CHECK(moves.possibleCW(board) == "");

    board = Board::from("r1b2rk1/ppppnp2/6p1/2b1N1qp/2B1P3/2NP2P1/PPP1QP1P/R3K2R w KQ - 0 12");
    CHECK(moves.possibleCW(board) == "7476");

    board = Board::from("r1b2rk1/ppppnp2/5qp1/2b1N2p/2B1P3/2NP1PP1/PPP1Q2P/R3K2R w KQ - 0 13");
    CHECK(moves.possibleCW(board) == "7472");

    board = Board::from("r1b2rk1/ppppnp2/6pq/2b1N2p/2B1P3/2NP1PP1/PPP1Q2P/R3K2R w KQ - 1 13");
    CHECK(moves.possibleCW(board) == "");
}

TEST_CASE("black castling calculation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("rnbqk2r/ppppppbp/5np1/8/2PP4/2N2N2/PP2PPPP/R1BQKB1R b KQkq - 3 4");
    CHECK(moves.possibleCB(board) == "0406");

    board = Board::from("r3kb1r/pppqpp1p/2np1np1/3P1b2/2P5/2N1PN2/PP1QBPPP/R1B2RK1 b kq - 4 9");
    CHECK(moves.possibleCB(board) == "0402");

    board = Board::from("r3k2r/pppqppbp/2np1np1/3P1b2/2P5/2NQPN2/PP2BPPP/R1B2RK1 b kq - 6 10");
    CHECK(moves.possibleCB(board) == "04060402");

    board = Board::from("r3k2r/p1pqppbp/Qpnp1np1/2PP4/6b1/2N1PN2/PP2BPPP/R1B2RK1 b kq - 2 12");
    CHECK(moves.possibleCB(board) == "0406");

    board = Board::from("r3k2r/p1pqpp1p/1pnp1bpQ/3Pnb2/2P5/1PN1PN2/P3BPPP/R1B2RK1 b kq - 3 14");
    CHECK(moves.possibleCB(board) == "0402");

    board = Board::from("r3k2r/N1p1pp1p/1pnpqbpQ/3P1b2/2P5/1P1nPN2/P3BPPP/R1B2RK1 b kq - 0 16");
    CHECK(moves.possibleCB(board) == "");

    board = Board::from("r6r/pppkppbp/2np1np1/1QPPqb2/8/1PN1PN2/P3BPPP/R1B2RK1 b - - 0 13");
    CHECK(moves.possibleCB(board) == "");

    board = Board::from("r3k2r/p1pNqpb1/bn2pnp1/3P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1");
    CHECK(moves.possibleCB(board) == "0402");
}

TEST_CASE("pseudolegal white move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    CHECK(moves.pseudoLegalMovesW(board) == "605061516252635366566757604061416343664667475534553655435547557671507152421542204224423142334251425342644275777577767364746474757476");

    board = Board::from("2B5/4k2K/P7/7b/2r2pP1/3P1QpP/5Rn1/2R1r3 w - - 0 1");
    CHECK(moves.pseudoLegalMovesW(board) == "463753422010463653435747021102130224023565606561656265636564656665757242725272627270727172737274550055115522553355445545555455565564556655731706170717161727");

    board = Board::from("8/4P3/8/PkPpBr2/RpN2p2/1PP2R1K/8/n7 w - d6 0 1");
    CHECK(moves.pseudoLegalMovesW(board) == "52413020322244QP44RP44BP44NP23WE4221422342504254426142633401340734123416342334253443344540414050406040705545555355545556556555755746574757665767");

    board = Board::from("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    CHECK(moves.pseudoLegalMovesW(board) == "332466573323605061516656604066463413341534223426344234463453523152405271527363276336634563546372642064316442645364736475707170727073777577765525553555375545554655535554555655577473747574767472");

    board = Board::from("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    CHECK(moves.pseudoLegalMovesW(board) == "60506151625266566757604061416646674732QP32RP32BP32NP6443644564526456647671507152716342154220422442314233425142537227723672457254726377757776732373337343735373637463746574757476");
}

TEST_CASE("pseudolegal black move generation works")
{
    Moves &moves = Moves::getInstance();

    Board board = Board::from("rnb2rk1/p5bp/2pn1q2/1p3ppN/3P4/2NB1P2/PPQ2BPP/R3R1K1 b - - 1 19");
    CHECK(moves.pseudoLegalMovesB(board) == "102017272232314135453646103001130120230423112315234223440211021302200224160716270503050405152503251425152524252625272534254306070615");

    board = Board::from("rnb1k2r/pp1pb2p/1q2pnp1/5p2/2pPPP2/5NPP/PPP3BK/RNBQ1R2 b kq d3 0 9");
    CHECK(moves.pseudoLegalMovesB(board) == "354410201323172724342636425210301333173723BE0120012225062533253725442546140314051423143214411450070507062103211221202122212321302131213221412143215121610403040504150406");
}

TEST_CASE("move making works")
{

    Moves &moves = Moves::getInstance();

    Board board = Board::from("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/3P1N2/PPP2PPP/RNBQK2R b KQkq - 0 4");
    CHECK(moves.makeMoveSinglePiece(board, board.getBB(), "0532", 'b') == 67108868);

    board = Board::from("rnbqkbnr/ppp1ppp1/8/2Pp3p/8/8/PP1PPPPP/RNBQKBNR w KQkq d6 0 3");
    CHECK(moves.makeMoveSinglePiece(board, board.getBP(), "23WE", 'p') == 2147514112);
    CHECK(moves.makeMoveSinglePiece(board, board.getWP(), "23WE", 'P') == 70650219154898944);

    board = Board::from("k1r1r3/3P4/8/8/8/8/8/K7 w - - 0 1");
    CHECK(moves.makeMoveSinglePiece(board, board.getWQ(), "33QP", 'Q') == 8);

    board = Board::from("k1r1r3/3P4/8/8/8/8/8/K7 w - - 0 1");
    CHECK(moves.makeMoveSinglePiece(board, board.getWQ(), "32QP", 'Q') == 4);

    board = Board::from("k1r1r3/3P4/8/8/8/8/8/K7 w - - 0 1");
    CHECK(moves.makeMoveSinglePiece(board, board.getWQ(), "34QP", 'Q') == 16);

    board = Board::from("k7/8/8/8/8/8/3p4/K1R1R3 b - - 0 1");
    CHECK(moves.makeMoveSinglePiece(board, board.getBQ(), "33Qp", 'q') == 576460752303423488);

    board = Board::from("k7/8/8/8/8/8/3p4/K1R1R3 b - - 0 1");
    CHECK(moves.makeMoveSinglePiece(board, board.getBQ(), "32Qp", 'q') == 288230376151711744);

    board = Board::from("k7/8/8/8/8/8/3p4/K1R1R3 b - - 0 1");
    CHECK(moves.makeMoveSinglePiece(board, board.getBQ(), "34Qp", 'q') == 1152921504606846976);

    board = Board::initiateStandardChess();
    CHECK(moves.makeMoveSinglePiece(board, board.getWP(), "6444", 'P') == 67272588153323520);

    board = Board::from("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    board = moves.makeMoveAll(board, "7476");
    CHECK(board.boardString() == "[ r ][   ][ b ][ q ][ k ][   ][ n ][ r ]\n[ p ][ p ][ p ][ p ][   ][ p ][ p ][ p ]\n[   ][   ][ n ][   ][   ][   ][   ][   ]\n[   ][   ][ b ][   ][ p ][   ][   ][   ]\n[   ][   ][ B ][   ][ P ][   ][   ][   ]\n[   ][   ][   ][   ][   ][ N ][   ][   ]\n[ P ][ P ][ P ][ P ][   ][ P ][ P ][ P ]\n[ R ][ N ][ B ][ Q ][   ][ R ][ K ][   ]");

    board = Board::from("r2q1rk1/ppp2ppp/2npbn2/2b1p1B1/2B1P3/2NP1N2/PPPQ1PPP/R3K2R w KQ - 2 8");
    board = moves.makeMoveAll(board, "7472");
    CHECK(board.boardString() == "[ r ][   ][   ][ q ][   ][ r ][ k ][   ]\n[ p ][ p ][ p ][   ][   ][ p ][ p ][ p ]\n[   ][   ][ n ][ p ][ b ][ n ][   ][   ]\n[   ][   ][ b ][   ][ p ][   ][ B ][   ]\n[   ][   ][ B ][   ][ P ][   ][   ][   ]\n[   ][   ][ N ][ P ][   ][ N ][   ][   ]\n[ P ][ P ][ P ][ Q ][   ][ P ][ P ][ P ]\n[   ][   ][ K ][ R ][   ][   ][   ][ R ]");

    board = Board::from("rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 4");
    board = moves.makeMoveAll(board, "0406");
    CHECK(board.boardString() == "[ r ][ n ][ b ][ q ][   ][ r ][ k ][   ]\n[ p ][ p ][ p ][ p ][   ][ p ][ p ][ p ]\n[   ][   ][   ][   ][   ][ n ][   ][   ]\n[   ][   ][ b ][   ][ p ][   ][   ][   ]\n[   ][   ][ B ][   ][ P ][   ][   ][   ]\n[   ][   ][   ][   ][   ][ N ][   ][   ]\n[ P ][ P ][ P ][ P ][   ][ P ][ P ][ P ]\n[ R ][ N ][ B ][ Q ][   ][ R ][ K ][   ]");

    board = Board::from("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    CHECK(board.getCWK());
    board = moves.makeMoveAll(board, "7776");
    CHECK(!board.getCWK());

    board = Board::from("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    CHECK(board.getCWK());
    board = moves.makeMoveAll(board, "7475");
    CHECK(!board.getCWK());

    board = Board::from("r1b1k1nr/p1pq3p/1pnp1pp1/2b1p3/2B1P2N/2NPBP2/PPPQ2PP/R3K2R w KQkq - 2 10");
    CHECK(board.getCWQ());
    board = moves.makeMoveAll(board, "7071");
    CHECK(!board.getCWQ());

    board = Board::from("r1b1k1nr/p1pq3p/1pnp1pp1/2b1p3/2B1P2N/2NPBP2/PPPQ2PP/R3K2R w KQkq - 2 10");
    CHECK(board.getCWQ());
    board = moves.makeMoveAll(board, "7473");
    CHECK(!board.getCWQ());

    board = Board::from("r1b1k2r/p1pqn2p/1pnp1pp1/2b1p3/2B1P2N/2NPBPP1/PPPQ3P/2KR3R b kq - 0 11");
    CHECK(board.getCBK());
    board = moves.makeMoveAll(board, "0706");
    CHECK(!board.getCBK());

    board = Board::from("r3k2r/pbpqn2p/1pnp1ppB/2b1p3/2B1P2N/2NP1PP1/PPPQ3P/2KR3R b kq - 2 12");
    CHECK(board.getCBK());
    board = moves.makeMoveAll(board, "0405");
    CHECK(!board.getCBK());

    board = Board::from("r3k2r/pbpqn2p/1pnp1ppB/2b1p3/2B1P2N/2NP1PP1/PPPQ3P/2KR3R b kq - 2 12");
    CHECK(board.getCBQ());
    board = moves.makeMoveAll(board, "0001");
    CHECK(!board.getCBQ());

    board = Board::from("r3k2r/pbpqn2p/1pnp1ppB/2b1p3/2B1P2N/2NP1PP1/PPPQ3P/2KR3R b kq - 2 12");
    CHECK(board.getCBQ());
    board = moves.makeMoveAll(board, "0403");
    CHECK(!board.getCBQ());

    board = Board::from("r3k2r/pbpqn2p/1pnp1ppB/2b1p3/2B1P2N/2NP1PP1/PPPQ3P/2KR3R b kq - 2 12");
    CHECK(board.getEP() == 0);
    board = moves.makeMoveAll(board, "1030");
    CHECK(board.getEP() == 72340172838076673);

    board = Board::from("r3k2r/pbpqn2p/1p1p1ppB/n1b1p3/2B1P2N/2NP1PP1/PPPQ3P/2KR3R w kq - 3 13");
    CHECK(board.getEP() == 0);
    board = moves.makeMoveAll(board, "6141");
    CHECK(board.getEP() == 144680345676153346);

    board = Board::from("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    board = moves.makeMoveAll(board, "32QP");
    CHECK(board.getWQ() == 576460752303423492);

    board = Board::from("8/7p/p5pb/4k3/P1pPn3/8/P5PP/1rB2RK1 b - d3 0 28");
    CHECK(moves.pseudoLegalMovesB(board) == "20302636425223BE4423442544324436445244564463446527052716273627452754276327727101711171217131714171517161717071723423342434333443");
    board = moves.makeMoveAll(board, "23BE");
    CHECK(moves.unsafeForBlack(board) == 18224646162527297568U);

    board = Board::from("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
    board = moves.makeMoveAll(board, "65Qp");
    CHECK(board.getWN() == 9223372036854775808U);
    CHECK(board.getWQ() == 0);
    CHECK(board.getBQ() == 2305843009213693952U);
    CHECK(board.getWhiteToMove());

    board = Board::from("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
    board = moves.makeMoveAll(board, "67Qp");
    CHECK(board.getWN() == 2305843009213693952U);

    board = Board(134217728ULL, 0ULL, 0ULL, 16777216ULL, 0ULL, 1152921504606846976ULL, 1024ULL, 0ULL, 0ULL, 0ULL, 0ULL, 2147483648ULL, 0ULL, 0, 1, false, false, false, false, false);
    board = moves.makeMoveAll(board, "1232");
    CHECK(board.getWN() == 0);
    CHECK(board.getWB() == 0);
    CHECK(board.getWQ() == 0);
    CHECK(board.getBN() == 0);
    CHECK(board.getBB() == 0);
    CHECK(board.getBR() == 0);
    CHECK(board.getBQ() == 0);
    CHECK(board.getEP() == 289360691352306692U);

    board = Board(35747871798067200ULL, 4755801206503243776ULL, 2594073385365405696ULL, 9295429630892703744ULL, 576460752303423488ULL, 1152921504606846976ULL, 1073790720ULL, 66ULL, 36ULL, 129ULL, 8ULL, 16ULL, 4629771061636907072ULL, 0, 1, true, true, true, true, true);
    board = moves.makeMoveAll(board, "4736");
    CHECK(board.getEP() == 0);

    board = Board::from("r3k2r/p1ppqpb1/bnN2np1/3p4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 2");
    board = moves.makeMoveAll(board, "2214");
    CHECK(moves.pseudoLegalMovesB(board) == "5766334441521222132326363343415112322102214021422506251725372544254616051627200220112031204220532064000100020003070507060717072707370747040304050414");
}

TEST_CASE("Board to FEN works")
{
    Moves &moves = Moves::getInstance();
    Board board = Board::from("rnbqkbnr/ppppp2p/5p2/6pP/8/8/PPPPPPP1/RNBQKBNR w KQkq g6 0 3");
    CHECK(board.fen() == "rnbqkbnr/ppppp2p/5p2/6pP/8/8/PPPPPPP1/RNBQKBNR w KQkq g6 0 3");
    board = moves.makeMoveAll(board, "76WE");
    CHECK(board.fen() == "rnbqkbnr/ppppp2p/5pP1/8/8/8/PPPPPPP1/RNBQKBNR b KQkq - 0 3");
}

TEST_CASE("Move counter works")
{
    Moves &moves = Moves::getInstance();
    Board board = Board::initiateStandardChess();
    CHECK(board.getMoveNumber() == 1);
    board = moves.makeMoveAll(board, "6444");
    CHECK(board.getMoveNumber() == 1);
    board = moves.makeMoveAll(board, "1434");
    CHECK(board.getMoveNumber() == 2);
}

TEST_CASE("Half move clock works")
{
    Moves &moves = Moves::getInstance();
    Board board = Board::initiateStandardChess();
    CHECK(board.getHalfMoveClock() == 0);
    board = moves.makeMoveAll(board, "6444");
    CHECK(board.getHalfMoveClock() == 0);
    board = moves.makeMoveAll(board, "1434");
    CHECK(board.getHalfMoveClock() == 0);
    board = moves.makeMoveAll(board, "7655");
    CHECK(board.getHalfMoveClock() == 1);
    board = moves.makeMoveAll(board, "0122");
    CHECK(board.getHalfMoveClock() == 2);
    board = moves.makeMoveAll(board, "5534");
    CHECK(board.getHalfMoveClock() == 0);

    board = Board::from("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
    CHECK(board.getHalfMoveClock() == 4);
    board = moves.makeMoveAll(board, "7476");
    CHECK(board.getHalfMoveClock() == 5);

    board = Board::from("rnbqkbnr/3pp1pp/ppp5/4PpP1/8/8/PPPP1P1P/RNBQKBNR w KQkq f6 0 5");
    CHECK(board.fen() == "rnbqkbnr/3pp1pp/ppp5/4PpP1/8/8/PPPP1P1P/RNBQKBNR w KQkq f6 0 5");
    board = moves.makeMoveAll(board, "65WE");
    CHECK(board.fen() == "rnbqkbnr/3pp1pp/ppp2P2/4P3/8/8/PPPP1P1P/RNBQKBNR b KQkq - 0 5");

    board = Board::from("1rbqkbnr/pp1p2Pp/n1p5/8/4P3/8/PPP2PPP/RNBQKBNR w KQk - 1 6");
    CHECK(board.fen() == "1rbqkbnr/pp1p2Pp/n1p5/8/4P3/8/PPP2PPP/RNBQKBNR w KQk - 1 6");
    board = moves.makeMoveAll(board, "67NP");
    CHECK(board.fen() == "1rbqkbnN/pp1p3p/n1p5/8/4P3/8/PPP2PPP/RNBQKBNR b KQ - 0 6");
}

TEST_CASE("material rating works") {
    Rating &rating = Rating::getInstance();
    Board board = Board::initiateStandardChess();
    CHECK(rating.getCentipawnValueWhite(board) == 4977);
    CHECK(rating.getCentipawnValueBlack(board) == -4977);
    CHECK(rating.getCentipawnValue(board) == 0);

    board = Board::from("1k6/5P2/8/8/1P6/8/4P3/4K3 w - - 0 1");
    CHECK(rating.getCentipawnValueWhite(board) == 300);
}

TEST_CASE("endgame calculation works") {
    Moves &moves = Moves::getInstance();
    Board board = Board::initiateStandardChess();
    CHECK(board.getPhase() == OpeningMiddle);

    board = Board::from("8/8/8/2k5/2p5/8/7r/R1K5 w - - 0 1");
    CHECK(board.getPhase() == End);

    board = Board::from("r2qkb1r/ppp2ppp/8/3p4/3QP3/8/PPP2PPP/R3KB1R w KQkq - 0 6");
    CHECK(board.getPhase() == OpeningMiddle);
    board = moves.makeMoveAll(board, "3344");
    CHECK(board.getPhase() == OpeningMiddle);
    board = moves.makeMoveAll(board, "4344");
    CHECK(board.getPhase() == OpeningMiddle);
    board = moves.makeMoveAll(board, "0314");
    CHECK(board.getPhase() == OpeningMiddle);
    board = moves.makeMoveAll(board, "4414");
    CHECK(board.getPhase() == OpeningMiddle);
    board = moves.makeMoveAll(board, "0514");
    CHECK(board.getPhase() == End);

    board = Board::from("1kr5/1bb5/8/6PQ/8/8/4K3/8 w - - 0 1");
    CHECK(board.getPhase() == End);

    board = Board::from("1kq5/1b6/6B1/6PQ/8/2p5/4K3/8 w - - 0 1");
    CHECK(board.getPhase() == End);
}

TEST_CASE("checkmate works") {
    Moves &moves = Moves::getInstance();
    Board board = Board::from("rnbqkbnr/ppppp2p/5p2/6pQ/4P3/3P4/PPP2PPP/RNB1KBNR b KQkq - 1 3");
    CHECK(moves.isCheckmate(board));

    board = Board::from("rnbqkbnr/ppppp2p/5p2/6p1/4P3/3P4/PPP2PPP/RNBQKBNR w KQkq g6 0 3");
    CHECK(!moves.isCheckmate(board));

    board = Board::from("rnbqkbnr/pppp1Bp1/7p/4p3/4P3/8/PPPP1PPP/RNBQK1NR b KQkq - 0 3");
    CHECK(!moves.isCheckmate(board));
}

// TEST_CASE("algebraic to maestro int string conversion works") {
//     Moves &moves = Moves::getInstance();
//     Board board = Board::from("7k/8/3N4/5p2/8/4N3/8/7K w - - 0 1");
//     string input = "e3f5";
//     string output = moves.convertMoveToMaestroIntString(input, board);
//     CHECK(output == "5435");

//     board = Board::from("rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");
//     input = "e1g1";
//     output = moves.convertMoveToMaestroIntString(input, board);
//     CHECK(output == "7476");

//     board = Board::from("rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 4");
//     input = "e8g8";
//     output = moves.convertMoveToMaestroIntString(input, board);
//     CHECK(output == "0406");

//     board = Board::from("r3k2r/pppbqppp/2np1n2/2b1p3/2B1P3/2NP1N2/PPPBQPPP/R3K2R w KQkq - 2 8");
//     input = "e1c1";
//     output = moves.convertMoveToMaestroIntString(input, board);
//     CHECK(output == "7472");

//     board = Board::from("r3k2r/pppbqppp/2np1n2/2b1p3/2B1P3/2NP1N2/PPPBQPPP/2KR3R b kq - 3 8");
//     input = "e8c8";
//     output = moves.convertMoveToMaestroIntString(input, board);
//     CHECK(output == "0402");

//     board = Board::from("8/3P4/8/7k/8/7K/8/8 w - - 0 1");
//     input = "d7d8q";
//     output = moves.convertMoveToMaestroIntString(input, board);
//     CHECK(output == "33QP");

//     board = Board::from("4n3/3P4/8/7k/8/7K/8/8 w - - 0 1");
//     input = "d7e8q";
//     output = moves.convertMoveToMaestroIntString(input, board);
//     CHECK(output == "34QP");

//     board = Board::from("2n5/3P4/8/7k/8/7K/8/8 w - - 0 1");
//     input = "d7c8q";
//     output = moves.convertMoveToMaestroIntString(input, board);
//     CHECK(output == "32QP");
    
//     board = Board::from("8/8/8/7k/8/7K/3p4/8 b - - 0 1");
//     input = "d2d1q";
//     output = moves.convertMoveToMaestroIntString(input, board);
//     CHECK(output == "33Qp");
    
//     board = Board::from("8/8/8/7k/8/7K/3p4/4N3 b - - 0 1");
//     input = "d2e1q";
//     output = moves.convertMoveToMaestroIntString(input, board);
//     CHECK(output == "34Qp");
    
//     board = Board::from("8/8/8/7k/8/7K/3p4/2N5 b - - 0 1");
//     input = "d2c1q";
//     output = moves.convertMoveToMaestroIntString(input, board);
//     CHECK(output == "32Qp");
    
//     board = Board::from("7k/8/8/2Pp4/8/8/8/7K w - d6 0 2");
//     input = "c5d6";
//     output = moves.convertMoveToMaestroIntString(input, board);
//     CHECK(output == "23WE");
    
//     board = Board::from("7k/8/8/8/2pP4/8/8/7K b - d3 0 1");
//     input = "c4d3";
//     output = moves.convertMoveToMaestroIntString(input, board);
//     CHECK(output == "23BE");

//     board = Board::from("r1bqk2r/pppp1ppp/2n1pn2/8/2B1P3/2b2N2/PPPP1PPP/R1BQ1RK1 w kq - 0 6");
//     input = "d2c3";
//     output = moves.convertMoveToMaestroIntString(input, board);
//     CHECK(output == "6352");

//     board = Board::from("1k6/6KP/6P1/p4P1r/P5R1/8/8/8 w - - 2 56");
//     input = "h7h8q";
//     output = moves.convertMoveToMaestroIntString(input, board);
//     board = moves.makeMoveAll(board, output);
//     CHECK(board.fen() == "1k5Q/6K1/6P1/p4P1r/P5R1/8/8/8 b - - 0 5");
// }

// TEST_CASE("mestro int to algebraic string conversion works") {
//     Moves &moves = Moves::getInstance();
//     //Mate in 1, white to move
//     Board board = Board::from("rnbqkb1r/ppp2ppp/3p1n2/4p2Q/2B1P3/8/PPPP1PPP/RNB1K1NR w KQkq - 0 4");
//     pair<string, int> selectedMove = Search::minimax(board, 5, INT_MIN, INT_MAX, "", "", std::chrono::milliseconds(0));
//     string output = moves.convertMoveToAlgebraUCI(selectedMove.first, board);
//     CHECK(output == "h5f7");

//     // Mate in 2, white to move FAILING
//     board = Board::from("r5k1/Q4ppp/p1p1p3/6q1/4p3/8/PPP2PP1/4RRK1 w - - 1 20");
//     selectedMove = Search::minimax(board, 5, INT_MIN, INT_MAX, "", "", 0);
//     output = moves.convertMoveToAlgebraUCI(selectedMove.first, board);
//     CHECK(output == "a7a8");

//     //Mate in 2, black to move FAILING
//     board = Board::from("4rrk1/ppp2pp1/8/4P3/6Q1/P1P1P3/q4PPP/R5K1 b - - 0 1");
//     selectedMove = Search::minimax(board, 5, INT_MIN, INT_MAX, "", "", 0);
//     output = moves.convertMoveToAlgebraUCI(selectedMove.first, board);
//     CHECK(output == "a2a1");

//     //Mate in 3, white to move FAILING
//     board = Board::from("r5rk/5p1p/5R2/4B3/8/8/7P/7K w - - 0 0");
//     selectedMove = Search::minimax(board, 5, INT_MIN, INT_MAX, "", "", 0);
//     output = moves.convertMoveToAlgebraUCI(selectedMove.first, board);
//     CHECK(output == "f6a6");

//     //Mate in 4, black to move
//     board = Board::from("Q7/p1p1q1pk/3p2rp/4n3/3bP3/7b/PP3PPK/R1B2R2 b - - 0 1");
//     selectedMove = Search::minimax(board, 5, INT_MIN, INT_MAX, "", "", 0);
//     output = moves.convertMoveToAlgebraUCI(selectedMove.first, board);
//     CHECK(output == "h3g2");

//     //Mate in 5, black to move
//     board = Board::from("6k1/3b3r/1p1p4/p1n2p2/1PPNpP1q/P3Q1p1/1R1RB1P1/5K2 b - - 0-1");
//     selectedMove = Search::minimax(board, 5, INT_MIN, INT_MAX, "", "", 0);
//     output = moves.convertMoveToAlgebraUCI(selectedMove.first, board);
//     CHECK(output == "h4f4");

//     //Mate in 2, white to move
//     board = Board::from("2bqkbn1/2pppp2/np2N3/r3P1p1/p2N2B1/5Q2/PPPPKPP1/RNB2r2 w KQkq - 0 1");
//     selectedMove = Search::minimax(board, 4, INT_MIN, INT_MAX, "", "", 0);
//     output = moves.convertMoveToAlgebraUCI(selectedMove.first, board);
//     CHECK(output == "f3f7");
//     Moves &moves = Moves::getInstance();
//     //Mate in 1, white to move
//     Board board = Board::from("5rk1/5p1p/6n1/2Q1p3/1P3bB1/3P4/2Pq4/5RK1 w - - 0 34");
//     pair<string, int> selectedMove = Search::iterativeDeepeningSearch(board, 5, std::chrono::milliseconds(54500));
//     string output = moves.convertMoveToAlgebraUCI(selectedMove.first, board);
//     CHECK(output == "h5f7");
// }

// TEST_CASE("iterative deepening works") {
//     Moves &moves = Moves::getInstance();
//     Board board = Board::from("rnbqkb1r/ppp2ppp/3p1n2/4p2Q/2B1P3/8/PPPP1PPP/RNB1K1NR w KQkq - 0 4");
//     pair<string, int> selectedMove = Search::iterativeDeepeningSearch(board, 3, std::chrono::milliseconds(100000));
//     string output = moves.convertMoveToAlgebraUCI(selectedMove.first, board);
//     CHECK(output == "h5f7");

//     board = Board::from("rnbqkb1r/ppp2ppp/3p1n2/4p2Q/2B1P3/8/PPPP1PPP/RNB1K1NR w KQkq - 0 4");
//     selectedMove = Search::iterativeDeepeningSearch(board, 3, std::chrono::milliseconds(100000));
//     output = moves.convertMoveToAlgebraUCI(selectedMove.first, board);
//     CHECK(output == "h5f7");

//     board = Board::from("r1bqkbnr/2pp1ppp/p3p3/np6/4P3/1B1P1N2/PPP2PPP/RNBQ1RK1 b kq - 0 6");
//     selectedMove = Search::iterativeDeepeningSearch(board, 5, std::chrono::milliseconds(7283));
//     output = moves.convertMoveToAlgebraUCI(selectedMove.first, board);
//     CHECK(output == "h5f7");

//     board = Board::from("r4b1r/p1pk3p/2p2p2/6p1/2pP4/P2bB3/3N1P1P/R1R3K1 w - - 2 18");
//     selectedMove = Search::iterativeDeepeningSearch(board, 5, std::chrono::milliseconds(963730));
//     output = moves.convertMoveToAlgebraUCI(selectedMove.first, board);
//     CHECK(output == "h5f7");
    
// }