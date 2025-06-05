#pragma once
#include "Entity.h"

class Tower;

class Projectile :
    public Entity
{
public:
    Projectile(Resources::Texture textureID, Tower& tower, float moveSpeed, float duration);

    virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window, const std::vector<std::unique_ptr<Enemy>>& enemies) override;
    bool isAlive() const;
private:
    float duration = 0;
    float moveSpeed = 0;
    bool projectileIsAlive;
    sf::Vector2f moveDirection;
};

