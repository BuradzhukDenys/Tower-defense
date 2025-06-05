#include "Tower.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/System/Time.hpp>

const sf::Color Tower::BASE_ATTACK_RADIUS_COLOR = sf::Color(90, 90, 90, 40);
const sf::Color Tower::WRONG_PLACE_TOWER_RADIUS_COLOR = sf::Color(255, 0, 0, 40);

Tower::Tower(Resources::Texture textureID, const sf::Vector2f& position, const int price,
	const float damage, const float attackSpeed, const float attackRange, const int framesCount)
	: Entity(textureID, framesCount), price(price), damage(damage), attackSpeed(attackSpeed),
	attackRange(attackRange), radius(attackRange), isActive(false)
{
	radius.setFillColor(BASE_ATTACK_RADIUS_COLOR);
}

void Tower::followTheEnemy(const Entity& enemy)
{
	if (inRadius(enemy.getPosition()))
	{
		sf::Vector2f towerPosition(sprite.getPosition());

		sf::Vector2f distanceToEnemy = enemy.getPosition() - towerPosition;
		float angleInRadians = std::atan2(distanceToEnemy.y, distanceToEnemy.x);
		sf::Angle angle(sf::radians(angleInRadians));
		sprite.setRotation(angle);
	}
}

void Tower::followTheEnemy(const Entity& enemy, const sf::RenderWindow& window)
{
	sf::Vector2f mousePosition(sf::Mouse::getPosition(window));
	sf::Vector2f towerPosition(sprite.getPosition());

	sf::Vector2f distanceToCursor = mousePosition - towerPosition;
	float angleInRadians = std::atan2(distanceToCursor.y, distanceToCursor.x);
	sf::Angle angle(sf::radians(angleInRadians));
	if (inRadius(mousePosition))
	{
		sprite.setRotation(angle);
	}
}

bool Tower::intersects(const sf::FloatRect& rect) const
{
	sf::Vector2f center = sprite.getPosition();
	float radius = sprite.getTextureRect().size.x / 2.f;

	float nearestX = std::max(rect.position.x, std::min(center.x, rect.position.x + rect.size.x));
	float nearestY = std::max(rect.position.y, std::min(center.y, rect.position.y + rect.size.y));

	float dx = center.x - nearestX;
	float dy = center.y - nearestY;
	float distance = std::sqrt(dx * dx + dy * dy);

	return distance <= radius;
}

bool Tower::intersects(const Tower& other) const
{
	sf::Vector2f centerA = sprite.getPosition();
	sf::Vector2f centerB = other.sprite.getPosition();

	float radiusA = sprite.getTextureRect().size.x / 2.0f;
	float radiusB = other.sprite.getTextureRect().size.x / 2.0f;

	float dx = centerA.x - centerB.x;
	float dy = centerA.y - centerB.y;
	float distance = std::sqrt(dx * dx + dy * dy);

	return distance < (radiusA + radiusB);
}

bool Tower::intersects(const sf::Vector2f& point) const
{
	sf::Vector2f center = sprite.getPosition();
	float radius = sprite.getTextureRect().size.x / 2.f;

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
