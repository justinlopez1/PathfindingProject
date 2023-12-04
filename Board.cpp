//
// Created by funny on 11/28/2023.
//

#include "Board.h"

Board::Cell::Cell(int x, int y, float sideLength) {
    this->x = x;
    this->y = y;
    this->disktraDistance = std::numeric_limits<int>::max();
    isFinish = false;
    isStart = false;
    isWall = false;
    visited = false;
    isPath = false;
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
    else if (isPath)
        tile.setFillColor(sf::Color::Yellow);
    else if (isWall)
        tile.setFillColor(sf::Color::Black);
    else if (visited)
        tile.setFillColor(sf::Color::Blue);

    else
        tile.setFillColor(sf::Color(255,255,255));

    window.draw(tile);
}

void Board::Cell::draw(sf::RenderWindow &window, sf::Font* font) {
    if (isFinish)
        tile.setFillColor(sf::Color::Red);
    else if (isStart)
        tile.setFillColor(sf::Color::Green);
    else if (isPath)
        tile.setFillColor(sf::Color::Yellow);
    else if (isWall)
        tile.setFillColor(sf::Color::Black);
    else if (visited)
        tile.setFillColor(sf::Color::Blue);

    else
        tile.setFillColor(sf::Color(255,255,255));

    window.draw(tile);

    if (GBFSdistance > 0) {
        t.setString(std::to_string(GBFSdistance + disktraDistance));
        t.setPosition(tile.getPosition());
        t.setFont(*font);
        t.setFillColor(sf::Color::Green);
        t.setScale(.75, .75);
        window.draw(t);
    }




}

void Board::Cell::setGBFSDistance(Board::Cell *finish) {
    int hDist = abs(finish->x - x);
    int vDist = abs(finish->y - y);
    GBFSdistance = std::max(hDist, vDist) + (std::min(hDist, vDist)*sqrt(2));
}



Board::Board(int boardLength) {
    this->boardLength = boardLength;
    cellSideLength = BOARD_LEN / boardLength;
    finished = false;
    for (int i = 0; i < boardLength; i++) {
        std::vector<Cell> temp;
        cells.push_back(temp);
        for (int j = 0; j < boardLength; j++) {
            cells[i].emplace_back(j, i, cellSideLength);
        }
    }
    for (int i = 1; i < boardLength; i++) {
        sf::RectangleShape temp;
        temp.setFillColor(sf::Color(115, 147, 179));
        temp.setSize(sf::Vector2f(2, BOARD_LEN));
        temp.setPosition(i*cellSideLength, 0);
        borders.push_back(temp);
        sf::RectangleShape temp2;
        temp2.setFillColor(sf::Color(115, 147, 179));
        temp2.setSize(sf::Vector2f(BOARD_LEN, 2));
        temp2.setPosition(0, i*cellSideLength);
        borders.push_back(temp2);
    }
    for (int i = 0; i < boardLength; i++) {
        for (int j = 0; j < boardLength; j++) {
            if (cells[i][j].y != 0)
                cells[i][j].nearbyCells.push_back(&cells[i-1][j]);
            if (cells[i][j].x != 0)
                cells[i][j].nearbyCells.push_back(&cells[i][j-1]);
            if (cells[i][j].y != boardLength-1)
                cells[i][j].nearbyCells.push_back(&cells[i+1][j]);
            if (cells[i][j].x != boardLength-1)
                cells[i][j].nearbyCells.push_back(&cells[i][j+1]);
            if (cells[i][j].y != 0 and cells[i][j].x != 0)
                cells[i][j].nearbyCells.push_back(&cells[i-1][j-1]);
            if (cells[i][j].y != 0 and cells[i][j].x != boardLength-1)
                cells[i][j].nearbyCells.push_back(&cells[i-1][j+1]);
            if (cells[i][j].y != boardLength-1 and cells[i][j].x != 0)
                cells[i][j].nearbyCells.push_back(&cells[i+1][j-1]);
            if (cells[i][j].x != boardLength-1 and cells[i][j].y != boardLength-1)
                cells[i][j].nearbyCells.push_back(&cells[i+1][j+1]);
        }
    }

    std::string ini = "Mouse Instruction\n"
                      "Left: Place Wall\n"
                      "Right: Erase Wall\n"
                      "Shift+Left: Place Start\n"
                      "Shift+Right: Place Finish\n\n"
                      "Keyboard Instruction\n"
                      "Enter: Reset Board\n"
                      "r: Rest Path\n"
                      "Down/Up Arrow: Change Algo\n";

    font.loadFromFile("font.ttf");
    instructions.setFont(font);
    instructions.setString(ini);
    instructions.setScale(.8, .8);
    instructions.setFillColor(sf::Color::Blue);
    instructions.setPosition(BOARD_LEN, 0);

    algorithm.setFont(font);
    algorithm.setString("Current Algorithm:\n  " + algorithms[cycle]);
    algorithm.setScale(.8, .8);
    algorithm.setFillColor(sf::Color::Blue);
    algorithm.setPosition(BOARD_LEN, 270);

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
    window.draw(instructions);
    window.draw(algorithm);
}
void Board::checkAlgorithm(){
    if(algorithms[cycle] == "BFS")
        BreadthFirstSearchloop();
    if(algorithms[cycle] == "Dijkstra")
        DijkstraSearchLoop();
    if(algorithms[cycle] == "A*"){
        AStarLoop();
    }
    if(algorithms[cycle] == "Greedy Best First Search")
        GreedyBestFirstSearchLoop();
}

void Board::downArrow() {
    cycle++;
    if (cycle == 4)
        cycle = 0;
    algorithm.setString("Current Algorithm: \n  " + algorithms[cycle]);
}
void Board::upArrow() {
     cycle--;
    if (cycle == -1)
        cycle = 3;
    algorithm.setString("Current Algorithm: \n  " + algorithms[cycle]);
}

void Board::leftClick(sf::Vector2i pos) {
    if (pos.x < BOARD_LEN and pos.x > 0 and pos.y < BOARD_LEN and pos.y > 0) {
        int x = pos.x / cellSideLength;
        int y = pos.y / cellSideLength;
        cells[y][x].isWall = true;
    }
}

void Board::rightClick(sf::Vector2i pos) {
    if (pos.x < BOARD_LEN and pos.x > 0 and pos.y < BOARD_LEN and pos.y > 0) {
        int x = pos.x / cellSideLength;
        int y = pos.y / cellSideLength;
        cells[y][x].isWall = false;
    }
}

void Board::shiftLeftClick(sf::Event &event) {
    sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
    if (pos.x < BOARD_LEN and pos.x > 0 and pos.y < BOARD_LEN and pos.y > 0) {
        int x = pos.x / cellSideLength;
        int y = pos.y / cellSideLength;
        for (auto& row : cells) {
            for (auto&cell : row) {
                cell.isStart = false;
            }
        }
        cells[y][x].isStart = true;
        cells[y][x].isWall = false;
        start = &cells[y][x];
    }
}

void Board::shiftRightClick(sf::Event &event) {
    sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
    if (pos.x < BOARD_LEN and pos.x > 0 and pos.y < BOARD_LEN and pos.y > 0) {
        int x = pos.x / cellSideLength;
        int y = pos.y / cellSideLength;
        for (auto& row : cells) {
            for (auto&cell : row) {
                cell.isFinish = false;
            }
        }
        cells[y][x].isFinish = true;
        cells[y][x].isWall = false;
        finish = &cells[y][x];
    }
}

void Board::reset() {
    finished = false;
    BFSstarted = false;
    GBFSstarted = false;
    DJKstarted = false;
    ASstarted = false;
    for (auto& row : cells) {
        for (auto& cell : row) {
            cell.isWall = false;
            cell.isFinish = false;
            cell.isStart = false;
            cell.visited = false;
            cell.isPath = false;
            cell.GBFSdistance = 0;
            cell.disktraDistance = std::numeric_limits<int>::max();
            cell.aStarVisited = false;

        }
    }
    while (!BFSq.empty())
        BFSq.pop();
    while (!GBFSpq.empty())
        GBFSpq.pop();
    while (!DJKpq.empty())
        DJKpq.pop();
    while (!ASpq.empty())
        ASpq.pop();
    start = nullptr;
    finish = nullptr;
}

void Board::BreadthFirstSearchloop() {
    if (!BFSstarted) {
        BFSstarted = true;
        BFSq.push(start);
        start->visited = true;
    }

    if (!BFSq.empty() and !finished) {
        Cell* curr = BFSq.front();
        BFSq.pop();
        for (auto& cell : curr->nearbyCells) {
            if (!cell->visited and !cell->isWall and !diagonallyWalled(curr, cell)) {
                cell->prev = curr;
                cell->visited = true;

                if (cell->isFinish) {
                    finished = true;
                    createPath();
                    break;
                }

                BFSq.push(cell);
            }
        }
    }
}

Board::Cell *Board::findStart() {
    for (auto& row : cells) {
        for (auto& cell : row) {
            if (cell.isStart)
                return &cell;
        }
    }
    return nullptr;
}

Board::Cell *Board::findFinish() {
    for (auto& row : cells) {
        for (auto& cell : row) {
            if (cell.isFinish)
                return &cell;
        }
    }
    return nullptr;
}

bool Board::isFinished() {
    return finished;
}

void Board::createPath() {
    Cell* temp = finish->prev;
    while (!temp->isStart) {
        temp->isPath = true;
        temp = temp->prev;
    }
}

void Board::DijkstraSearchLoop()
{
    if (!DJKstarted)
    {
        DJKstarted = true;
        DJKpq.push(std::make_pair(0, start));
        start->disktraDistance = 0;
        start->visited = true;
    }

    if (!DJKpq.empty() and !finished)
    {
        Cell *curr = DJKpq.top().second;
        curr->visited = true;
        DJKpq.pop();
        for (auto &cell : curr->nearbyCells)
        {
            if (!cell->visited and !cell->isWall and !diagonallyWalled(curr, cell))
            {
                float weight = 1;
                if(abs(cell->x - curr->x) == abs(cell->y - curr->y)){
                    weight = sqrt(2);
                }
                if (cell->disktraDistance > curr->disktraDistance + weight)
                {
                    cell->prev = curr;
                    cell->disktraDistance = curr->disktraDistance + weight;
                    DJKpq.emplace(curr->disktraDistance + weight, cell);
                }
                if (cell->isFinish)
                {
                    finished = true;
                    createPath();
                    break;
                }
            }
        }
    }
}

void Board::GreedyBestFirstSearchLoop() {
    if (!GBFSstarted) {
        GBFSstarted = true;
        start->setGBFSDistance(finish);
        GBFSpq.push(start);
        start->visited = true;
    }

    if (!GBFSpq.empty() and !finished) {
        Cell* curr = GBFSpq.top();
        GBFSpq.pop();

        for (auto& cell : curr->nearbyCells) {
            if (!cell->visited and !cell->isWall and !diagonallyWalled(curr, cell)) {
                cell->setGBFSDistance(finish);
                cell->prev = curr;
                cell->visited = true;

                if (cell->isFinish) {
                    finished = true;
                    createPath();
                    break;
                }

                GBFSpq.push(cell);
            }
        }
    }
}

bool Board::diagonallyWalled(Board::Cell *first, Board::Cell *second) {
    if (!cells[first->y][second->x].isWall or !cells[second->y][first->x].isWall)
        return false;
    return true;
}

void Board::resetPath() {
    finished = false;
    BFSstarted = false;
    DJKstarted= false;
    GBFSstarted = false;
    ASstarted = false;
    for (auto& row : cells) {
        for (auto& cell : row) {
            cell.isPath = false;
            cell.visited = false;
            cell.GBFSdistance = 0;
            cell.disktraDistance = std::numeric_limits<int>::max();
            cell.aStarVisited = false;
        }
    }
    while (!BFSq.empty())
        BFSq.pop();
    while (!GBFSpq.empty())
        GBFSpq.pop();
    while (!DJKpq.empty())
        DJKpq.pop();
    while(!ASpq.empty())
        ASpq.pop();
}

void Board::AStarLoop() {
    if (!ASstarted)
    {
        ASstarted = true;
        ASpq.push(start);
        start->disktraDistance = 0;
        start->setGBFSDistance(finish);
        start->visited = true;
        start->aStarVisited = true;
    }
    if (!ASpq.empty() and !finished)
    {
        Cell *curr = ASpq.top();
        ASpq.pop();
        curr->aStarVisited = true;
        for (auto &cell : curr->nearbyCells)
        {
            if (!cell->aStarVisited and !cell->isWall and !diagonallyWalled(curr, cell))
            {
                cell->visited = true;
                cell->setGBFSDistance(finish);
                float weight = 1;
                if(abs(cell->x - curr->x) == abs(cell->y - curr->y)){
                    weight = sqrt(2);
                }
                if (cell->disktraDistance > curr->disktraDistance + weight)
                {
                    cell->prev = curr;
                    cell->disktraDistance = curr->disktraDistance + weight;
                    ASpq.emplace(cell);
                }

                if (cell->isFinish)
                {
                    finished = true;
                    createPath();
                    break;
                }
            }
        }
    }
}


bool Board::CompareGBFSdistance::operator()(Board::Cell *lhs, Board::Cell *rhs) {
    //std::cout << lhs->GBFSdistance << "<" << rhs->GBFSdistance << std::endl;
    return lhs->GBFSdistance > rhs->GBFSdistance;
}

bool Board::CompareASdistance::operator()(Board::Cell *lhs, Board::Cell *rhs) {
    return lhs->GBFSdistance+lhs->disktraDistance > rhs->GBFSdistance + rhs->disktraDistance;
}
