#include "Wolf.h"

Wolf::Wolf(Resources::Texture textureID, const sf::Vector2f& position, const int framesCount)
	: Enemy(textureID, position, WOLF_HEALTH_POINTS, WOLF_MONEY, WOLF_MOVE_SPEED, framesCount)
{
}
