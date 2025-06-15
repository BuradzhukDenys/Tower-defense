#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace sf
{
    class Time;
    class RenderWindow;
    class RenderTarget;
};

class Button : public sf::Drawable
{
public:
    enum class ButtonType
    {
        Ballista,
        Bomber,
        Wizzard,
        Play,
        Resume,
        Restart,
        Exit,
        None
    };

    Button(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, const std::string& buttonText, const ButtonType& buttonType);
    
    const sf::Vector2f getSize();
    const sf::Vector2f getPosition();
    void setPosition(const sf::Vector2f& position);
    const ButtonType& getButtonType();

    void Update(sf::Time deltaTime, const  sf::Vector2f& mousePosition);
private:
    friend class InterfaceContainer;
    
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    sf::Color buttonColor;
    ButtonType buttonType = ButtonType::None;
    const float BASE_BUTTON_FONT = 50.f;
    bool isClicked(const sf::Vector2f& mousePos) const;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

