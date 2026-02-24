# SFMLMazeGame
SFML Based Maze Game for Lab 1 ECE 4122 @ GaTech

Requires SFML 2.5.1+.
February 16, 2026.
Enam Amevo
ECE 4122

# Overview

A C++ application that procedurally generates a maze and solves it using Breadth-First Search. This project uses **SFML 2.5.1+** for real-time rendering and includes dynamic speed controls and window resizing.

# Prerequisites

Download SFML [here](https://github.com/jhurley-blip/ECE4122-6122-SFML/tree/master/SFML).

Clone this repo.

# Project Structure
```text            
SFMLMazeGame/
├── SFML/                 # SFML library files
├── src/                  # Source files (.cpp, .h)
│   ├── CMakeLists.txt  
│   ├── main.cpp
│   ├── MazeGenerator.h
│   ├── MazeGenerator.cpp
│   ├── Pathfinder.h
│   ├── Pathfinder.cpp
│   └── fonts/
│       └── PressStart2P.ttf
├── README.md
└── CMakeLists.txt     # Top Level Build configuration 
```
  
# Compilation and Execution
1. In top level folder/root directory, create build folder:
```
mkdir build && cd build
```
3. Configure and build:
```
cmake ..
cmake --build .
```
3. Run Application:
```
./output/bin/MazeGame [width] [height]
```

Note: Default size is 25 x 25. Parameters are optional. For example, 50 40 creates a 51x41 maze.

# Controls
G: Generate new maze

S: Solve maze (BFS)

R: Reset visualization

+/-: Adjust simulation speed

ESC: Exit


