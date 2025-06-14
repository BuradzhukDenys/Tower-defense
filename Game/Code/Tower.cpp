#include "Tower.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/System/Time.hpp>
#include <fstream>
#include <string>
#include <sstream>

const sf::Color Tower::BASE_ATTACK_RADIUS_COLOR = sf::Color(90, 90, 90, 40);
const sf::Color Tower::WRONG_PLACE_TOWER_RADIUS_COLOR = sf::Color(255, 0, 0, 40);
std::map<Tower::TowerType, Tower::TowerStats> Tower::towerStatsMap;

Tower::Tower(const TowerType type, Resources::Texture textureID, const sf::Vector2f& position, const int framesCount)
	: Entity(textureID, framesCount), towerType(type)
{
	auto it = towerStatsMap.find(type);
	if (it != towerStatsMap.end())
	{
		const TowerStats& stats = it->second;
		price = stats.price;
		damage = stats.damage;
		attackSpeed = stats.attackSpeed;
		attackRange = stats.attackRange;
		animationSpeed = (1.f / attackSpeed) / (framesCount + 1);
	}
	else
		throw std::runtime_error("No stats found for this tower type");

	sprite.setTextureRect(sf::IntRect({ 0, 0 }, frameSize));
	sprite.setOrigin(sf::Vector2f(sprite.getGlobalBounds().getCenter()));
	sprite.setPosition(position);

	radius.setRadius(attackRange);
	radius.setOrigin(radius.getGeometricCenter());
	radius.setPosition(sprite.getPosition());
	radius.setFillColor(BASE_ATTACK_RADIUS_COLOR);
}

void Tower::Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies)
{
	timeBetweenShots -= deltaTime.asSeconds();

	for (auto& shot : projectiles)
	{
		shot->Update(deltaTime, mousePosition, enemies);
	}

	projectiles.remove_if(
		[](const std::unique_ptr<Projectile>& projectile) {
			return !projectile->isAlive();
		}
	);

	followTheEnemyAndShoot(enemies);

	if (isAnimationPlaying)
	{
		playAnimation(deltaTime);
	}

	isActive = intersects(mousePosition);
}

void Tower::playAnimation(sf::Time deltaTime)
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

void Tower::shoot()
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
		timeForLastAnimationPlay = 0;
	}
}

//edit
void Tower::followTheEnemyAndShoot(const std::list<std::unique_ptr<Enemy>>& enemies)
{
	for (const auto& enemy : enemies)
	{
		if (!enemy) continue;
		if (inRadius(enemy->getPosition()))
		{
			sf::Vector2f towerPosition(sprite.getPosition());

			sf::Vector2f distanceToEnemy = enemy->getPosition() - towerPosition;
			float angleInRadians = std::atan2(distanceToEnemy.y, distanceToEnemy.x);
			sf::Angle angle(sf::radians(angleInRadians));
			sprite.setRotation(angle);

			shoot();
			break;
		}
	}
}

bool Tower::intersects(const sf::FloatRect& rect) const
{
	sf::Vector2f center = sprite.getGlobalBounds().getCenter();
	float radius = sprite.getGlobalBounds().size.x / 2.f;

	float nearestX = std::max(rect.position.x, std::min(center.x, rect.position.x + rect.size.x));
	float nearestY = std::max(rect.position.y, std::min(center.y, rect.position.y + rect.size.y));

	float dx = center.x - nearestX;
	float dy = center.y - nearestY;
	float distance = std::sqrt(dx * dx + dy * dy);

	return distance <= radius;
}

bool Tower::intersects(const Tower& other) const
{
	sf::Vector2f centerA = sprite.getGlobalBounds().getCenter();
	sf::Vector2f centerB = other.sprite.getGlobalBounds().getCenter();

	float radiusA = sprite.getGlobalBounds().size.x / 2.0f;
	float radiusB = other.sprite.getGlobalBounds().size.x / 2.0f;

	float dx = centerA.x - centerB.x;
	float dy = centerA.y - centerB.y;
	float distance = std::sqrt(dx * dx + dy * dy);

	return distance < (radiusA + radiusB);
}

bool Tower::intersects(const sf::Vector2f& point) const
{
	sf::Vector2f center = sprite.getGlobalBounds().getCenter();
	float radius = sprite.getGlobalBounds().size.x / 2.f;

	float dx = point.x - center.x;
	float dy = point.y - center.y;
	float distance = std::sqrt(dx * dx + dy * dy);
	if (distance <= radius)
	{
		return sprite.getGlobalBounds().contains(point);
	}

	return false;
}

bool Tower::inRadius(const sf::Vector2f& point) const
{
	return radius.getGlobalBounds().contains(point);
}

float Tower::getDamage() const
{
	return damage;
}

int Tower::getPrice() const
{
	return price;
}

int Tower::getPrice(TowerType type)
{
	return towerStatsMap[type].price;
}

void Tower::upgradeDamage(const float damageValue, const UpgradeType& bonusType)
{
	switch (bonusType)
	{
	case UpgradeType::multiplier:
		this->damage *= damageValue;
		break;
	case UpgradeType::additive:
		this->damage += damageValue;
		break;
	default:
		break;
	}
}

void Tower::upgradeAttackSpeed(const float AttackSpeedValue, const UpgradeType& bonusType)
{
	switch (bonusType)
	{
	case UpgradeType::multiplier:
		this->attackSpeed *= AttackSpeedValue;
		break;
	case UpgradeType::additive:
		this->attackSpeed += AttackSpeedValue;
		break;
	default:
		break;
	}
}

void Tower::upgradeRange(const float RangeValue, const UpgradeType& bonusType)
{
	switch (bonusType)
	{
	case UpgradeType::multiplier:
		this->attackRange *= RangeValue;
		break;
	case UpgradeType::additive:
		this->attackRange += RangeValue;
		break;
	default:
		break;
	}
}

void Tower::setRadiusColor(const sf::Color& color)
{
	radius.setFillColor(color);
}

void Tower::showRadius()
{
	isActive = true;
	radius.setFillColor(BASE_ATTACK_RADIUS_COLOR);
	radius.setPosition(this->getPosition());
}

void Tower::initializeTowersStats()
{
	std::ifstream towersStatsFile("Stats/Towers.txt");

	if (!towersStatsFile.is_open())
	{
		throw std::runtime_error("Couldn't open file.");
	}

	std::string line;
	while (std::getline(towersStatsFile, line))
	{
		std::istringstream lineStream(line);
		std::string towerName;
		lineStream >> towerName;

		if (line.empty() || line[0] == '#') continue;
		if (!towerName.empty() && towerName.back() == ':')
			towerName.pop_back();

		int price;
		float damage, attackSpeed, attackRange;
		lineStream >> price >> damage >> attackSpeed >> attackRange;

		TowerType type;
		if (towerName == "Ballista")
		{
			type = TowerType::Ballista;
		}
		else if (towerName == "Bomber")
		{
			type = TowerType::Bomber;
		}
		else if (towerName == "Wizzard")
		{
			type = TowerType::Wizzard;
		}
		else continue;

		towerStatsMap[type] = { price, damage, attackSpeed, attackRange };
	}

	towersStatsFile.close();
}

void Tower::draw(sf::RenderTarget& target, sf::RenderStates states) const
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