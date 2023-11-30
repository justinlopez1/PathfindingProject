//
// Created by funny on 11/28/2023.
//

#include "Board.h"

Board::Cell::Cell(int x, int y, float sideLength) {
    this->x = x;
    this->y = y;
    this->sideLength = sideLength;
    isFinish = false;
    isStart = false;
    isWall = false;
    //std::cout << x << " " << y << " " << sideLength << std::endl;
    tile.setPosition(x*sideLength, y*sideLength);
    tile.setSize(sf::Vector2f(sideLength, sideLength));
    tile.setFillColor(sf::Color::White);
}

void Board::Cell::draw(sf::RenderWindow &window) {
    if (isFinish)
        tile.setFillColor(sf::Color::Red);
    else if (isStart)
        tile.setFillColor(sf::Color::Green);
    else if (isWall)
        tile.setFillColor(sf::Color::Black);
    else
        tile.setFillColor(sf::Color(255,255,255));

    window.draw(tile);
}




Board::Board(int boardLength) {
    this->boardLength = boardLength;
    float cellLength = BOARD_LEN / (float)boardLength;
    for (int i = 0; i < boardLength; i++) {
        std::vector<Cell> temp;
        cells.push_back(temp);
        for (int j = 0; j < boardLength; j++) {
            cells[i].emplace_back(j, i, cellLength);
        }
    }
    for (auto &row : cells) {
        for (auto &cell : row) {
            //std::cout << cell.x << " " << cell.y << std::endl;
        }
    }
    for (int i = 1; i < boardLength; i++) {
        sf::RectangleShape temp;
        temp.setFillColor(sf::Color(115, 147, 179));
        temp.setSize(sf::Vector2f(2, BOARD_LEN));
        temp.setPosition(i*cellLength, 0);
        borders.push_back(temp);
        sf::RectangleShape temp2;
        temp2.setFillColor(sf::Color(115, 147, 179));
        temp2.setSize(sf::Vector2f(BOARD_LEN, 2));
        temp2.setPosition(0, i*cellLength);
        borders.push_back(temp2);
    }
}

void Board::draw(sf::RenderWindow &window) {
    for (auto& row : cells) {
        for (auto& cell : row) {
            cell.draw(window);
        }
    }
    for (auto& line : borders) {
        window.draw(line);
    }
}

void Board::leftClick(int x, int y) {
    //std::cout << x << " " << y << std::endl;
    cells[y][x].isWall = true;
}

void Board::rightClick(int x, int y) {
    cells[y][x].isWall = false;
}

void Board::shiftLeftClick(int x, int y) {
    for (auto& row : cells) {
        for (auto&cell : row) {
            cell.isStart = false;
        }
    }
    cells[y][x].isStart = true;
    cells[y][x].isWall = false;
}

void Board::shiftRightClick(int x, int y) {
    for (auto& row : cells) {
        for (auto&cell : row) {
            cell.isFinish = false;
        }
    }
    cells[y][x].isFinish = true;
    cells[y][x].isWall = false;
}

void Board::reset() {
    for (auto& row : cells) {
        for (auto& cell : row) {
            cell.isWall = false;
            cell.isFinish = false;
            cell.isStart = false;
        }
    }
}


