#pragma once
#include <SFML/Graphics.hpp>
#include "Tower.h"
#include "Interface.h"

const sf::String controlText = L"Control\nLMB - Поставити башню\nRMB - видалити башню";

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

	std::vector<std::unique_ptr<Tower>> towers;
	std::vector<std::unique_ptr<Interface>> interface;
	std::unique_ptr<Tower> pickableTower = nullptr;

	void checkSelectedTower();
	void updatePickableTower();
	void checkCanPlace();

	void Events();
	void Update(sf::Time deltaTime);
	void Render();
};

