#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Resources.h"

class Interface : public sf::Drawable
{
public:
	enum class SelectedTowerType
	{
		Ballista,
		Bomber,
		Wizzard,
		None
	};

	enum class InterfaceType
	{
		SelectTowerInterface,
		PauseInterface,
		LoserInterface,
		WinnerInterface
	};

	static constexpr int MAX_WAVES = 25;
	static const sf::Color BASE_BACKGROUND_COLOR;

	virtual ~Interface();

	static SelectedTowerType getSelectedTower();
	static void setSelectedTower(SelectedTowerType towerType);

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
	static SelectedTowerType selectedTower;
	static int currentWave;
	static int money;
	static int lives;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};