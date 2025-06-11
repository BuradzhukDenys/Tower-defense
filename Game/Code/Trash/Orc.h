#pragma once
#include "Enemy.h"
class Orc :
    public Enemy
{
public:
    Orc(Resources::Texture textureID, const sf::Vector2f& position, const int framesCount = ORC_MAX_FRAMES);
private:
    static constexpr int ORC_MONEY = 135;
    static constexpr int ORC_HEALTH_POINTS = 110;
    static constexpr float ORC_MOVE_SPEED = 28.f;// - test
    //static constexpr float GOBLIN_MOVE_SPEED = 17.f; - default
    static constexpr int ORC_MAX_FRAMES = 6;
};

