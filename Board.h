//
// Created by funny on 11/28/2023.
//

#ifndef PATHFINDINGPROJECT_BOARD_H
#define PATHFINDINGPROJECT_BOARD_H

#pragma once
#include <vector>
#include <iostream>
#include <queue>
#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 900
#define BOARD_LEN 900

class Board {
    struct Cell {
        sf::RectangleShape tile;
        bool isStart;
        bool isFinish;
        bool isWall;
        bool visited;
        bool isPath;
        int x;
        int y;
        float sideLength;
        std::vector<Cell*> nearbyCells;
        Cell* prev = nullptr;

        Cell(int x, int y, float sideLength);
        void draw(sf::RenderWindow &window);

    };

    std::vector<std::vector<Cell>> cells;
    std::vector<sf::RectangleShape> borders;
    int boardLength;
    bool finished;
    Cell* start;
    Cell* finish;

    std::queue<Cell*> BFSq;
    bool BFSstarted = false;

public:
    Board(int boardLength);
    void draw(sf::RenderWindow &window);
    void leftClick(int x, int y);
    void rightClick(int x, int y);
    void shiftLeftClick(int x, int y);
    void shiftRightClick(int x, int y);
    void reset();
    Cell* findStart();
    Cell* findFinish();
    bool isFinished();
    void createPath();

    void BFSloop();
    void startBFS();
};


#endif //PATHFINDINGPROJECT_BOARD_H
