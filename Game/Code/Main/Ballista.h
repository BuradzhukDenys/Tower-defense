#pragma once
#include "Tower.h"

class Ballista :
    public Tower
{
public:
    Ballista(Resources::Texture textureID, const sf::Vector2f& position);

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
    static constexpr int MAX_FRAMES = 5;
    int currentFrame;
    float timeForLastAnimationPlay;
    float animationSpeed = (attackSpeed / MAX_FRAMES) - 0.08f;
    bool isAnimationPlaying = false;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

