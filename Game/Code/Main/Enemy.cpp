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

void Enemy::playAnimation(sf::Time deltaTime)
{
	if (distanceForLastFrame >= distancePerFrame)
	{
		distanceForLastFrame = 0.f;
		currentFrame = (currentFrame + 1) % framesCount;
		sprite.setTextureRect(sf::IntRect(
			{ frameSize.x * currentFrame, 0 },
			{ frameSize.x, frameSize.y }
		));
	}
}

void Enemy::Update(sf::Time deltaTime, const sf::RenderWindow& window, const std::list<std::unique_ptr<Enemy>>& enemies)
{
	sf::Vector2f moveOffset;
	switch (direction)
	{
	case Enemy::Direction::Up:
		moveOffset.y = -moveSpeed * deltaTime.asSeconds();
		break;
	case Enemy::Direction::Down:
		moveOffset.y = moveSpeed * deltaTime.asSeconds();
		break;
	case Enemy::Direction::Right:
		moveOffset.x = moveSpeed * deltaTime.asSeconds();
		break;
	default:
		break;
	}

	sprite.move(moveOffset);
	healthBar.move(moveOffset);
	healthBarBackground.move(moveOffset);

	distanceForLastFrame += std::sqrt(moveOffset.x * moveOffset.x + moveOffset.y * moveOffset.y);
	playAnimation(deltaTime);
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
	target.draw(healthBarBackground, states);
	target.draw(healthBar, states);
}
