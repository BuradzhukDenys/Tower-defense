#include "Enemy.h"
#include <sstream>
#include <fstream>

std::map<Enemy::EnemyType, Enemy::EnemyStats> Enemy::enemiesStatsMap;

Enemy::Enemy(const EnemyType type, Resources::Texture textureID, const sf::Vector2f& position, const int framesCount)
	: Entity(textureID, framesCount),
	direction(Enemy::Direction::Right), healthBar({ 30.f, 4.f }),
	healthBarBackground({ 30.f, 4.f })
{
	auto it = enemiesStatsMap.find(type);
	if (it != enemiesStatsMap.end())
	{
		const EnemyStats& stats = it->second;
		moneyReward = stats.moneyReward;
		healthPoints = stats.healthPoints;
		moveSpeed = stats.moveSpeed;
		maxHealthPoints = stats.healthPoints;
	}
	else
		throw std::runtime_error("No stats found for this enemy type");

	sprite.setOrigin({ sprite.getLocalBounds().position.x, sprite.getLocalBounds().getCenter().y });
	sprite.setPosition({ position.x - sprite.getLocalBounds().size.x, position.y});
	healthBarBackground.setOrigin({ healthBarBackground.getLocalBounds().getCenter().x, 0 });
	healthBarBackground.setPosition({ sprite.getPosition().x + sprite.getLocalBounds().getCenter().x, sprite.getPosition().y + sprite.getOrigin().y + healthBar.getSize().y });
	healthBarBackground.setFillColor(sf::Color::Red);
	healthBarBackground.setOutlineColor(sf::Color::Black);
	healthBarBackground.setOutlineThickness(2.f);
	healthBar.setOrigin(healthBarBackground.getOrigin());
	healthBar.setPosition(healthBarBackground.getPosition());
	healthBar.setFillColor(sf::Color::Green);
}

const Enemy::Direction& Enemy::getDirection() const
{
	return direction;
}

void Enemy::setDirection(const Direction& direction)
{
	this->direction = direction;
}

int Enemy::getMoneyReward() const
{
	return moneyReward;
}

void Enemy::takeDamage(const float damage)
{
	healthPoints -= damage;
	updateHealthBar();

	if (healthPoints <= 0)
	{
		enemyIsAlive = false;
	}
}

bool Enemy::isAlive() const
{
	return enemyIsAlive;
}

float Enemy::getHealth() const
{
	return healthPoints;
}

void Enemy::updateHealthBar()
{
	float healthPercent = static_cast<float>(healthPoints) / maxHealthPoints;
	healthBar.setSize(sf::Vector2f(healthBarBackground.getSize().x * healthPercent, 4.f));
}

void Enemy::playAnimation(sf::Time deltaTime)
{
	if (distanceForLastFrame >= distancePerFrame)
	{
		distanceForLastFrame = 0.f;
		currentFrame = (currentFrame + 1) % framesCount;
		sprite.setTextureRect(sf::IntRect(
			{ frameSize.x * currentFrame, 0 },
			{ frameSize.x, frameSize.y }
		));
	}
}

void Enemy::Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies)
{
	sf::Vector2f moveOffset;
	switch (direction)
	{
	case Enemy::Direction::Up:
		moveOffset.y = -moveSpeed * deltaTime.asSeconds();
		break;
	case Enemy::Direction::Down:
		moveOffset.y = moveSpeed * deltaTime.asSeconds();
		break;
	case Enemy::Direction::Right:
		moveOffset.x = moveSpeed * deltaTime.asSeconds();
		break;
	default:
		break;
	}

	sprite.move(moveOffset);
	healthBar.move(moveOffset);
	healthBarBackground.move(moveOffset);

	distanceForLastFrame += std::sqrt(moveOffset.x * moveOffset.x + moveOffset.y * moveOffset.y);
	playAnimation(deltaTime);
}

void Enemy::initializeEnemiesStats()
{
	std::ifstream enemiesStatsFile("Stats/Enemies.txt");

	if (!enemiesStatsFile.is_open())
	{
		throw std::runtime_error("Couldn't open file.");
	}

	std::string line;
	while (std::getline(enemiesStatsFile, line))
	{
		std::istringstream lineStream(line);
		std::string EnemyName;
		lineStream >> EnemyName;

		if (line.empty() || line[0] == '#') continue;
		if (!EnemyName.empty() && EnemyName.back() == ':')
			EnemyName.pop_back();

		int moneyReward;
		float healthPoints, moveSpeed;
		lineStream >> moneyReward >> healthPoints >> moveSpeed;

		EnemyType type;
		if (EnemyName == "Goblin")
		{
			type = EnemyType::Goblin;
		}
		else if (EnemyName == "Orc")
		{
			type = EnemyType::Orc;
		}
		else if (EnemyName == "Wolf")
		{
			type = EnemyType::Wolf;
		}
		else continue;

		enemiesStatsMap[type] = { moneyReward, healthPoints, moveSpeed};
	}

	enemiesStatsFile.close();
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
	target.draw(healthBarBackground, states);
	target.draw(healthBar, states);
}
