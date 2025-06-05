#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Entity.h"
#include "Projectile.h"
#include "InterfaceContainer.h"
#include "Enemy.h"

class Tower : public Entity
{
public:
	Tower(Resources::Texture textureID, const sf::Vector2f& position, const int price,
		const float damage, const float attackSpeed, const float attackRange, const int framesCount = 1);

	bool followTheEnemy(const Entity& enemy);
	bool intersects(const sf::FloatRect& rect) const;
	bool intersects(const Tower& other) const;
	bool intersects(const sf::Vector2f& point) const;
	bool inRadius(const sf::Vector2f& point) const;

	enum class UpgradeType
	{
		multiplier,
		additive
	};

	void upgradeDamage(const float damageValue, const UpgradeType& bonusType);
	void upgradeAttackSpeed(const float AttackSpeedValue, const UpgradeType& bonusType);
	void upgradeRange(const float RangeValue, const UpgradeType& bonusType);
	void setRadiusColor(const sf::Color& color);
	void showRadius();

	static const sf::Color BASE_ATTACK_RADIUS_COLOR;
	static const sf::Color WRONG_PLACE_TOWER_RADIUS_COLOR;

	virtual void shoot() = 0;
	virtual void playAnimation(sf::Time deltaTime) = 0;
protected:
	sf::CircleShape radius;
	int price;
	float damage;
	float attackSpeed;
	float attackRange;
	bool isActive;
	std::list<std::unique_ptr<Projectile>> projectiles;
	float timeBetweenShots = 0;
	static constexpr int BASE_BALLISTA_MAX_FRAMES = 5;
	static constexpr int BASE_BOMBER_MAX_FRAMES = 5;
	static constexpr int BASE_WIZZARD_MAX_FRAMES = 5;
};

