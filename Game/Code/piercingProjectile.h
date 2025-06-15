#pragma once
#include "Projectile.h"

class piercingProjectile
	: public Projectile
{
public:
	piercingProjectile(Resources::Texture textureID, const Tower& tower, float moveSpeed, float duration, const int piercing, const int framesCount = 1);

	virtual void Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies) override;
	virtual void checkHitAndDamageEnemy(const std::list<std::unique_ptr<Enemy>>& enemies) override;
private:
	int piercing;//Максимальна кількість ворогів, яку може пробити снаряд
};

