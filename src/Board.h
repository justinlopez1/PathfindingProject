//
// Created by funny on 11/28/2023.
//

#ifndef PATHFINDINGPROJECT_BOARD_H
#define PATHFINDINGPROJECT_BOARD_H

#pragma once
#include <vector>
#include <iostream>
#include <queue>
#include <limits>
#include <string>
#include <cmath>
#include "SFML/Graphics.hpp"
#include <fstream>

#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 900
#define BOARD_LEN 900

class Board {
private:
    struct Cell {
        sf::Text t;
        sf::RectangleShape tile;
        bool isStart;
        bool isFinish;
        bool isWall;
        bool visited;
        bool isPath;
        int x;
        int y;
        std::vector<Cell*> nearbyCells;
        Cell* prev;

        float GBFSdistance;
        void setGBFSDistance(Cell* finish);

        float dijkstraDistance;

        bool aStarVisited = false;

        Cell(int x, int y, float sideLength);
        void draw(sf::RenderWindow &window);
        void draw(sf::RenderWindow &window, sf::Font* font);
    };

    std::vector<std::vector<Cell>> cells;
    std::vector<sf::RectangleShape> borders;

    int boardLength;
    float cellSideLength;
    bool finished;
    bool solving;
    Cell* start;
    Cell* finish;

    sf::Font font;
    std::vector<sf::Text> instructions;
    sf::Text algorithm;
    sf::Text mazeSize;
    sf::Text framerate;
    sf::Text totalCheckstext;
    sf::Text pathLengthtext;
    sf::Text timeTakentext;

    int cycle = 0;
    int dimensionCycle = 0;
    int framerateCycle = 0;

    std::string algorithms[4] = {"BFS", "Dijkstra", "A*", "Greedy Best First Search"};
    int dimensions[11] = {7 ,9, 11, 13 ,15, 17, 19, 21, 23, 25, 49};
    int framerates[9] = {20, 30, 40, 50, 60, 80, 100, 200, 1000};

    int totalChecks;
    float pathLength;
    sf::Time timeTaken;
    sf::Clock timer;

    std::queue<Cell*> BFSq;
    bool BFSstarted = false;

    std::priority_queue<std::pair<float, Cell *>, std::vector<std::pair<float, Cell *>>, std::greater<std::pair<float, Cell *>>> DJKpq;
    bool DJKstarted = false;

    struct CompareGBFSdistance {
        bool operator()(Cell* lhs, Cell* rhs);
    };
    std::priority_queue<Cell*, std::vector<Cell*>, CompareGBFSdistance> GBFSpq;
    bool GBFSstarted = false;

    struct CompareASdistance {
        bool operator()(Cell* lhs, Cell* rhs);
    };
    std::priority_queue<Cell*, std::vector<Cell*>, CompareASdistance> ASpq;
    bool ASstarted = false;

public:
    Board();

    void draw(sf::RenderWindow &window);

    void downArrow();
    void upArrow();
    void leftClick(sf::Vector2i pos);
    void rightClick(sf::Vector2i pos);
    void shiftLeftClick(sf::Event &event);
    void shiftRightClick(sf::Event &event);
    void checkAlgorithm();
    void changeDimensionsK();
    void changeDimensionsL();
    void increaseFramerate();
    void decreaseFramerate();

    void reset();
    void resetPath();
    void resizeBoard();

    Cell* findStart();
    Cell* findFinish();

    bool isFinished();
    bool diagonallyWalled(Cell* first, Cell* second);
    bool isDiagonal(Cell* first, Cell* second);

    void readMazeFile();

    void BreadthFirstSearchloop();
    void GreedyBestFirstSearchLoop();
    void DijkstraSearchLoop();
    void AStarLoop();
    void createPath();
};




#endif //PATHFINDINGPROJECT_BOARD_H