#pragma once
#include "Interface.h"
#include <SFML/Graphics/Text.hpp>

class Button :
    public Interface
{
public:
    Button(sf::Vector2f size, sf::Vector2f position, sf::Color color);
    void setOrigin(sf::Vector2f point);
    const sf::Text& getButtonText();
    sf::RectangleShape& getButtonShape();

    virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window) override;
private:
    friend class InterfaceContainer;

    bool isClicked(const sf::Vector2f& mousePos) const;
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

