#include <SFML/Graphics.hpp>

int main()
{
    int length = 200;
    int width = 200;
    sf::RenderWindow window(sf::VideoMode(length, width), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    float centerL = length / 2;
    float centerW = width / 2;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
