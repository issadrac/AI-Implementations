# Connect Four AI using Alpha-Beta Pruning

This is a C++ implementation of **Connect Four (8x8)** where a human plays against a computer opponent.

The computer uses **Alpha-Beta Pruning** combined with **Iterative Deepening Search (IDS)** and a custom heuristic evaluation to make decisions within a 5-second time limit (Given a longer time limit, the AI makes better moves).

## AI Strategy

- **Minimax with Alpha-Beta Pruning** - The main driver in the AI's evalution process
- **Iterative Deepening (up to depth 5)** - Allows for better descions the longer it runs
- **Heuristic Evaluations** - Better board positions, how many in a row, certain patterns, and blocks.
- **Pattern detection** for 2 or 3 in four configurations.
- **Time-limited decision making** - AI only has a 5-second time limit to respond with a move. If changed to a longer time, the AI plays better.

## Game Rules

- 8x8 board
- Players alternate placing a piece
- 4 in a row **horizontally or vertically** wins
- Game ends in a draw if board fills

## How to Run

Compile Source1.cpp and run
