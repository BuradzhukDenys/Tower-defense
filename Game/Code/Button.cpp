#include "Button.h"
#include "Interface.h"
#include "Resources.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Rendertarget.hpp>
#include "GameState.h"

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, const std::string& buttonTextString, const ButtonType& buttonType)
	: buttonShape(size), buttonText(Resources::fonts.Get(Resources::Font::BasicFont)), buttonType(buttonType), buttonColor(color)
{
	buttonShape.setFillColor(color);
	buttonShape.setOrigin({ buttonShape.getSize().x / 2.f, 0 });
	buttonShape.setPosition(position);
	buttonText.setString(buttonTextString);
	buttonText.setCharacterSize(BASE_BUTTON_FONT);
	buttonText.setOrigin({ buttonText.getGlobalBounds().size.x / 2.f, 0 });
	buttonText.setPosition(buttonShape.getPosition());
}

bool Button::isClicked(const sf::Vector2f& mousePos) const
{
	return this->buttonShape.getGlobalBounds().contains(mousePos);
}

const sf::Text& Button::getButtonText()
{
	return buttonText;
}

void Button::setButtonText(const std::string& text)
{
	buttonText.setString(text);
}

const sf::Vector2f Button::getSize()
{
	return buttonShape.getSize();
}

void Button::setSize(const sf::Vector2f& size)
{
	buttonShape.setSize(size);
}

const sf::Vector2f Button::getPosition()
{
	return buttonShape.getPosition();
}

void Button::setPosition(const sf::Vector2f& position)
{
	buttonShape.setPosition(position);
	buttonText.setPosition(position);
}

const sf::RectangleShape& Button::getButtonShape()
{
	return buttonShape;
}

const Button::ButtonType& Button::getButtonType()
{
	return buttonType;
}

void Button::setScale(const sf::Vector2f& scale)
{
	buttonShape.setScale(scale);
}

void Button::Update(sf::Time deltaTime, const sf::Vector2f& mousePosition)
{
	if (isClicked(mousePosition))
	{
		buttonShape.setOutlineThickness(-5.f);
		buttonShape.setOutlineColor(sf::Color::Red);
	}
	else
	{
		buttonShape.setOutlineThickness(0.f);
		buttonShape.setOutlineColor(sf::Color::Transparent);
	}

	if (Interface::getSelectedTower() == Interface::TowerType::Ballista && buttonType == Button::ButtonType::Ballista)
	{
		buttonShape.setOutlineThickness(-5.f);
		buttonShape.setOutlineColor(sf::Color::Red);
	}
	else if (Interface::getSelectedTower() == Interface::TowerType::Bomber && buttonType == Button::ButtonType::Bomber)
	{
		buttonShape.setOutlineThickness(-5.f);
		buttonShape.setOutlineColor(sf::Color::Red);
	}
	else if (Interface::getSelectedTower() == Interface::TowerType::Wizzard && buttonType == Button::ButtonType::Wizzard)
	{
		buttonShape.setOutlineThickness(-5.f);
		buttonShape.setOutlineColor(sf::Color::Red);
	}
	
	if (GameState::getState() == GameState::State::WavePlay && buttonType == Button::ButtonType::Play)
	{
		buttonShape.setFillColor(sf::Color(100, 100, 100));
		buttonShape.setOutlineThickness(-5.f);
		buttonShape.setOutlineColor(sf::Color::Red);
	}
	else
	{
		buttonShape.setFillColor(buttonColor);
	}
	
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(buttonShape, states);
	target.draw(buttonText, states);
}