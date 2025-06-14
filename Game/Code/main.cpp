#include "Game.h"

using namespace std;

int main()
{
	Tower::initializeTowersStats();
	Enemy::initializeEnemiesStats();
	Resources::Initialize();
	Game game(sf::State::Windowed);
	game.Run();

	return 0;
} 