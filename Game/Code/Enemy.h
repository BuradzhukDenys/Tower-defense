#pragma once
#include "Entity.h"
#include <map>

namespace EnemiesFrames
{
    static constexpr int GOBLIN_MAX_FRAMES = 6;
    static constexpr int ORC_MAX_FRAMES = 6;
    static constexpr int WOLF_MAX_FRAMES = 6;
}

class Enemy :
    public Entity
{
public:
    enum class EnemyType
    {
        Goblin,
        Orc,
        Wolf
    };

    enum class Direction
    {
        Up,
        Down,
        Right
    };

    Enemy(const EnemyType type, Resources::Texture textureID, const sf::Vector2f& position, const int framesCount = 1);

    const Direction& getDirection() const;
    void setDirection(const Direction& direction);
    void takeDamage(const float damage);
    int getMoneyReward() const;
    bool isAlive() const;

    Enemy::EnemyType& getType();
    float getHealth() const;//delete
    void piersed(bool wasPiersed);
    bool wasPiersed() const;

    void updateHealthBar();
    virtual void playAnimation(sf::Time deltaTime) override;
    virtual void Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies) override;

    static void initializeEnemiesStats();
private:
    struct EnemyStats
    {
        int moneyReward;
        float healthPoints;
        float moveSpeed;
    };
    int moneyReward = 0;
    float healthPoints = 0;
    float moveSpeed = 0;
    float animationSpeed = 0.25;

    EnemyType type;
    Direction direction;
    float distancePerFrame = 6.f;
    float distanceForLastFrame = 0.f;
    bool enemyIsAlive = true;
    bool piersedEnemy = false;

    int maxHealthPoints;
    sf::RectangleShape healthBar;
    sf::RectangleShape healthBarBackground;

    static std::map<EnemyType, EnemyStats> enemiesStatsMap;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

