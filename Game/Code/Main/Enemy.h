#pragma once
#include "Entity.h"
class Enemy :
    public Entity
{
public:
    enum class Direction
    {
        Up,
        Down,
        Right
    };

    Enemy(Resources::Texture textureID, const sf::Vector2f& position, const int healthPoints,
        const int money, const float moveSpeed, const int framesCount = 1);

    const Direction& getDirection() const;
    void setDirection(const Direction& direction);
    void takeDamage(const float damage);
    int getMoney();
    bool isAlive();

    virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window, const std::list<std::unique_ptr<Enemy>>& enemies) override;
protected:
    Direction direction;
private:
    bool enemyIsAlive = true;
    int money;
    int healthPoints;
    float moveSpeed;
    sf::RectangleShape healthBar;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

