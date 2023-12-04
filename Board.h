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
#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 900
#define BOARD_LEN 900

class Board {
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
        Cell* prev = nullptr;

        int GBFSdistance = 0;
        void setGBFSDistance(Cell* finish);

        float disktraDistance;

        bool aStarVisited = false;

        Cell(int x, int y, float sideLength);
        void draw(sf::RenderWindow &window);
        void draw(sf::RenderWindow &window, sf::Font* font);
    };
    struct CompareGBFSdistance {
        bool operator()(Cell* lhs, Cell* rhs);
    };
    struct CompareASdistance {
        bool operator()(Cell* lhs, Cell* rhs);
    };

    std::vector<std::vector<Cell>> cells;
    std::vector<sf::RectangleShape> borders;

    int boardLength;
    float cellSideLength;
    int cycle = 0;
    bool finished;
    Cell* start;
    Cell* finish;
    sf::Font font;
    sf::Text instructions;
    sf::Text algorithm;
    std::string algorithms[4] = {"BFS", "Dijkstra", "A*", "Greedy Best First Search"};


    std::queue<Cell*> BFSq;
    bool BFSstarted = false;

    std::priority_queue<std::pair<int, Cell *>, std::vector<std::pair<int, Cell *>>, std::greater<std::pair<int, Cell *>>> DJKpq;
    bool DJKstarted = false;

    std::priority_queue<Cell*, std::vector<Cell*>, CompareGBFSdistance> GBFSpq;
    bool GBFSstarted = false;

    std::priority_queue<Cell*, std::vector<Cell*>, CompareASdistance> ASpq;
    bool ASstarted = false;

public:
    Board(int boardLength);
    void downArrow();
    void upArrow();
    void draw(sf::RenderWindow &window);
    void leftClick(sf::Vector2i pos);
    void rightClick(sf::Vector2i pos);
    void shiftLeftClick(sf::Event &event);
    void shiftRightClick(sf::Event &event);
    void checkAlgorithm();

    void reset();
    void resetPath();

    Cell* findStart();
    Cell* findFinish();
    bool isFinished();
    void createPath();
    bool diagonallyWalled(Cell* first, Cell* second);

    void BreadthFirstSearchloop();

    void GreedyBestFirstSearchLoop();

    void DijkstraSearchLoop();

    void AStarLoop();
};




#endif //PATHFINDINGPROJECT_BOARD_H
