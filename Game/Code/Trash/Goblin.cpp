#include "Goblin.h"

Goblin::Goblin(Resources::Texture textureID, const sf::Vector2f& position, const int framesCount)
	: Enemy(textureID, position, GOBLIN_HEALTH_POINTS, GOBLIN_MONEY, GOBLIN_MOVE_SPEED, framesCount)
{
}
