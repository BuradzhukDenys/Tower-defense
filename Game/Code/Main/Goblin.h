#pragma once
#include "Enemy.h"
class Goblin :
    public Enemy
{
public:
    Goblin(Resources::Texture textureID, const sf::Vector2f& position, const int framesCount = GOBLIN_MAX_FRAMES,
        const int healthPoints = GOBLIN_HEALTH_POINTS, const int money = GOBLIN_MONEY, const float moveSpeed = GOBLIN_MOVE_SPEED);
private:
    static constexpr int GOBLIN_MONEY = 85;
    static constexpr int GOBLIN_HEALTH_POINTS = 55;
    static constexpr float GOBLIN_MOVE_SPEED = 40.f;// - test
    //static constexpr float GOBLIN_MOVE_SPEED = 17.f; - default
    static constexpr int GOBLIN_MAX_FRAMES = 6;
};

