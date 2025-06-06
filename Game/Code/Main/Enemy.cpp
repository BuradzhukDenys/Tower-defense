#include "Enemy.h"

Enemy::Enemy(Resources::Texture textureID, const sf::Vector2f& position, const int healthPoints,
	const int money, const float moveSpeed, const int framesCount)
	: Entity(textureID, framesCount),
	healthPoints(healthPoints), maxHealthPoints(healthPoints), money(money), moveSpeed(moveSpeed),
	direction(Enemy::Direction::Right), healthBar({ sprite.getLocalBounds().size.x * 1.4f, 4.f }),
	healthBarBackground({ sprite.getLocalBounds().size.x * 1.4f, 4.f })
{
	sprite.setOrigin({ sprite.getLocalBounds().position.x, sprite.getLocalBounds().getCenter().y });
	sprite.setPosition({ position.x - sprite.getLocalBounds().size.x, position.y});
	healthBarBackground.setOrigin({ healthBar.getLocalBounds().getCenter().x, 0 });
	healthBarBackground.setPosition({ sprite.getPosition().x + sprite.getLocalBounds().getCenter().x, sprite.getPosition().y + sprite.getOrigin().y + healthBar.getSize().y });
	healthBarBackground.setFillColor(sf::Color::Red);
	healthBarBackground.setOutlineColor(sf::Color::Black);
	healthBarBackground.setOutlineThickness(2.f);
	healthBar.setOrigin(healthBarBackground.getOrigin());
	healthBar.setPosition(healthBarBackground.getPosition());
	healthBar.setFillColor(sf::Color::Green);
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
	updateHealthBar();

	if (healthPoints <= 0)
	{
		enemyIsAlive = false;
	}
}

bool Enemy::isAlive()
{
	return enemyIsAlive;
}

void Enemy::updateHealthBar()
{
	float healthPercent = static_cast<float>(healthPoints) / maxHealthPoints;
	healthBar.setSize(sf::Vector2f(sprite.getLocalBounds().size.x * 1.4f * healthPercent, 4.f));
}

void Enemy::Update(sf::Time deltaTime, const sf::RenderWindow& window, const std::list<std::unique_ptr<Enemy>>& enemies)
{
	switch (direction)
	{
	case Enemy::Direction::Up:
		sprite.move(sf::Vector2f(0, -moveSpeed * deltaTime.asSeconds()));
		healthBar.move(sf::Vector2f(0, -moveSpeed * deltaTime.asSeconds()));
		healthBarBackground.move(sf::Vector2f(0, -moveSpeed * deltaTime.asSeconds()));
		break;
	case Enemy::Direction::Down:
		sprite.move(sf::Vector2f(0, moveSpeed * deltaTime.asSeconds()));
		healthBar.move(sf::Vector2f(0, moveSpeed * deltaTime.asSeconds()));
		healthBarBackground.move(sf::Vector2f(0, moveSpeed * deltaTime.asSeconds()));
		break;
	case Enemy::Direction::Right:
		sprite.move(sf::Vector2f(moveSpeed * deltaTime.asSeconds(), 0));
		healthBar.move(sf::Vector2f(moveSpeed * deltaTime.asSeconds(), 0));
		healthBarBackground.move(sf::Vector2f(moveSpeed * deltaTime.asSeconds(), 0));
		break;
	default:
		break;
	}
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
	target.draw(healthBarBackground, states);
	target.draw(healthBar, states);
}
