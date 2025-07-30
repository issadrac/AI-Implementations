# Connect Four AI using Alpha-Beta Pruning

This is a C++ implementation of **Connect Four (8x8)** where a human plays against a computer opponent.

The computer uses **Alpha-Beta Pruning** combined with **Iterative Deepening Search (IDS)** and a custom heuristic evaluation to make intelligent decisions within a 5-second time limit (Given a longer time limit, the AI makes better moves).

## AI Strategy

- **Minimax with Alpha-Beta Pruning**
- **Iterative Deepening (up to depth 5)**
- **Position-based scoring**
- **Pattern detection** for 2-in-a-row and 3-in-a-row configurations
- **Time-limited decision making**

## Game Rules

- 8x8 board
- Players alternate placing a piece
- 4 in a row **horizontally or vertically** wins
- Game ends in a draw if board fills

## How to Run

Compile Source1.cpp and run
