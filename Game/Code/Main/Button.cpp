#include "Button.h"
#include "Resources.h"

Button::Button(sf::Vector2f size, sf::Vector2f position, sf::Color color)
	: Interface(size, position),
	buttonText(Resources::fonts.Get(Resources::Font::BasicFont))
{
	this->GUI.setFillColor(color);
	buttonText.setString("Ballista");
	buttonText.setCharacterSize(50);
	buttonText.setOrigin({ buttonText.getLocalBounds().size.x / 2, buttonText.getLocalBounds().size.y / 2 });
	buttonText.setPosition(
		{ this->GUI.getPosition().x + this->GUI.getSize().x / 2,
		this->GUI.getPosition().y + this->GUI.getSize().y / 2 }
	);
}

void Button::setOrigin(sf::Vector2f point)
{
	this->GUI.setOrigin(point);
}

bool Button::isClicked(const sf::Vector2f& mousePos) const
{
	return this->GUI.getGlobalBounds().contains(mousePos);
}

sf::Text& Button::getButtonText()
{
	return buttonText;
}

void Button::Update(sf::Time deltaTime, const sf::RenderWindow& window)
{
}
