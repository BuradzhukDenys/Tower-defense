#pragma once
#include "Projectile.h"
class StandartProjectile :
    public Projectile
{
public:
    StandartProjectile(Resources::Texture textureID, const Tower& tower, float moveSpeed, float duration, const int framesCount = 1);

    virtual void Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies) override;
};

