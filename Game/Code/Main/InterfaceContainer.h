#pragma once
#include "Button.h"
#include <SFML/System/Vector2.hpp>
#include <list>

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
    //void addButton(const InterfaceContainer& container, const sf::Color& color, const std::string& buttonText);
    //void addButtons(int count, const sf::Color& color, const std::vector<std::string>& texts, const std::vector<Button::ButtonType>& buttonType);
    void addButton(const sf::Color& color, const std::string& text, const Button::ButtonType& buttonType);
    void addContainerText(const std::string& containerString, const sf::Vector2f& position, const float fontSize = BASE_CONTAINER_FONT_SIZE);

    const sf::RectangleShape& getGUI() const;
    const sf::Vector2f& getSize() const;
    const sf::Vector2f& getPosition() const;

    void handleClick(const sf::Vector2f& mousePos);
    virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window) override;
    bool contains(const sf::Vector2f& point) const;
private:
    sf::RectangleShape GUI;
    sf::Color backgroundColor;
    std::vector<std::unique_ptr<sf::Text>> containerTexts;
    static constexpr float BASE_CONTAINER_FONT_SIZE = 50.f;
    const float MARGIN_BETWEEN_BUTTONS = 25.f;
    std::vector<std::unique_ptr<Button>> buttons;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

