#include "Projectile.h"
#include "Tower.h"
#include <iostream>

Projectile::Projectile(Resources::Texture textureID, Tower& tower, float moveSpeed, float duration, float towerDamage)
	: Entity(textureID), moveSpeed(moveSpeed), duration(duration), projectileIsAlive(true), towerDamage(towerDamage)
{
	sprite.setPosition(tower.getPosition());
	sprite.setRotation(tower.getRotation());

	float angleInRadians = tower.getRotation().asRadians();
	moveDirection = sf::Vector2f(std::cos(angleInRadians), std::sin(angleInRadians));
}

void Projectile::Update(sf::Time deltaTime, const sf::RenderWindow& window, const std::list<std::unique_ptr<Enemy>>& enemies)
{
	sprite.move(moveDirection * moveSpeed * deltaTime.asSeconds());

	duration -= deltaTime.asSeconds();
	if (duration < 0)
	{
		projectileIsAlive = false;
	}

	for (const auto& enemy : enemies)
	{
		if (!enemy) continue;
		if (enemy->getSprite().getGlobalBounds().findIntersection(sprite.getGlobalBounds()))
		{
			enemy->takeDamage(towerDamage);
			projectileIsAlive = false;
			break;
		}
	}
}

bool Projectile::isAlive() const
{
	return projectileIsAlive;
}
