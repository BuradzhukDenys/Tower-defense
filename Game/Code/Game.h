#pragma once
#include <map>
#include "Interface.h"
#include "InterfaceContainer.h"
#include "Tower.h"
#include "Enemy.h"
#include "Map.h"
#include "GameState.h"
#include "WavesManager.h"

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
	float timeToShowMoneyTextColor = 0.3;
	sf::Text fps;//delete
	sf::Text control;

	Map map;

	std::unique_ptr<Tower> pickableTower = nullptr;
	std::list<std::unique_ptr<Tower>> towers;
	std::list<std::unique_ptr<Enemy>> enemies;
	sf::Music& music;
	std::list<std::unique_ptr<sf::Sound>> sounds;
	std::map<Interface::InterfaceType, std::unique_ptr<Interface>> interface;

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
	void playSound(const Resources::Sound soundId, float volume);
	void SoundsManager();
	void showGameInfo();

	void Events();
	void Update(sf::Time deltaTime);
	void Render();
};

