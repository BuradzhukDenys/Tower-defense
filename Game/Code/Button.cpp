#include "Button.h"
#include "Interface.h"
#include "Resources.h"
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

const sf::Vector2f Button::getSize()
{
	return buttonShape.getSize();
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

const Button::ButtonType& Button::getButtonType()
{
	return buttonType;
}

void Button::Update(sf::Time deltaTime, const sf::Vector2f& mousePosition)
{
	//якщо курсор на кнопц≥, робимо навколо кнопки рамку
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

	//якщо вибрана вежа, ≥ тим кнопки - ц€ ж вежа, залишаЇмо рамку
	if (Interface::getSelectedTower() == Interface::SelectedTowerType::Ballista && buttonType == Button::ButtonType::Ballista)
	{
		buttonShape.setOutlineThickness(-5.f);
		buttonShape.setOutlineColor(sf::Color::Red);
	}
	else if (Interface::getSelectedTower() == Interface::SelectedTowerType::Bomber && buttonType == Button::ButtonType::Bomber)
	{
		buttonShape.setOutlineThickness(-5.f);
		buttonShape.setOutlineColor(sf::Color::Red);
	}
	else if (Interface::getSelectedTower() == Interface::SelectedTowerType::Wizzard && buttonType == Button::ButtonType::Wizzard)
	{
		buttonShape.setOutlineThickness(-5.f);
		buttonShape.setOutlineColor(sf::Color::Red);
	}
	
	//якщо стан гри це WavePlay, то робимо кол≥р кнопки грати, с≥рим
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