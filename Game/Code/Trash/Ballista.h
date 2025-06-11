#pragma once
#include "Tower.h"

class Ballista :
    public Tower
{
public:
    //Ballista(Resources::Texture textureID, const sf::Vector2f& position, const int framesCount = 1);

    static int getPrice();
    virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window, const std::list<std::unique_ptr<Enemy>>& enemies) override;
private:
    static constexpr int BALLISTA_PRICE = 300;
    static constexpr float BALLISTA_DAMAGE = 21.f;
    static constexpr float BALLISTA_ATTACK_SPEED = 1.f;
    static constexpr float BALLISTA_ATTACK_RANGE = 200.f;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

