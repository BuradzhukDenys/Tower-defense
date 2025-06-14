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

	enum class InterfaceType
	{
		SelectTowerInterface,
		UpgradeTowerInterface,
		PauseInterface,
		LoserInterface,
		WinnerInterface
	};

	static const sf::Color BASE_BACKGROUND_COLOR;

	virtual ~Interface();

	static TowerType getSelectedTower();
	static void setSelectedTower(TowerType towerType);

	static int getCurrentRound();
	static int getMoney();
	static int getLives();
	static int getMaxRoudns();

	static void nextRound();
	static void addMoney(const int value);
	static void substractMoney(const int value);
	static void lostlives();

	static void reset();
	virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window) = 0;
private:
	static constexpr int MAX_ROUNDS = 25;
	static constexpr int START_ROUND = 1;
	static constexpr int START_MONEY = 550;
	static constexpr int START_LIVES = 30;
	static TowerType selectedTower;
	static int currentRound;
	static int money;
	static int lives;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};