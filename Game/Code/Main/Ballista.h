#pragma once
#include "Tower.h"

class Ballista :
    public Tower
{
public:
    Ballista(Resources::Texture textureID, const sf::Vector2f& position, const int framesCount = BASE_BALLISTA_MAX_FRAMES);

    virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window) override;
    virtual void playAnimation(sf::Time deltaTime) override;
    virtual void shoot() override;
private:
    static constexpr int BALLISTA_PRICE = 400;
    static constexpr float BALLISTA_DAMAGE = 50.f;
    static constexpr float BALLISTA_ATTACK_SPEED = 1.f;
    static constexpr float BALLISTA_ATTACK_RANGE = 200.f;
    static constexpr float BALLISTA_PROJECTILE_SPEED = 300.f;
    static constexpr float BALLISTA_PROJECTILE_DURATION = 5.f;
    int framesCount;
    int currentFrame;
    float timeForLastAnimationPlay;
    float animationSpeed = (1.f / attackSpeed) / (framesCount + 1);
    bool isAnimationPlaying = false;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

