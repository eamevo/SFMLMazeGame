/*
Author: Enam Amevo
Class: ECE4122
Last Date Modified: February 15, 2026
Description: This file implements the necessary functions 
to solve the maze using BFS and reconstruct shortest path.
*/

/**
 * @file Pathfinder.cpp
 * @brief Implementation of maze solving using BFS!
 * @course ECE 4122/6122 - Homework 1
 * 
 * BFS works as follows:
 * 1. Add start position to queue, mark it as visited.
 * 2. While Queue is not empty:
 *    a. Dequeue the current position
 *    b. If current pos is the End, trigger path reconstruction.
 *    c. For each valid neighbor one unit away in cardinal directions:
 *       - if neighbor is in path/is an end, AND hasn't been visited,
 *       - mark neighbor as visited
 *       - record current cell as predecessor in cellMap
 *       - add neighbor to queue.
 */

#include "Pathfinder.h"
#include <algorithm>

std::vector<Position> solveBFS(Maze& goalMaze, std::function<void(bool)> renderer, float& delayTime, int& nodes)
{
    nodes = 0; //reset count

    //get start and end
    Position start = {goalMaze.getStart().first, goalMaze.getStart().second};
    Position end = {goalMaze.getEnd().first, goalMaze.getEnd().second};
    
    //setup FIFO queue
    std::queue<Position> frontier;
    std::unordered_map<Position, Position, PositionHash> cellMap; //current pos : prev pos, allows retracing
    std::unordered_map<Position, bool, PositionHash> visited; //cells already traversed
    
    frontier.push(start);
    visited[start] = true;
    nodes ++;
    cellMap[start] = {-1, -1};  // Sentinel value for start
    
    // Direction vectors: up, down, left, right
    const int dr[] = {-1, 1, 0, 0}; // delta row
    const int dc[] = {0, 0, -1, 1}; // delta column
    
    while (!frontier.empty()) //while queue not empty
    {
        Position current = frontier.front(); // grab oldest item in queue (FIFO)
        frontier.pop(); // remove it
        
        // Goal check
        if (current == end) // if we're at the end, reconstruct
        {
            return reconstructPath(cellMap,end, goalMaze);
        }
        
        //if we're here, we're not at the end
        //at our current pos, let's explore neighbors
        for (int i = 0; i < 4; ++i) 
        {
            Position next = {current.row + dr[i], current.col + dc[i]};
            
            if (goalMaze.isValidPath(next.row, next.col) && !visited[next]) 
            // if the next row and column are valid paths and we havent visited yet
            {
                //lets mark next as visited and update queue
                visited[next] = true;
                nodes++;
                cellMap[next] = current;
                frontier.push(next);
                
                // Mark for visualization (optional)
                if (goalMaze.getCell(next.row, next.col).type == CellType::Path) 
                {
                    goalMaze.setCellType(next.row, next.col, CellType::Visited);
                }

                if (renderer)
                {
                    renderer(true);
                }
            }
        }
    }
    return {};  // No path found
}

std::vector<Position> reconstructPath(std::unordered_map<Position, Position, PositionHash>& cellMap, Position end, Maze& goalMaze) {
            std::vector<Position> path;
            Position pos = end;
            while (!(pos.row == -1 && pos.col == -1)) //loop backwards until we hit the start @ {-1. -1}
            {
                path.push_back(pos); //add position to end path
               
                // if position is in bounds, and not start/end
                if (goalMaze.isInBounds(pos.row, pos.col) && 
                    goalMaze.getCell(pos.row, pos.col).type != CellType::Start && 
                    goalMaze.getCell(pos.row, pos.col).type != CellType::End)
                  {
                    goalMaze.setCellType(pos.row, pos.col, CellType::Solution);
                    // then its in the solution
                  }
                // move to previous cell in the reconstructed trail
                pos = cellMap[pos];
            }
            std::reverse(path.begin(), path.end()); //BFS finds path backwards, reverse it!
            return path;
}