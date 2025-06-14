#include "StandartProjectile.h"

StandartProjectile::StandartProjectile(Resources::Texture textureID, const Tower& tower, float moveSpeed, float duration, const int framesCount)
	: Projectile(textureID, tower, moveSpeed, duration, framesCount)
{
}

void StandartProjectile::Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies)
{
	Projectile::Update(deltaTime, mousePosition, enemies);
	checkHitAndDamageEnemy(enemies);
}

void StandartProjectile::checkHitAndDamageEnemy(const std::list<std::unique_ptr<Enemy>>& enemies)
{
	for (const auto& enemy : enemies)
	{
		if (hitEnemy(enemy))
		{
			enemy->takeDamage(damage);
			projectileIsAlive = false;
			return;
		}
	}
}
