# Maestro
Maestro is a chess engine written in C++ and maintained by David Chopin. The engine is developed with TDD principles and utilizes a number of common chess engine concepts, such as:
- Bitboards
- Minimax search
- Alpha-beta pruning
- Iterative deepening
- Piece-square tables

TODO:
- Implement transposition tables via Zobrist hashing
- Improve move generation efficiency
- Refactor moves into objects from their current, inefficient String state
- Implement an opening book
- Implement engame tables
- Implement move pondering
- Probably some other stuff as well

Feel free to fork/submit PRs. I may merge some of them in (haven't really decided if I want to do that yet or not).