#pragma once
#include "Enemy.h"
#include "Interface.h"
#include "GameState.h"
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
	static int currentWaveIndex;//Індекс поточної хвилі
	static int currentEnemyTypeIndex;//Кількість типів ворогів в шаблоні
	static int spawnedEnemiesOfType;//Кількість створених ворогів за типом
	static float timeUntilNextSpawn;//Час до наступного створення ворога
	static std::array<std::vector<waveConfig>, Interface::MAX_WAVES> wavespattern;//Шаблон хвиль з ворогами
	static const std::vector<WavesManager::waveConfig>* currentWavePattern;//Вектор теперішньої хвилі

	static bool spawnEnemy;
	static bool waveActive;
};

