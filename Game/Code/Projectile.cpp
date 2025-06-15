#include "Projectile.h"
#include "Tower.h"

Projectile::Projectile(Resources::Texture textureID, const Tower& tower, float moveSpeed, float duration, const int framesCount)
	: Entity(textureID, framesCount), moveSpeed(moveSpeed), duration(duration), projectileIsAlive(true), damage(tower.getDamage())
{
	//Створюємо снаряд в позиції вежі, та ставимо кут повроту до ворога
	sprite.setPosition(tower.getPosition());
	sprite.setRotation(tower.getRotateAngleToEnemy());

	//Визначаємо напрямко руху снаряда
	float angleInRadians = tower.getRotateAngleToEnemy().asRadians();
	moveDirection = sf::Vector2f(std::cos(angleInRadians), std::sin(angleInRadians));
}

void Projectile::Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies)
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

bool Projectile::hitEnemy(const std::unique_ptr<Enemy>& enemy)
{
	if (enemy->getSprite().getGlobalBounds().findIntersection(sprite.getGlobalBounds()))
	{
		return true;
	}

	return false;
}
