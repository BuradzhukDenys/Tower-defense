#include "InterfaceContainer.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include "Ballista.h"
#include <iostream>

InterfaceContainer::InterfaceContainer(sf::Vector2f size, sf::Vector2f position, sf::Color backgroundColor)
	: Interface(size, position), backgroundColor(backgroundColor)
{
	this->GUI.setFillColor(backgroundColor);
	buttons.emplace_back(std::make_unique<Button>(
		sf::Vector2f({ this->GUI.getSize().x * 0.7f, 150.f }),
		sf::Vector2f(this->GUI.getPosition().x + 50.f, this->GUI.getPosition().y + 50.f),
		sf::Color::Blue
	));
}

void InterfaceContainer::handleClick(const sf::Vector2f& mousePos)
{
	for (const auto& button : buttons) {  
		if (button->isClicked(mousePos)) {
			Interface::setSelectedTower(Interface::TowerType::Ballista);
		}
	}
}

void InterfaceContainer::Update(sf::Time deltaTime, const sf::RenderWindow& window)
{
	sf::Vector2f mousePosition(sf::Mouse::getPosition(window));
	for (const auto& button : buttons) {
		if (Interface::getSelectedTower() == Interface::TowerType::Ballista)
		{
			button->GUI.setOutlineThickness(5.f);
			button->GUI.setOutlineColor(sf::Color::Red);
		}
		else
		{
			button->GUI.setOutlineThickness(0.f);
		}
	}

	updateCurrentRound();
	updateMoney();
	updateHealth();
}

bool InterfaceContainer::contains(const sf::Vector2f& point) const
{
	return GUI.getGlobalBounds().contains(point);
}

void InterfaceContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(GUI, states);
	for (const auto& button : buttons)
	{
		target.draw(*button, states);
		target.draw(button->getButtonText(), states);
	}
	target.draw(getCurrentRoundText(), states);
	target.draw(getMoneyText(), states);
	target.draw(getHealthText(), states);
}
