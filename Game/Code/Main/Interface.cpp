#include "Interface.h"
#include <iostream>

int Interface::currentRound = 1;
int Interface::money = 600;
int Interface::lives = 30;
Interface::TowerType Interface::selectedTower = Interface::TowerType::None;
Interface::States Interface::currentState = Interface::States::Game;
const sf::Color Interface::PAUSE_BACKGROUND_COLOR = sf::Color(0, 0, 0, 50);

Interface::~Interface() {}

int Interface::getCurrentRound()
{
	return currentRound;
}

int Interface::getMoney()
{
	return money;
}

int Interface::getLives()
{
	return lives;
}

int Interface::getMaxRoudns()
{
	return MAX_ROUNDS;
}

void Interface::nextRound()
{
	if (currentRound < MAX_ROUNDS)
	{
		currentRound++;
	}
}

void Interface::addMoney(const int value)
{
	money += value;
}

void Interface::substractMoney(const int value)
{
	if (money > 0)
	{
		money -= value;
	}
}

void Interface::lostlives()
{
	if (lives > 0)
	{
		lives--;
	}
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

void Interface::setSelectedTower(TowerType towerType)
{
	selectedTower = towerType;
}