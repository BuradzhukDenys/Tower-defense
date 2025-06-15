#include "Game.h"

int main()
{
	WavesManager::initializeWavesPattern();
	Tower::initializeTowersStats();
	Enemy::initializeEnemiesStats();
	Resources::Initialize();
	Game game(sf::State::Fullscreen);
	game.Run();

	return 0;
} 