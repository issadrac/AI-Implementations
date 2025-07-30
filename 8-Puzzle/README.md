# 8-Puzzle Solver using A* Search

This is a C++ implementation of the **8-Puzzle Solver** using the **A\* (A-Star) Search Algorithm**

It allows solving puzzles either manually entered or randomly generated, and also supports batch testing using pre-defined depth limited puzzle sets.

## Heuristics

1. **Misplaced Tiles**  
   Counts the number of tiles not in their goal position.

2. **Manhattan Distance**  
   Sum of the distances of the tiles from their goal positions (in grid moves).

## Modes

- **Random Puzzle Mode**: Generates a random solvable puzzle.
- **Manual Mode**: User inputs each tile manually.
- **Test File Mode (Option 5)**: Automatically loads 100 puzzles of a specified depth (`Length<depth>.txt`) and computes average runtime and cost for each heuristic.

### How to Run
Compile with all the necessary files (If not using predefined depths, then only source1.cpp is necessary) and Run the program
