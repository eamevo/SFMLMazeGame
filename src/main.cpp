/*
Author: Amevo Enam
Class: ECE4122
Last Date Modified: February 16, 2026
Description: Main for Maze game. 
Handles SFML window, user input, and rendering.
*/

#include <SFML/Graphics.hpp>
#include "MazeGenerator.h"
#include "Pathfinder.h"
#include <iostream>
#include <iomanip>
#include <string>

struct ResetException : public std::exception {};
// using this struct to handle reset/gen during BFS

// UI Updater function
/**
     * @brief UI Updater
     * @param sizeAlgo reference to sf::Text object for left column (Size, Algorithm)
     * @param stats reference to sf::Text object for middle column (Statistics, Status)
     * @param controls reference to sf::Text object for right column (Controls and Speed)
     * @param maze read-only reference to current maze object
     * @param status string stating status: solving, reset, generating
     * @param path length of found path. -1 if no path found yet.
     * @param nodes number of nodes explored by BFS
     * @param delay read-only reference to delay float from main
     * 
     */
void updateStatus(sf::Text& sizeAlgo, sf::Text& stats, sf::Text& controls, const Maze& maze, const std::string status, int path, int nodes, const float& delay) {
    
    
    // left column: size and algorithm
    std::string height = std::to_string(maze.getHeight());
    std::string width = std::to_string(maze.getWidth());
    std::string sizeStr = "SIZE: " + width + "x" + height;
    sizeStr += "\n\nALGORITHM: BFS"; 
    sizeAlgo.setString(sizeStr); // update the string from main

    // middle column: statisics and status
    std::string statsStr = "STATUS: " + status;
    statsStr += "\n\nNODES EXPLORED: " + std::to_string(nodes);
    statsStr += "\n\nPATH LENGTH: " + (path == -1 ? "N/A" : std::to_string(path));
    stats.setString(statsStr); // update the string from main

    // right column: controls
    int delayMS = static_cast<int>(delay * 1000);
    std::string controlStr = "G:   GENERATE\n\nS:   SOLVE\n\nR:   RESET\n\n";
    controlStr += "+/-: SPEED (" + std::to_string(delayMS) + "ms)\n\n";
    controlStr += "ESC: EXIT";
    controls.setString(controlStr); // update the string from main
};


// Terminal Argument handler
/**
 * @brief Parses terminal arguments, makes sure values are between 10 and 100.
 * @param argc count of arguments passed into main
 * @param argv vector of arg strings from main, treated as an array of pointers to chars
 * @param width reference to width variable to be updated
 * @param height reference to height variable to be updated
 */
void handleArguments(int argc, char* argv[], int& width, int&height){

    //default values
    width = 25;
    height = 25;

    //check user input
    if (argc >= 3) //this means user passed program name, width, height
    {
        try
        {
        	// from vector of inputs, input @ index 1 should be width
            // input @ index 2 should be height.
            // should be convertable to integers
            int argWidth = std::stoi(argv[1]);
            int argHeight = std::stoi(argv[2]);

            // if we got to here, they were convertable
            // keep width and height in range (10-100)
            // pick smaller value between num and 100, thne pick larger value between 10 and num
            argWidth = std:: max(10, std::min(argWidth, 100));
            argHeight = std:: max(10, std::min(argHeight, 100));

            // update global width and height
            width = argWidth;
            height = argHeight;
        }
        catch(const std::exception& e)
        {
            // this should only run if they typed something that isnt a number
            // most likely a word as doubles and floats would autoconvert
            std::cerr << "Invalid arguments detected. Defaulting to 25x25." << std::endl;
        }  
    }
}

// maze renderer
/**
 * @brief Draws the maze grid to the screen based on current window size.
 * @param window The SFML window object we draw on
 * @param maze The maze data structure
 * @param topOffset height of black UI bar at the top
 */

 void drawMaze(sf::RenderWindow& window, const Maze& maze, float topOffset)
 {
    // grab the window's size in pixels
    sf::Vector2u windowSize = window.getSize(); // gives us mathematical vector object holding x and y
    float windowWidth = static_cast<float> (windowSize.x);
    float windowHeight = static_cast<float> (windowSize.y);
    // make them floats for max accuracy

    //calculate cell width and height
    // to keep cells proportional, divide width of window by width of maze
    float cellWidth = windowWidth / maze.getWidth();
    // subtract top bar offset from window height then divide by maze height
    float cellHeight = (windowHeight - topOffset) / maze.getHeight();

    sf::RectangleShape cellObj(sf::Vector2f(cellWidth, cellHeight));
    // SFML rectangle shape object with given height and width

    for (int row = 0; row < maze.getHeight(); row++)
    {
    	for (int col = 0; col < maze.getWidth(); col++)
        {
            //set position
            cellObj.setPosition( col * cellWidth, topOffset + (row * cellHeight));

            // get cell type
            CellType cellType = maze.getCell(row, col).type;

            switch (cellType) //assign color baed on type of cell
            {
                case CellType::Wall:
                    cellObj.setFillColor(sf::Color(50, 50, 50)); //dark gray
                    break;
                case CellType::Path:
                    cellObj.setFillColor(sf::Color(255, 255, 255)); //whote
                    break;
                case CellType::Start:
                    cellObj.setFillColor(sf::Color(0, 200, 0)); //green
                    break;
                case CellType::End:
                    cellObj.setFillColor(sf::Color(200, 0, 0)); //red
                    break;
                case CellType::Visited:
                    cellObj.setFillColor(sf::Color(173, 216, 230)); //light blue
                    break;
                case CellType::Solution:
                    cellObj.setFillColor(sf::Color(255, 255, 0)); //yellow
                    break;
                default: //default tp path
                    cellObj.setFillColor(sf::Color(255, 255, 255));
                    break;
            }
            window.draw(cellObj);
        }
    }
 }


// handler for R, G, Escape
 /**
 * @brief Handles some user keyboard input for maze interaction.
 * @param event The SFML event object (specifically key presses)
 * @param window render window (close if Escape is hit)
 * @param maze The maze object to modify based on input
 * @param sizeAlgo reference to sf::Text object for left column (Size, Algorithm)
 * @param stats reference to sf::Text object for middle column (Statistics, Status)
 * @param controls reference to sf::Text object for right column (Controls and Speed) * 
 * @param delay read-only reference to delay float from main
 * @param status reference to string holding status
 * @param pathLength reference to int holding path length
 * @param nodes reference to int holding nodes explored
 */
void keyBoardHandler(sf::Event& event, sf::RenderWindow& window, Maze& maze, sf::Text& sizeAlgo, sf::Text& stats, sf::Text& controls, const float& delay, std::string& status, int& pathLength, int& nodes)
{
    //if the event is a keypress
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            //16. Press 'Escape' to exit the application
            window.close();
        }

        if (event.key.code == sf::Keyboard::R)
        {
            //15. Press 'R' to reset the current maze (clear path visualization)
            maze.resetVisualization();
            pathLength = -1;
            nodes = 0; //reset
            status = "Maze Reset!";
            //updateStatus(sizeAlgo, stats, controls, maze, status, pathLength, nodes, delay);

        }

        if (event.key.code == sf::Keyboard::G)
        {
            //13. Press 'G' to generate a new random maze
            maze.generate();
            maze.resetVisualization(); //clear colors on new maze
            pathLength = - 1;
            nodes = 0; //reset
            status = "New Maze!";
            //updateStatus(sizeAlgo, stats, controls, maze, status, pathLength, nodes, delay);
        }
    }
}

// handled UI updating, terminal arguments, and pressing Esc,R,G outside of main.
// now main will be shorter as I only handle solving and editing delay (requires cont. polling).
int main(int argc, char* argv[]) {

    // set up variables
    int pathLength = -1; //holds path length
    bool isSolving = false; //flag denoting solve state
    std::string status = "READY"; //holds status string
    int nodes = 0; // nodes explored

    // parse inputs and setup maze
    int width, height;
    handleArguments(argc, argv, width, height);

    // initialize and generate maze with our set width and height
    Maze maze (width, height);
    maze.generate();
    float delayTime = 0.05f; //default delay 50 ms

    // initialize window and font
    // default size 800 x 600 but user can resize
    sf::RenderWindow window(sf::VideoMode(800,600), "ECE 4122 Maze Solver!", sf::Style::Default);
    sf::Font font;
    bool validFont = font.loadFromFile("output/bin/PressStart2P.ttf"); //CMakeLists will have already copied this to the specified location
    // will be true if the font loads
    if (!validFont)
    {
        std::cerr << "Font not found! Text will not display." << std::endl;
    }

    // more UI setup
    float barRatio = 0.25f; //the top bar will be 25 percent of screen height
    sf::RectangleShape topBar;
    topBar.setFillColor(sf::Color(0, 0, 0, 200)); // param 4 is opacity, this is partially transparent

    //setup status text!
    sf::Text sizeAlgo, stats, controls;
    if (validFont)
    {
      sizeAlgo.setFont(font);
      sizeAlgo.setCharacterSize(14);
      sizeAlgo.setFillColor(sf::Color::Red);

      stats.setFont(font);
      stats.setCharacterSize(14);
      stats.setFillColor(sf::Color::Green);

      controls.setFont(font);
      controls.setCharacterSize(14);
      controls.setFillColor(sf::Color::Yellow);
    }


    //initial status
    updateStatus(sizeAlgo, stats, controls, maze, status, pathLength, nodes, delayTime);


    //lambda function/code snippet that handles visuals dynamically
    auto renderer = [&](bool wait) {
        window.clear(); //clear previous frame

        //update UI every frame to reflect real time updating of speed and node count
        updateStatus(sizeAlgo, stats, controls, maze, status, pathLength, nodes, delayTime);

        // continous polling for adjusting delay time (allows holding)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add) || sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
        // add and equal share a key
        {
            if (delayTime > 0.001f)
            {
            	delayTime -= 0.001f; //decrease delay, speed up
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) || sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
        // subtract and dash share a key
        {
            if (delayTime < 1.0f)
            {
            	delayTime += 0.001f; //increase delay, slow down
            }
        }

        // check for reset while bfs is running
        // this allows the user to reset or generate mid solve.
        if (isSolving)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) || sf::Keyboard::isKeyPressed(sf::Keyboard::G)) 
            {
                throw ResetException();
            }
        }



        //calculate dynamic height using current window size
        sf::Vector2u winSize = window.getSize();
        float winWidth = static_cast<float>(winSize.x);
        float winHeight = static_cast<float>(winSize.y);
        float barHeight = winHeight * barRatio;
        

        // draw maze and top bar
        drawMaze(window, maze, barHeight);
        topBar.setSize(sf::Vector2f(winWidth, barHeight));
        window.draw(topBar);

        if (validFont)
        // waste of computational power if font doesnt load
        {
            //left col holds size and algo
            sizeAlgo.setPosition(20, 10);
            window.draw(sizeAlgo);

            //middle col holds stats
            //calculate center
            float statsXPos = (winWidth / 2.0f) - (stats.getGlobalBounds().width / 2.0f);
            stats.setPosition(statsXPos, 10);
            window.draw(stats);

            //right col holds controls
            float controlXPos = winWidth - controls.getGlobalBounds().width - 20;
            controls.setPosition(controlXPos, 10);
            window.draw(controls);
        }

        window.display();
        

        if (wait) {
            // this secondary polling keeps the window responsive
            // handles exit and resize logic during BFS
            sf::Event e;
            while (window.pollEvent(e))
            {
                if (e.type == sf::Event::Closed || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape))
                {
                    //18. Handle window close event properly
                    //allow user to exit even during BFS
                    window.close();
                }
                if (e.type == sf::Event::Resized)
                {
                    // if we resize the window, update view
                    // 9. Use SFML 2.5+ to render the maze in a resizable window
                    // sf::FloatRect(0,0,width,height) -> defines 2D rectangle with new dimensions of window
                    // sf::View(...) -> update the view with new window dimensions
                    window.setView(sf::View(sf::FloatRect(0, 0, (float)e.size.width, (float)e.size.height)));
                }
            }
            // pause execution so we can actually watch the solving
            sf::sleep(sf::milliseconds(static_cast<int>(delayTime * 1000)));
        }
    };


    // main input loop

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
			{
				//18. Handle window close event properly
				window.close();
			}
			if (event.type == sf::Event::Resized)
			{
				// if we resize the window, update view
				// 9. Use SFML 2.5+ to render the maze in a resizable window
				// sf::FloatRect(0,0,width,height) -> defines 2D rectangle with new dimensions of window
				// sf::View(...) -> update the view with new window dimensions
				window.setView(sf::View(sf::FloatRect(0, 0, (float)event.size.width, (float)event.size.height)));
			}

			// custom handler for R, G, escape
			keyBoardHandler(event, window, maze, sizeAlgo, stats, controls, delayTime, status, pathLength, nodes);

			// S logic
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
			{
				try
				{
					isSolving = true;
					pathLength = -1; //reset
					status = "SOLVING...";
					maze.resetVisualization();

					auto path = solveBFS(maze, renderer, delayTime, nodes);
					isSolving = false;
					if (!path.empty())
					{
						status = "SOLVED!"; //update status string
						pathLength = static_cast<int>(path.size()); //update path length
					}
				}
				catch (const ResetException& e)
				{
					isSolving = false; //interrupted, reset flag
					maze.resetVisualization();
					pathLength = -1;
					nodes = 0;
					
					// manually handle reset logic so user doesnt have to click key twice for response
					// without this, first click of R/G pauses solving, second click works

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
					{
						status = "Maze Reset!";
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
					{
						maze.generate();
						status = "New Maze!";
					}
				}
			}
        }

        // screen refreshing
        // call renderer(false) for no wait/sleep when just idle
        renderer(false);
    }

    return 0;
}