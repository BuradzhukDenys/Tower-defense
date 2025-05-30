#pragma once
#include <SFML/Graphics.hpp>

class Slider {
private:
    sf::RectangleShape track;    // Фон повзунка
    sf::RectangleShape thumb;   // Рухомий елемент
    sf::Font font;
    sf::Text valueText;         // Відображення значення

    float minValue;             // Мінімальне значення (наприклад, 0)
    float maxValue;             // Максимальне значення (наприклад, 100)
    float currentValue;         // Поточне значення

    bool isDragging = false;    // Чи перетягується повзунок?

public:
    Slider(float x, float y, float width, float min, float max)
        : minValue(min), maxValue(max), currentValue(min), valueText(font)
    {
        // Налаштування фону (трек)
        track.setSize(sf::Vector2f(width, 10));
        track.setPosition({ x, y });
        track.setFillColor(sf::Color(100, 100, 100));

        // Налаштування повзунка (thumb)
        thumb.setSize(sf::Vector2f(20, 30));
        thumb.setPosition({ x, y - 10 });
        thumb.setFillColor(sf::Color(200, 50, 50));

        // Налаштування тексту
        if (font.openFromFile("Assets/Fonts/arial.ttf")) {
            valueText.setCharacterSize(20);
            valueText.setFillColor(sf::Color::White);
            valueText.setPosition({ x + width + 10, y - 5 });
        }

        updateValueText();
    }

    void updateValueText() {
        valueText.setString(std::to_string(static_cast<int>(currentValue)));
    }

    // Оновлення позиції повзунка
    void update(const sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (thumb.getGlobalBounds().contains(sf::Vector2f( mousePos.x, mousePos.y ))) {
                isDragging = true;
            }
        }
        else {
            isDragging = false;
        }

        if (isDragging) {
            // Обмеження руху повзунка в межах треку
            float newX = std::max(track.getPosition().x,
                std::min(float(mousePos.x), track.getPosition().x + track.getSize().x));
            thumb.setPosition({ newX - thumb.getSize().x / 2, thumb.getPosition().y });

            // Оновлення значення (0-100 або інший діапазон)
            float ratio = (newX - track.getPosition().x) / track.getSize().x;
            currentValue = minValue + (maxValue - minValue) * ratio;
            updateValueText();
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(track);
        window.draw(thumb);
        window.draw(valueText);
    }

    float getValue() const { return currentValue; }
};
