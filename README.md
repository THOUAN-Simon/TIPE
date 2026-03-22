# TIPE
TIPE is a project is a two-year project that students in preparatory classes must present as part of the admissions exam. It focuses on a specific topic (in this case, “Sports and Games”) and gives students a glimpse into the work of a reasercher. My TIPE was about game theory, searching an optimal strategy for the board game "blokus"

# TIPE Project  - Optimal Strategy for "Blokus" (Game Theory & AI - Scientific Research - Classe Préparatoire)

## Overview
This project focuses on the mathematical and computational analysis of the board game Blokus. The objective was to design and implement an algorithm capable of testing different strategy in the game, to see which one perform the best, using specific ideas (eg. control of territory, defense, ... etc) implemented as heuristic in a minimax algorithm.

Developed as part of my two-year intensive research project (TIPE) under the 2023-2024 theme: "Sports and Games".

## Key Features
Game Engine: Complete implementation of Blokus rules (piece placement, collision detection, scoring).

- Algorithms: Implementation of decision-making models (the objective was Minimax, Alpha-Beta Pruning)

- Heuristics: Developed custom evaluation functions based on territory control, piece remaining, and mobility.

- Performance: Optimized move generation to handle the game's high branching factor.

## Scientific Approach

- Modeling: Abstracting the game board and pieces into efficient data structures.
  
- Strategy: The plan was to use a minimax approach so that two computers can play against each other, and in order to evaluate different strategies. Ultimately, due to a lack of time and the increasing complexity of the implementation, the code currently consists of a number of functions that would be useful for implementing certain strategies, such as gaining space on the opponent or gaining space on the field

- Analysis: Testing the strategies some against others to measure win rates of each in different situations.

## Tech Stack
- Language: C
- Libraries: stdlib


