#include "Goblin.h"

Goblin::Goblin(Resources::Texture textureID, const sf::Vector2f& position, const int framesCount,
	const int healthPoints, const int money, const float moveSpeed)
	: Enemy(textureID, position, healthPoints, money, moveSpeed, framesCount)
{
}
