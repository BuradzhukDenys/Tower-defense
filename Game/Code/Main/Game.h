#pragma once
#include <SFML/Graphics.hpp>
#include "Tower.h"
#include "Interface.h"
#include "Map.h"
#include <map>

const sf::String controlText = L"Control\nLMB - ��������� �����\nRMB - �������� �����";

class Game
{
public:
	Game();

	void Run();
private:
	const sf::String windowTitle = "Tower Defense";
	sf::VideoMode windowSize;
	sf::RenderWindow window;

	sf::Vector2f mousePosition;
	bool canPlaceTower = true;

	sf::Text currentRoundText;
	sf::Text moneyText;
	sf::Text livesText;
	const int GAME_FONT_SIZE = 35;
	const float MARGIN_ROWS = 10.f;
	const float MARGIN_BORDERS = 45.f;

	Map map;

	std::vector<std::unique_ptr<Tower>> towers;
	std::map<Interface::InterfaceType, std::unique_ptr<Interface>> interface;
	std::unique_ptr<Tower> pickableTower = nullptr;

	void checkSelectedTower();
	void updatePickableTower();
	void checkCanPlace();
	void addInterfaceContainer(const Interface::InterfaceType& interfaceType,
		const sf::Vector2f& containerSize,
		const sf::Vector2f& containerPosition,
		const sf::Color& containerColor);
	void deleteInterfaceContainer(const Interface::InterfaceType& interfaceType);
	void initializeInterface();
	void initializeGameInfo();
	void updateGameInfo();
	void showGameInfo();

	void Events();
	void Update(sf::Time deltaTime);
	void Render();
};

