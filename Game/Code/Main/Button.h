#pragma once
#include "Interface.h"
#include <SFML/Graphics/Text.hpp>

namespace sf
{
    class RenderWindow;
};

class Button :
    public Interface
{
public:
    Button(sf::Vector2f size, sf::Vector2f position, sf::Color color);
    void setOrigin(sf::Vector2f point);
    sf::Text& getButtonText();

    virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window) override;
private:
    friend class InterfaceContainer;

    bool isClicked(const sf::Vector2f& mousePos) const;
    sf::Text buttonText;
};

