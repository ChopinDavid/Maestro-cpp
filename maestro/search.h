#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED
#include <unordered_map>
#include <chrono>
#include "zobrist.h"
#include "moves.h"
#include "rating.h"
using namespace std;
using namespace std::chrono;
class Search
{
private:
    static Zobrist zobrist;
    static std::unordered_map<uint64_t, vector<pair<string, int>>> hashMap;

public:
    static std::pair<string, int> minimax(Board node, int depth, int alpha, int beta, string move, string firstMove, milliseconds breakOffTime);

    static std::pair<string, int> iterativeDeepeningSearch(Board initialPosition, int maxDepth, milliseconds maxSearchTime);

    static std::vector<std::string> split(const std::string &str, int splitLength);
};
#endif // SEARCH_H_INCLUDED