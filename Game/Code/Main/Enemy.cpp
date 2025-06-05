#include "Enemy.h"

Enemy::Enemy(Resources::Texture textureID, const sf::Vector2f& position, const int healthPoints,
	const int money, const float moveSpeed, const int framesCount)
	: Entity(textureID, framesCount),
	healthPoints(healthPoints), money(money), moveSpeed(moveSpeed), direction(Enemy::Direction::Right),
	healthBar({ sprite.getLocalBounds().size.x * 1.4f, 4.f })
{
	sprite.setOrigin({ sprite.getLocalBounds().position.x, sprite.getLocalBounds().getCenter().y });
	sprite.setPosition({ position.x - sprite.getLocalBounds().size.x, position.y});
	healthBar.setOrigin({ healthBar.getLocalBounds().getCenter().x, 0 });
	healthBar.setPosition({ sprite.getPosition().x + sprite.getLocalBounds().getCenter().x, sprite.getPosition().y + sprite.getOrigin().y + healthBar.getSize().y});
	healthBar.setFillColor(sf::Color::Green);
	healthBar.setOutlineColor(sf::Color::Black);
	healthBar.setOutlineThickness(2.f);
}

const Enemy::Direction& Enemy::getDirection() const
{
	return direction;
}

void Enemy::setDirection(const Direction& direction)
{
	this->direction = direction;
}

int Enemy::getMoney()
{
	return money;
}

void Enemy::takeDamage(const float damage)
{
	healthPoints -= damage;
}

bool Enemy::isAlive()
{
	return enemyIsAlive;
}

void Enemy::Update(sf::Time deltaTime, const sf::RenderWindow& window, const std::list<std::unique_ptr<Enemy>>& enemies)
{
	switch (direction)
	{
	case Enemy::Direction::Up:
		sprite.move(sf::Vector2f(0, -moveSpeed * deltaTime.asSeconds()));
		healthBar.move(sf::Vector2f(0, -moveSpeed * deltaTime.asSeconds()));
		break;
	case Enemy::Direction::Down:
		sprite.move(sf::Vector2f(0, moveSpeed * deltaTime.asSeconds()));
		healthBar.move(sf::Vector2f(0, moveSpeed * deltaTime.asSeconds()));
		break;
	case Enemy::Direction::Right:
		sprite.move(sf::Vector2f(moveSpeed * deltaTime.asSeconds(), 0));
		healthBar.move(sf::Vector2f(moveSpeed * deltaTime.asSeconds(), 0));
		break;
	default:
		break;
	}

	if (healthPoints <= 0)
	{
		enemyIsAlive = false;
	}
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
	target.draw(healthBar, states);
}
