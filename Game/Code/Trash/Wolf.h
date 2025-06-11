#pragma once
#include "Enemy.h"
class Wolf :
    public Enemy
{
public:
    Wolf(Resources::Texture textureID, const sf::Vector2f& position, const int framesCount = WOLF_MAX_FRAMES);
private:
    static constexpr int WOLF_MONEY = 55;
    static constexpr int WOLF_HEALTH_POINTS = 63;
    static constexpr float WOLF_MOVE_SPEED = 75.f;// - test
    //static constexpr float GOBLIN_MOVE_SPEED = 17.f; - default
    static constexpr int WOLF_MAX_FRAMES = 6;
};

