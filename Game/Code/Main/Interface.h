#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Resources.h"

namespace sf
{
	class Time;
}

class Interface : public sf::Drawable
{
public:
	enum class TowerType
	{
		Ballista,
		Bomber,
		Wizzard,
		None
	};

	enum class States
	{
		Game,
		Pause
	};

	enum class InterfaceType
	{
		SelectTowerInterface,
		UpgradeTowerInterface,
		PauseInterface
	};

	static const sf::Color PAUSE_BACKGROUND_COLOR;

	virtual ~Interface();

	static TowerType getSelectedTower();
	static void setSelectedTower(TowerType towerType);
	static States getCurrentState();
	static void setState(const States& state);

	static int getCurrentRound();
	static int getMoney();
	static int getLives();

	static void nextRound();
	static void addMoney(const int value);
	static void substractMoney(const int value);
	static void lostlives();

	virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window) = 0;
private:
	static States currentState;
	static TowerType selectedTower;
	static int currentRound;
	static int money;
	static int lives;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};