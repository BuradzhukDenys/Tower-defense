#pragma once
#include "Interface.h"
#include <SFML/Graphics/Text.hpp>

class Button :
    public Interface
{
public:
    enum class ButtonType
    {
        Ballista,
        Bomber,
        Wizzard,
        Play,
        Upgrade,
        Sell,
        Resume,
        Exit,
        None
    };

    Button(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, const std::string& buttonText, const ButtonType& buttonType);
    
    const sf::Text& getButtonText();
    void setButtonText(const std::string& text);
    const sf::Vector2f getSize();
    void setSize(const sf::Vector2f& size);
    const sf::Vector2f getPosition();
    void setPosition(const sf::Vector2f& position);
    const sf::RectangleShape& getButtonShape();
    const ButtonType& getButtonType();

    virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window) override;
private:
    friend class InterfaceContainer;

    bool isClicked(const sf::Vector2f& mousePos) const;
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    ButtonType buttonType = ButtonType::None;
    const float BASE_BUTTON_FONT = 50.f;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

