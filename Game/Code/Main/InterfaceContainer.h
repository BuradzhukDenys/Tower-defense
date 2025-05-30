#pragma once
#include "Button.h"
#include <SFML/System/Vector2.hpp>
#include <list>

class Tower;

namespace sf
{
    class Time;
    class RenderWindow;
}

class InterfaceContainer :
    public Interface
{
public:
    InterfaceContainer(sf::Vector2f size, sf::Vector2f position, sf::Color backgroundColor);

    void handleClick(const sf::Vector2f& mousePos);
    virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window) override;
    bool contains(const sf::Vector2f& point) const;
private:
    sf::Color backgroundColor;
    std::list<std::unique_ptr<Button>> buttons;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

