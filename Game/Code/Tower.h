#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Entity.h"
#include "Projectile.h"
#include "InterfaceContainer.h"
#include "Enemy.h"
#include <map>

namespace TowersFrames
{
	static constexpr int BALLISTA_MAX_FRAMES = 5;
	static constexpr int BOMBER_MAX_FRAMES = 5;
	static constexpr int WIZZARD_MAX_FRAMES = 5;
}

class Tower : public Entity
{
public:
	enum class TowerType
	{
		Ballista,
		Bomber,
		Wizzard
	};

	enum class UpgradeType
	{
		multiplier,
		additive
	};

	Tower(const TowerType type, Resources::Texture textureID, const sf::Vector2f& position, const int framesCount = 1);

	void followTheEnemyAndShoot(const std::list<std::unique_ptr<Enemy>>& enemies);
	bool intersects(const sf::FloatRect& rect) const;
	bool intersects(const Tower& other) const;
	bool intersects(const sf::Vector2f& point) const;
	bool inRadius(const sf::Vector2f& point) const;

	float getDamage() const;
	int getPrice() const;
	static int getPrice(TowerType type);
	void upgradeDamage(const float damageValue, const UpgradeType& bonusType);
	void upgradeAttackSpeed(const float AttackSpeedValue, const UpgradeType& bonusType);
	void upgradeRange(const float RangeValue, const UpgradeType& bonusType);
	void setRadiusColor(const sf::Color& color);
	void showRadius();

	static const sf::Color BASE_ATTACK_RADIUS_COLOR;
	static const sf::Color WRONG_PLACE_TOWER_RADIUS_COLOR;
	
	virtual void Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies) override;
	virtual void playAnimation(sf::Time deltaTime) override;
	void shoot();

	static void initializeTowersStats();
private:
	struct TowerStats
	{
		int price;
		float damage;
		float attackSpeed;
		float attackRange;
	};
	int price = 0;
	float damage = 0;
	float attackSpeed = 0;
	float attackRange = 0;
	float animationSpeed = 0;
	float timeBetweenShots = 0;
	bool isActive = false;

	sf::CircleShape radius;
	std::list<std::unique_ptr<Projectile>> projectiles;
	static std::map<TowerType, TowerStats> towerStatsMap;
	static constexpr float BALLISTA_PROJECTILE_SPEED = 300.f;
	static constexpr float BALLISTA_PROJECTILE_DURATION = 5.f;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

