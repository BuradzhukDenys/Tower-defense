#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Entity.h"
#include "Projectile.h"
#include "InterfaceContainer.h"
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

	sf::Angle getRotateAngleToEnemy() const;
	void shoot();
	void Attack(const std::list<std::unique_ptr<Enemy>>& enemies);
	bool intersects(const sf::FloatRect& rect) const;
	bool intersects(const Tower& other) const;
	bool intersects(const sf::Vector2f& point) const;
	bool inRadius(const sf::Vector2f& point) const;

	std::list<std::unique_ptr<Projectile>>& getProjectiles();
	float getDamage() const;
	int getPrice() const;
	TowerType getType() const;
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

	static void initializeTowersStats();
private:
	struct TowerStats
	{
		int price;
		float damage;
		float attackSpeed;
		float attackRange;
		bool canRotate;
		float projectileSpeed;
		float projectileDuration;
	};
	int price = 0;
	float damage = 0.f;
	float attackSpeed = 0.f;
	float attackRange = 0.f;
	float animationSpeed = 0.f;
	float timeBetweenShots = 0.f;
	bool isActive = false;
	bool canRotate = true;
	float projectileSpeed;
	float projectileDuration;
	sf::Angle rotateAngleToEnemy;

	TowerType towerType;
	sf::CircleShape radius;
	std::list<std::unique_ptr<Projectile>> projectiles;
	static std::map<TowerType, TowerStats> towerStatsMap;
	static constexpr float BOMBER_AOE_RADIUS = 90.f;
	static constexpr int WIZZARD_PIERCING_NUMBER = 4;

	Enemy* getFrontEnemy(const std::list<std::unique_ptr<Enemy>>& enemies);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

