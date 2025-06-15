#pragma once
#include "Entity.h"
#include "Enemy.h"

class Tower;

class Projectile :
    public Entity
{
public:
    Projectile(Resources::Texture textureID, const Tower& tower, float moveSpeed, float duration, const int framesCount = 1);

    virtual void Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies) = 0;
    bool isAlive() const;
protected:
    bool hitEnemy(const std::unique_ptr<Enemy>& enemy);//Перевірка, чи торкнувся снаряд ворога
    virtual void checkHitAndDamageEnemy(const std::list<std::unique_ptr<Enemy>>& enemies) = 0;
    float duration;
    float moveSpeed;
    float damage;
    bool projectileIsAlive;
    sf::Vector2f moveDirection;
};

