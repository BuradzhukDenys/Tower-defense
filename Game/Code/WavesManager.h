#pragma once
#include "Enemy.h"
#include "Interface.h"
#include "Map.h"

class WavesManager
{
public:
	WavesManager() = delete;
	static void initializeWavesPattern();
	static void startWave();
	static void Update(sf::Time deltaTime, std::list<std::unique_ptr<Enemy>>& enemies, Map& map);
	struct waveConfig
	{
		Enemy::EnemyType enemyType;
		Resources::Texture enemyTexture;
		int numberEnemies;
		float timeBetweenSpawn;
		int enemyFrames;
	};
private:
	static int currentWaveIndex;
	static int currentEnemyTypeIndex;
	static int spawnedEnemiesOfType;
	static float timeUntilNextSpawn;
	static std::array<std::vector<waveConfig>, Interface::MAX_WAVES> wavespattern;
	static const std::vector<WavesManager::waveConfig>* currentWavePattern;

	static bool spawnEnemy;
	static bool waveActive;
};

