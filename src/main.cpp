#include "SFML/Graphics.hpp"
#include "Board.h"

void ProcessInputs(sf::RenderWindow &window, Board &board) {

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { //drawing walls
        board.leftClick(sf::Mouse::getPosition(window));
    }

    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) and !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { //erasing walls
        board.rightClick(sf::Mouse::getPosition(window));
    }

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            exit(0);
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left and sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { //places start
                board.shiftLeftClick(event);
            }
            else if (event.mouseButton.button == sf::Mouse::Right and sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {  //places finish
                board.shiftRightClick(event);
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                board.reset();
            }
            if (event.key.code == sf::Keyboard::R) {
                board.resetPath();
            }
            if (event.key.code == sf::Keyboard::Down) {
                board.downArrow();
            }
            if (event.key.code == sf::Keyboard::Up) {
                board.upArrow();
            }
            if (event.key.code == sf::Keyboard::G){
                board.readMazeFile();
            }
            if (event.key.code == sf::Keyboard::K){
                board.changeDimensionsK();
            }
            if (event.key.code == sf::Keyboard::L){
                board.changeDimensionsL();
            }
            if (event.key.code == sf::Keyboard::P) {
                board.increaseFramerate();
            }
            if (event.key.code == sf::Keyboard::O) {
                board.decreaseFramerate();
            }
        }
    }
}
//
int main() {

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pathfinding");

    Board board;

    while (window.isOpen()) {

        ProcessInputs(window, board);

        if (board.findStart() != nullptr and board.findFinish() != nullptr and !board.isFinished()) {
            board.checkAlgorithm();
        }

        window.clear(sf::Color(211, 211, 211));  //light grey
        board.draw(window);
        window.display();
    }

    return 0;
}