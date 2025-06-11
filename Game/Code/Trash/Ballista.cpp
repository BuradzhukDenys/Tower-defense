#include "Ballista.h"
#include <iostream>

//Ballista::Ballista(Resources::Texture textureID, const sf::Vector2f& position, const int framesCount)
//	: Tower(textureID, position, BALLISTA_PRICE, BALLISTA_DAMAGE, BALLISTA_ATTACK_SPEED, BALLISTA_ATTACK_RANGE, framesCount)
//{
//}

int Ballista::getPrice()
{
	return BALLISTA_PRICE;
}

void Ballista::Update(sf::Time deltaTime, const sf::RenderWindow& window, const std::list<std::unique_ptr<Enemy>>& enemies)
{
	timeBetweenShots -= deltaTime.asSeconds();
	sf::Vector2f mousePosition(sf::Mouse::getPosition(window));

	for (auto& shot : projectiles)
	{
		shot->Update(deltaTime, window, enemies);
	}

	for (auto it = projectiles.begin(); it != projectiles.end(); ++it)
	{
		if (!(*it)->isAlive())
		{
			projectiles.erase(it);
			break;
		}
	}

	followTheEnemyAndShoot(enemies);

	if (isAnimationPlaying)
	{
		playAnimation(deltaTime);
	}

	isActive = intersects(mousePosition);
}

void Ballista::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
	for (auto& shot : projectiles)
	{
		target.draw(*shot, states);
	}

	if (isActive)
	{
		target.draw(radius, states);
	}
}
