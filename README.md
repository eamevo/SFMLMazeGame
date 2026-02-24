# SFMLMazeGame
SFML Based Maze Game for Lab 1 ECE 4122 @ GaTech

Requires SFML 2.5.1+.
February 16, 2026.
Enam Amevo
ECE 4122

I set up my CMakeList to put the font into the same directory as the executable: build/output/bin

If for some reason it does not work, you can find my font in the HW1 folder. It is PressStart2P.ttf. It's in the folder main level and also in a folder called fonts.

If my Cmake method doesn't work, copy the font into your build/output/bin folder manually.

Folder Structure should be:

SFMLMazeGame
- HW1
- SFML
- CMakeLists.txt
- README.md
  
Compilation Instructions:
1. in top level folder, run mkdir build && cd build
2. cmake ..
3. cmake --build .
4. ./output/bin/HW1 (optional width) (optional height)

example inputs: ./output/bin/HW1 50 40 would create a 51 x 41 maze.
