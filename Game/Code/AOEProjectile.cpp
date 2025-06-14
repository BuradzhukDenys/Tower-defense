#include "AOEProjectile.h"

AOEProjectile::AOEProjectile(Resources::Texture textureID, const Tower& tower, float moveSpeed, float duration, const int radius, const int framesCount)
	: Projectile(textureID, tower, moveSpeed, duration, framesCount), radius(radius), radiusShape(0)
{
}

void AOEProjectile::damageEnemyAOE(const std::list<std::unique_ptr<Enemy>>& enemies)
{
	for (const auto& enemy : enemies)
	{
		if (enemy->getSprite().getGlobalBounds().findIntersection(radiusShape.getGlobalBounds()))
		{
			enemy->takeDamage(damage);
		}
	}
}

void AOEProjectile::Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies)
{
	Projectile::Update(deltaTime, mousePosition, enemies);

	checkHitAndDamageEnemy(enemies);
}

void AOEProjectile::checkHitAndDamageEnemy(const std::list<std::unique_ptr<Enemy>>& enemies)
{
	for (const auto& enemy : enemies)
	{
		if (hitEnemy(enemy))
		{
			radiusShape.setRadius(radius);
			radiusShape.setOrigin(radiusShape.getGeometricCenter());
			radiusShape.setPosition(sprite.getGlobalBounds().getCenter());

			damageEnemyAOE(enemies);

			projectileIsAlive = false;
			return;
		}
	}
}
