#pragma once
#include "Button.h"
#include "Interface.h"
#include <SFML/System/Vector2.hpp>

class Tower;

namespace sf
{
    class Time;
}

class InterfaceContainer :
    public Interface
{
public:
    InterfaceContainer(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& backgroundColor);
    void addButtons(const int buttonsCount, const std::vector<sf::Vector2f>& sizes, const std::vector<sf::Vector2f>& positions, const std::vector<sf::Color>& colors, const std::vector<std::string>& texts, const std::vector<Button::ButtonType>& buttonTypes);
    void addContainerText(const std::string& containerString, const sf::Vector2f& position, const float fontSize = BASE_CONTAINER_FONT_SIZE);

    const sf::RectangleShape& getGUI() const;
    const sf::Vector2f& getSize() const;
    const sf::Vector2f& getPosition() const;
    const sf::Vector2f& getContainerTextSize(const std::string& text) const;

    void handleClick(const sf::Vector2f& mousePos);
    virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window) override;
    bool contains(const sf::Vector2f& point) const;
private:
    sf::RectangleShape GUI;
    sf::Color backgroundColor;
    static constexpr float BASE_CONTAINER_FONT_SIZE = 50.f;
    const float MARGIN_BETWEEN_COMPONENTS = 25.f;
    std::vector<std::unique_ptr<Button>> buttons;
    std::vector<std::unique_ptr<sf::Text>> containerTexts;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

