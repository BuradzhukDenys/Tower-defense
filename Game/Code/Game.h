#pragma once
#include <SFML/Graphics.hpp>
#include "Tower.h"
#include "Interface.h"
#include "Map.h"
#include "Enemy.h"
#include "GameState.h"
#include "WavesManager.h"
#include <map>

class Game
{
public:
	Game(const sf::State& windowState);

	void Run();
private:
	const sf::String windowTitle = "Tower Defense";
	sf::VideoMode windowSize;
	sf::RenderWindow window;

	sf::Vector2f mousePosition;
	bool canPlaceTower = true;

	sf::Text currentWaveText;
	sf::Text moneyText;
	sf::Text livesText;
	const int GAME_FONT_SIZE = 35;
	const float MARGIN_ROWS = 10.f;
	const float MARGIN_BORDERS = 45.f;

	Map map;
	float timeToShowMoneyTextColor = 0.3;

	std::list<std::unique_ptr<Tower>> towers;
	std::map<Interface::InterfaceType, std::unique_ptr<Interface>> interface;
	std::list<std::unique_ptr<Enemy>> enemies;
	std::list<std::unique_ptr<sf::Sound>> sounds;
	sf::Music& music;
	std::unique_ptr<Tower> pickableTower = nullptr;

	bool notEnoughMoney = false;
	bool towerSelled = false;

	void checkSelectedTower();
	void updatePickableTower();
	void checkCanPlace();
	void placeTower();
	void addInterfaceContainer(const Interface::InterfaceType& interfaceType,
		const sf::Vector2f& containerSize,
		const sf::Vector2f& containerPosition,
		const sf::Color& containerColor);
	InterfaceContainer& getInterfaceContainer(const Interface::InterfaceType& interfaceType);
	void initializeInterface();
	void initializeGameInfo();
	void updateGameInfo(sf::Time deltaTime);
	void checkVictory();
	void checkLoss();
	void Pause();
	void playSounds();
	void showGameInfo();

	void Events();
	void Update(sf::Time deltaTime);
	void Render();
};

