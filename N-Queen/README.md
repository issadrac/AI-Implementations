# N-Queens Solver using Hill Climbing & Genetic Algorithm

This is a C++ implementation of the **N-Queens problem** solver, where the goal is to place **N queens on an N×N chessboard** so that no two queens attack each other.

The program utilizess two algorithms:

**Hill Climbing** — a local search heuristic that iteratively improves solutions by minimizing conflicts.
**Genetic Algorithm** — an evolutionary approach that uses populations, reproduction, mutation, and selection to find solutions.

## Solver Strategy

**Hill Climbing:**
This algorithm starts with a random board configurations and moves queens within rows. The move chosen is the one that results in the least number of conflicts. It stops when the sucessor is not better than the predecessor. Each attempt is not guaranteed to result in solved board. 

**Genetic Algorithm:**
This algorithm starts with a population of board states and selects parents based on fitness which counts the number of non-attacking pairs. However, it doesn't choose the absolute best parents because "genetic diversity" is needed for the algorithm to work. The offsprings are created through crossover and they occasionaly mutate (A queen was moved to a different row). It runs up to 1000 generatiosn and stops when a solution is found. The program finds multiple solutions for several populations. 

## Heuristic Evaluation

**Hill Climbing:** Calculates the number of conflicting queen pairs.

**Genetic:** Fitness is defined as the **maximum number of non-conflicting pairs** possible minus the number of conflicts.

The goal is to minimize conflicts to zero (valid solutions).

## Problem Constraints

Place N queens on an N×N board.
No two queens may share the same row, column, or diagonal.
The solution is valid when zero conflicts remain.

## How to Run
Compile the source code and run.

