#include "AOEProjectile.h"

AOEProjectile::AOEProjectile(Resources::Texture textureID, const Tower& tower, float moveSpeed, float duration, const int radius, const float radiusDamage, const int framesCount)
	: Projectile(textureID, tower, moveSpeed, duration, framesCount), radiusShape(radius), radiusDamage(radiusDamage)
{
	radiusShape.setOrigin(radiusShape.getGeometricCenter());
	radiusShape.setFillColor(sf::Color::Red);
}

void AOEProjectile::hitEnemyAOE(const std::list<std::unique_ptr<Enemy>>& enemies)
{
	for (const auto& enemy : enemies)
	{
		if (!enemy) continue;
		if (enemy->getSprite().getGlobalBounds().findIntersection(radiusShape.getGlobalBounds()))
		{
			enemy->takeDamage(radiusDamage);
		}
	}
}

void AOEProjectile::Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies)
{
	Projectile::Update(deltaTime, mousePosition, enemies);
	
	if (hitEnemy(enemies))
	{
		radiusShape.setPosition(sprite.getGlobalBounds().getCenter());
		hitEnemyAOE(enemies);
	}
}