#include "Interface.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "Resources.h"
#include <iostream>

int Interface::currentRound = 1;
int Interface::money = 600;
int Interface::health = 30;
Interface::TowerType Interface::selectedTower = Interface::TowerType::None;
Interface::States Interface::currentState = Interface::States::Game;

Interface::Interface(const sf::Vector2f& size, const sf::Vector2f& position)
	: GUI(size),
	currentRoundText(Resources::fonts.Get(Resources::Font::BasicFont)),
	moneyText(Resources::fonts.Get(Resources::Font::BasicFont)),
	healthText(Resources::fonts.Get(Resources::Font::BasicFont))
{
	GUI.setPosition(position);

	currentRoundText.setString("Round: " + std::to_string(currentRound) + "/25");
	currentRoundText.setCharacterSize(GAME_FONT_SIZE);
	currentRoundText.setOrigin({ currentRoundText.getGlobalBounds().size.x, 0 });
	currentRoundText.setPosition({
		GUI.getPosition().x - GAME_FONT_SIZE,
		GUI.getPosition().y
	});

	moneyText.setString("Money: " + std::to_string(money));
	moneyText.setCharacterSize(GAME_FONT_SIZE);
	moneyText.setOrigin({ moneyText.getGlobalBounds().size.x, 0 });
	moneyText.setPosition({
		GUI.getPosition().x - GAME_FONT_SIZE,
		GUI.getPosition().y + currentRoundText.getGlobalBounds().size.y
	});

	healthText.setString("Health: " + std::to_string(health));
	healthText.setCharacterSize(GAME_FONT_SIZE);
	healthText.setOrigin({ healthText.getGlobalBounds().size.x, 0 });
	healthText.setPosition({
		GUI.getPosition().x - GAME_FONT_SIZE,
		GUI.getPosition().y + 100
	});
}

Interface::~Interface() {}

void Interface::setSelectedTower(TowerType towerType)
{
	selectedTower = towerType;
}

const sf::Text& Interface::getCurrentRoundText() const
{
	return currentRoundText;
}

const sf::Text& Interface::getMoneyText() const
{
	return moneyText;
}

const sf::Text& Interface::getHealthText() const
{
	return healthText;
}

int Interface::getCurrentRound()
{
	return currentRound;
}

int Interface::getMoney()
{
	return money;
}

int Interface::getHealth()
{
	return health;
}

const sf::Vector2f& Interface::getSize()
{
	return GUI.getSize();
}

const sf::Vector2f& Interface::getPosition()
{
	return GUI.getPosition();
}

void Interface::nextRound()
{
	currentRound++;
}

void Interface::addMoney(const int value)
{
	money += value;
}

void Interface::substractMoney(const int value)
{
	money -= value;
}

void Interface::lostHealth()
{
	health--;
}

void Interface::updateCurrentRound()
{
	currentRoundText.setString("Round: " + std::to_string(currentRound) + "/25");
}

void Interface::updateMoney()
{
	moneyText.setString("Money: " + std::to_string(money));
}

void Interface::updateHealth()
{
	healthText.setString("Health: " + std::to_string(health));
}

Interface::States Interface::getCurrentState()
{
	return currentState;
}

void Interface::setState(const States& state)
{
	currentState = state;
}

Interface::TowerType Interface::getSelectedTower()
{
	return selectedTower;
}

void Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(GUI, states);
}
