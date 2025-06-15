#include "piercingProjectile.h"

piercingProjectile::piercingProjectile(Resources::Texture textureID, const Tower& tower, float moveSpeed, float duration, const int piercing, const int framesCount)
	: Projectile(textureID, tower, moveSpeed, duration, framesCount), piercing(piercing)
{
}

void piercingProjectile::Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies)
{
	Projectile::Update(deltaTime, mousePosition, enemies);

	checkHitAndDamageEnemy(enemies);
}

void piercingProjectile::checkHitAndDamageEnemy(const std::list<std::unique_ptr<Enemy>>& enemies)
{
	for (const auto& enemy : enemies)
	{
		//якщо снар€д влучив в ворога, ≥ в≥н не був пробитий до цього, робимо його пробитим,
		//наносимо шкоду, та змуншуЇмо л≥чильник пробитих ворог≥в
		if (hitEnemy(enemy))
		{
			if (!enemy->wasPiersed())
			{
				enemy->piersed(true);
				enemy->takeDamage(damage);
				piercing--;
			}
		}
		else
			enemy->piersed(false);

		if (piercing == 0)
		{
			projectileIsAlive = false;
			return;
		}
	}
}
