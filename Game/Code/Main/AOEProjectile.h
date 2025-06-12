#pragma once
#include "Projectile.h"
class AOEProjectile :
    public Projectile
{
public:
    AOEProjectile(Resources::Texture textureID, const Tower& tower, float moveSpeed, float duration, const int radius, const float radiusDamage, const int framesCount = 1);

    void hitEnemyAOE(const std::list<std::unique_ptr<Enemy>>& enemies);
    virtual void Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies) override;
private:
    sf::CircleShape radiusShape;
    float radiusDamage;
};

