#include "Ballista.h"
#include <iostream>

Ballista::Ballista(Resources::Texture textureID, const sf::Vector2f& position, const int framesCount)
	: Tower(textureID, position, BALLISTA_PRICE, BALLISTA_DAMAGE, BALLISTA_ATTACK_SPEED, BALLISTA_ATTACK_RANGE, framesCount),
	framesCount(framesCount), currentFrame(0), timeForLastAnimationPlay(0)
{
	sprite.setTextureRect(sf::IntRect({ 0, 0 }, frameSize));
	sprite.setOrigin(sf::Vector2f(sprite.getTextureRect().size / 2));
	sprite.setPosition(position);

	radius.setRadius(attackRange);
	radius.setOrigin(radius.getGeometricCenter());
	radius.setPosition(sprite.getPosition());
}

void Ballista::Update(sf::Time deltaTime, const sf::RenderWindow& window, const std::vector<std::unique_ptr<Enemy>>& enemies)
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

	//------------followTheEnemy(Tower.h)-------------------//
	/*sf::Vector2f towerPosition(sprite.getPosition());

	sf::Vector2f distanceToCursor = mousePosition - towerPosition;
	float angleInRadians = std::atan2(distanceToCursor.y, distanceToCursor.x);
	sf::Angle angle(sf::radians(angleInRadians));
	if (radius.getGlobalBounds().contains(mousePosition))
	{
		sprite.setRotation(angle);
		shoot();
	}*/
	//-------------------------------------------------------//

	for (const auto& enemy : enemies)
	{
		if (!enemy) continue;
		if (inRadius(enemy->getPosition()))
		{
			// Повертаємо вежу на ворога
			sf::Vector2f towerPos = sprite.getPosition();
			sf::Vector2f enemyPos = enemy->getPosition();
			sf::Vector2f toEnemy = enemyPos - towerPos;
			float angleRad = std::atan2(toEnemy.y, toEnemy.x);
			sprite.setRotation(sf::radians(angleRad));

			// Стріляємо
			shoot();
			break; // Тільки по першому ворогу в радіусі
		}
	}

	if (isAnimationPlaying)
	{
		playAnimation(deltaTime);
	}

	isActive = intersects(mousePosition);
}

void Ballista::playAnimation(sf::Time deltaTime)
{
	timeForLastAnimationPlay += deltaTime.asSeconds();
	if (timeForLastAnimationPlay >= animationSpeed)
	{
		currentFrame++;
		timeForLastAnimationPlay = 0.f;

		if (currentFrame < framesCount)
		{
			sprite.setTextureRect(sf::IntRect(
				{ currentFrame * frameSize.x, 0 },
				frameSize));
		}
		else
		{
			currentFrame = 0;
			isAnimationPlaying = false;
			sprite.setTextureRect(sf::IntRect({ 0, 0 }, frameSize));
		}
	}
}

void Ballista::shoot()
{	
	if (timeBetweenShots <= 0)
	{
		projectiles.emplace_back(std::make_unique<Projectile>(
			Resources::Texture::BallistaShot,
			*this,
			BALLISTA_PROJECTILE_SPEED,
			BALLISTA_PROJECTILE_DURATION
		));
		timeBetweenShots = 1.f / attackSpeed;
		isAnimationPlaying = true;
		currentFrame = 0;
		timeForLastAnimationPlay = 0;
	}
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
