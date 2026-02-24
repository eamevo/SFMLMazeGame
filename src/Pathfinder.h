/*
Author: Enam Amevo
Class: ECE4122
Last Date Modified: February 16, 2026
Description: This header defines the Position struct and the necessary
functions to solve the maze and reconstruct shortest path.
Based on Pathfinding Notes!
*/

/**
* @file Pathfinder.h
* @brief Header file for pathfinder implementation.
* @course ECE 4122/6122 - Homework 1
* 
* This header defines the Position struct and the necessary
functions to solve the maze and reconstruct shortest path.
*/



#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector> //List
#include <queue> //Queue
#include <unordered_map> //Map
#include "MazeGenerator.h" // Maze def
#include <functional> //so we can do the renderer thing


//types

/**
 * @struct Position
 * @brief represents a single coordinate (row/column) in the maze.
 */
struct Position {
    int row, col;
    
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

/**
 * @struct PositionHash
 * @brief Custom hash function allows Position to be used as a key in std::unordered_map.
 */
struct PositionHash {
    size_t operator()(const Position& p) const {
        return std::hash<int>()(p.row) ^ (std::hash<int>()(p.col) << 16);
    }
};

//functions

/**
 * @brief Finds the shortest path from start to end using BFS.
 * @param goalMaze reference to a maze object to solve.
 * @param renderer function from main which handles SFML rendering.
 * @param delayTime chosen delay time for animation.
 * @param nodes number of nodes explored
 * @return A vector of Position structs representing the path.
 */
std::vector<Position> solveBFS(Maze& goalMaze, std::function<void(bool)> renderer, float& delayTime, int& nodes);

/**
 * @brief Traces end to start using a map of all cells traversed.
 * @param cellMap map containing the cells before each traversed cell.
 * @param end position to begin traceback from.
 * @param goalMaze reference to the maze object to solve.
 * @return A vector of Position structs representing the path in ORDER from start pos to end.
 */
std::vector<Position> reconstructPath(std::unordered_map<Position, Position, PositionHash>& cellMap, Position end, Maze& goalMaze);

#endif
