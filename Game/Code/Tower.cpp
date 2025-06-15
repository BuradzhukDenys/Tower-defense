#include "Tower.h"
#include <fstream>
#include <sstream>
#include "AOEProjectile.h"
#include "StandartProjectile.h"
#include "piercingProjectile.h"

const sf::Color Tower::BASE_ATTACK_RADIUS_COLOR = sf::Color(90, 90, 90, 65);
const sf::Color Tower::WRONG_PLACE_TOWER_RADIUS_COLOR = sf::Color(255, 0, 0, 65);
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
		canRotate = stats.canRotate;
		projectileSpeed = stats.projectileSpeed;
		projectileDuration = stats.projectileDuration;
		animationSpeed = (1.f / attackSpeed) / (framesCount + 1);
	}
	else
		throw std::runtime_error("No stats found for this tower type");

	sprite.setOrigin(sf::Vector2f(sprite.getGlobalBounds().getCenter()));
	sprite.setPosition(position);

	radius.setRadius(attackRange);
	radius.setOrigin(radius.getGeometricCenter());
	radius.setPosition(sprite.getPosition());
	radius.setFillColor(BASE_ATTACK_RADIUS_COLOR);
}

void Tower::changeTower(TowerType type)
{
	//Міняємо характеристики, відповідно до типу
	auto it = towerStatsMap.find(type);
	if (it != towerStatsMap.end())
	{
		const TowerStats& stats = it->second;
		price = stats.price;
		damage = stats.damage;
		attackSpeed = stats.attackSpeed;
		attackRange = stats.attackRange;
		canRotate = stats.canRotate;
		projectileSpeed = stats.projectileSpeed;
		projectileDuration = stats.projectileDuration;
		animationSpeed = (1.f / attackSpeed) / (framesCount + 1);
	}
	else
		throw std::runtime_error("No stats found for this tower type");

	//В залежності від типу міняємо текстуру, та кількість кадрів, міняємо радіус та його центр
	this->towerType = type;
	switch (this->towerType)
	{
	case Tower::TowerType::Ballista:
		this->sprite.setTexture(Resources::textures.Get(Resources::Texture::Ballista));
		this->framesCount = TowersFrames::BALLISTA_MAX_FRAMES;
		break;
	case Tower::TowerType::Bomber:
		this->sprite.setTexture(Resources::textures.Get(Resources::Texture::Bomber));
		this->framesCount = TowersFrames::BOMBER_MAX_FRAMES;
		break;
	case Tower::TowerType::Wizzard:
		this->sprite.setTexture(Resources::textures.Get(Resources::Texture::Wizzard));
		this->framesCount = TowersFrames::WIZZARD_MAX_FRAMES;
		break;
	default:
		break;
	}
	this->radius.setRadius(attackRange);
	radius.setOrigin(radius.getGeometricCenter());
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

	Attack(enemies);
	if (isAnimationPlaying)
	{
		playAnimation(deltaTime);
	}

	//Якщо вежа пересікається з курсором, показуємо радіус
	isActive = intersects(mousePosition);
}

void Tower::playAnimation(sf::Time deltaTime)
{
	//Програємо анімацію перезарядки та стрільби
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
		switch (towerType)
		{
		case Tower::TowerType::Ballista:
			projectiles.emplace_back(std::make_unique<StandartProjectile>(
				Resources::Texture::BallistaShot,
				*this,
				projectileSpeed,
				projectileDuration
			));
			break;
		case Tower::TowerType::Bomber:
			projectiles.emplace_back(std::make_unique<AOEProjectile>(
				Resources::Texture::BomberShot,
				*this,
				projectileSpeed,
				projectileDuration,
				BOMBER_AOE_RADIUS
			));
			break;
		case Tower::TowerType::Wizzard:
			projectiles.emplace_back(std::make_unique<piercingProjectile>(
				Resources::Texture::WizzardShot,
				*this,
				projectileSpeed,
				projectileDuration,
				WIZZARD_PIERCING_NUMBER
			));
			break;
		default:
			break;
		}

		timeBetweenShots = 1.f / attackSpeed;
		isAnimationPlaying = true;
		timeForLastAnimationPlay = 0;
	}
}

void Tower::Attack(const std::list<std::unique_ptr<Enemy>>& enemies)
{
	Enemy* targetEnemy = getFrontEnemy(enemies);

	if (targetEnemy && inRadius(targetEnemy->getSprite().getGlobalBounds().getCenter()))
	{
		sf::Vector2f towerPosition(sprite.getPosition());
		sf::Vector2f distanceToEnemy = targetEnemy->getSprite().getGlobalBounds().getCenter() - towerPosition;
		//Визначаємо кут повроту до ворога через дистанцію між вежою та ворогом
		float angleInRadians = std::atan2(distanceToEnemy.y, distanceToEnemy.x);
		rotateAngleToEnemy = sf::radians(angleInRadians);

		if (canRotate)
		{
			sprite.setRotation(rotateAngleToEnemy);
		}

		shoot();
	}
}

bool Tower::intersects(const sf::FloatRect& rect) const
{
	sf::Vector2f center = sprite.getGlobalBounds().getCenter();
	float radius = sprite.getGlobalBounds().size.x / 2.f;

	//Визначаэмо найближчу точку прямокутника до вежі
	float nearestX = std::max(rect.position.x, std::min(center.x, rect.position.x + rect.size.x));
	float nearestY = std::max(rect.position.y, std::min(center.y, rect.position.y + rect.size.y));

	//Дистанція між вежою та точкою з прямокутника
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

	//Дистанція між вежами
	float dx = centerA.x - centerB.x;
	float dy = centerA.y - centerB.y;
	float distance = std::sqrt(dx * dx + dy * dy);

	return distance < (radiusA + radiusB);
}

bool Tower::intersects(const sf::Vector2f& point) const
{
	sf::Vector2f center = sprite.getGlobalBounds().getCenter();
	float radius = sprite.getGlobalBounds().size.x / 2.f;

	//Дистанція між точкою і вежою
	float dx = point.x - center.x;
	float dy = point.y - center.y;
	float distance = std::sqrt(dx * dx + dy * dy);

	return distance < radius;
}

bool Tower::inRadius(const sf::Vector2f& point) const
{
	return radius.getGlobalBounds().contains(point);
}

float Tower::getDamage() const
{
	return damage;
}

Tower::TowerType Tower::getType() const
{
	return towerType;
}

int Tower::getPrice() const
{
	return price;
}

int Tower::getPrice(TowerType type)
{
	return towerStatsMap[type].price;
}

void Tower::setRadiusColor(const sf::Color& color)
{
	radius.setFillColor(color);
}

void Tower::showRadius()
{
	isActive = true;
	radius.setFillColor(BASE_ATTACK_RADIUS_COLOR);
	radius.setPosition(sprite.getPosition());
}

sf::Angle Tower::getRotateAngleToEnemy() const
{
	return rotateAngleToEnemy;
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

		//Пропускаємо коментарі, які починаються з '#', пусті рядки
		if (line.empty() || line[0] == '#') continue;
		if (!towerName.empty() && towerName.back() == ':')
			towerName.pop_back();

		int price;
		float damage, attackSpeed, attackRange, projectileSpeed, projectileDuration;
		bool canRotate;
		lineStream >> price >> damage >> attackSpeed >> attackRange >> canRotate >> projectileSpeed >> projectileDuration;

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

		towerStatsMap[type] = { price, damage, attackSpeed, attackRange, canRotate, projectileSpeed, projectileDuration };
	}

	towersStatsFile.close();
}

Enemy* Tower::getFrontEnemy(const std::list<std::unique_ptr<Enemy>>& enemies)
{
	Enemy* frontEnemy = nullptr;

	for (const auto& enemy : enemies)
	{
		if (!enemy) continue;
		if (inRadius(enemy->getSprite().getGlobalBounds().getCenter()))
		{
			if (!frontEnemy)
			{
				frontEnemy = enemy.get();
			}
			else
			{
				//Визначаємо позицію ворога в контейнері ворогів, та ворога, який перший зайшов в радіус
				const sf::Vector2f& enemyPos = enemy->getPosition();
				const sf::Vector2f& frontEnemyPos = frontEnemy->getPosition();

				//В залежності від напрямку ворога, перевіряємо відповідні координати ворога, та першого ворга
				if (frontEnemy->getDirection() == Enemy::Direction::Right && enemyPos.x > frontEnemyPos.x)
				{
					frontEnemy = enemy.get();
				}
				else if (frontEnemy->getDirection() == Enemy::Direction::Up && enemyPos.y < frontEnemyPos.y)
				{
					frontEnemy = enemy.get();
				}
				else if (frontEnemy->getDirection() == Enemy::Direction::Down && enemyPos.y > frontEnemyPos.y)
				{
					frontEnemy = enemy.get();
				}
			}
		}
	}

	return frontEnemy;
}

void Tower::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Перевизначений віртуальний метод, для малювання вежі, снарядів та радіуса
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