#include "Projectile.h"
#include "Tower.h"
#include <iostream>

Projectile::Projectile(Resources::Texture textureID, Tower& tower, float moveSpeed, float duration)
	: Entity(textureID), moveSpeed(moveSpeed), duration(duration), projectileIsAlive(true)
{
	sprite.setPosition(tower.getPosition());
	sprite.setRotation(tower.getRotation());

	float angleInRadians = tower.getRotation().asRadians();
	moveDirection = sf::Vector2f(std::cos(angleInRadians), std::sin(angleInRadians));
}

void Projectile::Update(sf::Time deltaTime, const sf::RenderWindow& window)
{
	sprite.move(moveDirection * moveSpeed * deltaTime.asSeconds());

	duration -= deltaTime.asSeconds();
	if (duration < 0)
	{
		projectileIsAlive = false;
	}
}

bool Projectile::isAlive() const
{
	return projectileIsAlive;
}
