#include "Game.h"

using namespace std;

int main()
{
	Resources::Initialize();
	Game game;
	game.Run();

	return 0;
} 