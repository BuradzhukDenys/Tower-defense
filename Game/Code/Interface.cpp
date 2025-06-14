#include "Interface.h"
#include <iostream>

int Interface::currentWave = Interface::START_WAVE;
int Interface::money = Interface::START_MONEY;
int Interface::lives = Interface::START_LIVES;
Interface::TowerType Interface::selectedTower = Interface::TowerType::None;
const sf::Color Interface::BASE_BACKGROUND_COLOR = sf::Color(0, 0, 0, 50);

Interface::~Interface() {}

int Interface::getCurrentWave()
{
	return currentWave;
}

int Interface::getMoney()
{
	return money;
}

int Interface::getLives()
{
	return lives;
}

int Interface::getMoneyAfterWave()
{
	return MONEY_AFTER_WAVE;
}

void Interface::nextWave()
{
	if (currentWave < MAX_WAVES)
	{
		currentWave++;
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

Interface::TowerType Interface::getSelectedTower()
{
	return selectedTower;
}

void Interface::setSelectedTower(TowerType towerType)
{
	selectedTower = towerType;
}

void Interface::reset()
{
	currentWave = START_WAVE;
	money = START_MONEY;
	lives = START_LIVES;
}