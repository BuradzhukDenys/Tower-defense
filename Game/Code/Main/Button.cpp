#include "Button.h"
#include "Resources.h"

Button::Button(sf::Vector2f size, sf::Vector2f position, sf::Color color)
	: buttonShape(size), buttonText(Resources::fonts.Get(Resources::Font::BasicFont))
{
	buttonShape.setPosition(position);
	buttonShape.setFillColor(color);
	buttonText.setString("Ballista");
	buttonText.setCharacterSize(50);
	buttonText.setOrigin({ buttonText.getLocalBounds().size.x / 2, buttonText.getLocalBounds().size.y / 2 });
	buttonText.setPosition(
		{ this->buttonShape.getPosition().x + this->buttonShape.getSize().x / 2,
		this->buttonShape.getPosition().y + this->buttonShape.getSize().y / 2 }
	);
}

void Button::setOrigin(sf::Vector2f point)
{
	this->buttonShape.setOrigin(point);
}

bool Button::isClicked(const sf::Vector2f& mousePos) const
{
	return this->buttonShape.getGlobalBounds().contains(mousePos);
}

const sf::Text& Button::getButtonText()
{
	return buttonText;
}

sf::RectangleShape& Button::getButtonShape()
{
	return buttonShape;
}

void Button::Update(sf::Time deltaTime, const sf::RenderWindow& window)
{
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(buttonShape, states);
	target.draw(buttonText, states);
}