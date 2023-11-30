#include <SFML/Graphics.hpp>
#include "Board.h"

int main() {

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pathfinding");

    std::string ini = "Mouse Instruction\nLeft: Place Wall\nRight: Erase Wall\nShift+Left: Place Start\nShift+Right: Place Finish\n\nKeyboard Instruction\nEnter: Reset Board";
    sf::Font font;
    font.loadFromFile("font.ttf");
    sf::Text instructions(ini, font);
    instructions.setScale(.8, .8);
    instructions.setFillColor(sf::Color::Blue);
    instructions.setPosition(BOARD_LEN, 0);

    int boardLength = 15;  //change side length count here (its always a square)
    Board board(boardLength);



    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left and sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                    auto pos = sf::Mouse::getPosition(window);
                    if (pos.x < BOARD_LEN and pos.x > 0 and pos.y < BOARD_LEN and pos.y > 0) {
                        int x = pos.x / (BOARD_LEN / boardLength);
                        int y = pos.y / (BOARD_LEN / boardLength);
                        board.shiftLeftClick(x, y);
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right and sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                    auto pos = sf::Mouse::getPosition(window);
                    if (pos.x < BOARD_LEN and pos.x > 0 and pos.y < BOARD_LEN and pos.y > 0) {
                        int x = pos.x / (BOARD_LEN / boardLength);
                        int y = pos.y / (BOARD_LEN / boardLength);
                        board.shiftRightClick(x, y);
                    }
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    board.reset();
                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            auto pos = sf::Mouse::getPosition(window);
            if (pos.x < BOARD_LEN and pos.x > 0 and pos.y < BOARD_LEN and pos.y > 0) {
                int x = pos.x / (BOARD_LEN / boardLength);
                int y = pos.y / (BOARD_LEN / boardLength);
                board.leftClick(x, y);
            }

        }

        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) and !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            auto pos = sf::Mouse::getPosition(window);
            if (pos.x < BOARD_LEN and pos.x > 0 and pos.y < BOARD_LEN and pos.y > 0) {
                int x = pos.x / (BOARD_LEN / boardLength);
                int y = pos.y / (BOARD_LEN / boardLength);
                board.rightClick(x, y);
            }
        }

        window.clear(sf::Color(211, 211, 211));  //light grey
        board.draw(window);
        window.draw(instructions);
        window.display();
    }

    return 0;
}