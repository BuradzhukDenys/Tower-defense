#pragma once
#include "Button.h"
#include "Interface.h"

class Tower;

class InterfaceContainer :
    public Interface
{
public:
    InterfaceContainer(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& backgroundColor);
    void addButtons(const int buttonsCount, const std::vector<sf::Vector2f>& sizes, const sf::Vector2f& firstButtonPosition, const std::vector<sf::Color>& colors, const std::vector<std::string>& texts, const std::vector<Button::ButtonType>& buttonTypes);
    void addContainerText(const std::string& containerString, const sf::Vector2f& position, const sf::Color& colorText = sf::Color::White, const float fontSize = BASE_CONTAINER_FONT_SIZE);

    sf::RectangleShape getGUI() const;
    sf::Vector2f getSize() const;
    sf::Vector2f getPosition() const;

    void handleClick(const sf::Vector2f& mousePos);
    virtual void Update(sf::Time deltaTime, const sf::Vector2f& mousePosition) override;
    bool contains(const sf::Vector2f& point) const;
private:
    sf::RectangleShape GUI;
    sf::Color backgroundColor;
    static constexpr float BASE_CONTAINER_FONT_SIZE = 50.f;
    const float MARGIN_BETWEEN_COMPONENTS_Y = 25.f;
    std::vector<std::unique_ptr<Button>> buttons;
    std::vector<std::unique_ptr<sf::Text>> containerTexts;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

