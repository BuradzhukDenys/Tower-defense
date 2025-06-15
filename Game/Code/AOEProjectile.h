#pragma once
#include "Projectile.h"
class AOEProjectile :
    public Projectile
{
public:
    AOEProjectile(Resources::Texture textureID, const Tower& tower, float moveSpeed, float duration, const int radius, const int framesCount = 1);

    void damageEnemyAOE(const std::list<std::unique_ptr<Enemy>>& enemies, const sf::CircleShape& radiusShape);
    virtual void Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies) override;
    virtual void checkHitAndDamageEnemy(const std::list<std::unique_ptr<Enemy>>& enemies) override;
private:
    int radius;
};

