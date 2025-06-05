#include "Enemy.h"

Enemy::Enemy(Resources::Texture textureID, const sf::Vector2f& position, const int healthPoints,
	const int money, const float moveSpeed, const int framesCount)
	: Entity(textureID, framesCount),
	healthPoints(healthPoints), money(money), moveSpeed(moveSpeed), direction(Enemy::Direction::Right)
{
	sprite.setOrigin({ sprite.getLocalBounds().position.x, sprite.getLocalBounds().getCenter().y });
	sprite.setPosition({ position.x - sprite.getOrigin().x, position.y });
}

const Enemy::Direction& Enemy::getDirection() const
{
	return direction;
}

void Enemy::setDirection(const Direction& direction)
{
	this->direction = direction;
}

void Enemy::Update(sf::Time deltaTime, const sf::RenderWindow& window)
{
	switch (direction)
	{
	case Enemy::Direction::Up:
		sprite.move(sf::Vector2f(0, -moveSpeed * deltaTime.asSeconds()));
		break;
	case Enemy::Direction::Down:
		sprite.move(sf::Vector2f(0, moveSpeed * deltaTime.asSeconds()));
		break;
	case Enemy::Direction::Right:
		sprite.move(sf::Vector2f(moveSpeed * deltaTime.asSeconds(), 0));
		break;
	default:
		break;
	}
}
