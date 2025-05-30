#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "Slider.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML Slider Example");

    Slider slider(100, 300, 200, 0, 100); // (x, y, ширина, min, max)

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        slider.update(window); // Оновлюємо стан повзунка

        window.clear(sf::Color(30, 30, 30));
        slider.draw(window);   // Малюємо повзунок
        window.display();
    }

    return 0;
}