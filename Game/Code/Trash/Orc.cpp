#include "Orc.h"

Orc::Orc(Resources::Texture textureID, const sf::Vector2f& position, const int framesCount)
	: Enemy(textureID, position, ORC_HEALTH_POINTS, ORC_MONEY, ORC_MOVE_SPEED, framesCount)
{
}
