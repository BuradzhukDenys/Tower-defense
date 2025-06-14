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

	static constexpr int MAX_WAVES = 25;
	static const sf::Color BASE_BACKGROUND_COLOR;

	virtual ~Interface();

	static TowerType getSelectedTower();
	static void setSelectedTower(TowerType towerType);

	static int getCurrentWave();
	static int getMoney();
	static int getLives();
	static int getMoneyAfterWave();

	static void nextWave();
	static void addMoney(const int value);
	static void substractMoney(const int value);
	static void lostlives();

	static void reset();
	virtual void Update(sf::Time deltaTime, const sf::Vector2f& mousePosition) = 0;
private:
	static constexpr int START_WAVE = 1;
	static constexpr int START_MONEY = 550;
	static constexpr int START_LIVES = 10;
	static constexpr int MONEY_AFTER_WAVE = 85;
	static TowerType selectedTower;
	static int currentWave;
	static int money;
	static int lives;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};