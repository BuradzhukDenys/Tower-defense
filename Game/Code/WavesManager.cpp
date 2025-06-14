#include "WavesManager.h"

std::array<std::vector<WavesManager::waveConfig>, Interface::MAX_WAVES> wavesPattern;

int WavesManager::currentWaveIndex = -1;
int WavesManager::currentEnemyTypeIndex = 0;
int WavesManager::spawnedEnemiesOfType = 0;
float WavesManager::timeUntilNextSpawn = 0.f;
bool WavesManager::waveActive = false;
bool WavesManager::spawnEnemy = false;
const std::vector<WavesManager::waveConfig>* WavesManager::currentWavePattern = nullptr;

void WavesManager::startWave()
{
	currentWaveIndex = Interface::getCurrentWave() - 1;
	if (currentWaveIndex >= 0 && currentWaveIndex < Interface::MAX_WAVES)
	{
		currentWavePattern = &wavesPattern[currentWaveIndex];
		currentEnemyTypeIndex = 0;
		spawnedEnemiesOfType = 0;
		timeUntilNextSpawn = 0.f;
		waveActive = true;
		spawnEnemy = true;
	}
}

void WavesManager::Update(sf::Time deltaTime, std::list<std::unique_ptr<Enemy>>& enemies, Map& map)
{
	if (!waveActive || !currentWavePattern || currentWaveIndex < 0)
	{
		return;
	}

	if (currentEnemyTypeIndex >= currentWavePattern->size())
	{
		spawnEnemy = false;
		if (enemies.empty())
		{
			waveActive = false;
			GameState::setState(GameState::State::Game);
			GameState::setStateBeforePause(GameState::State::Game);
			Interface::addMoney(Interface::getMoneyAfterWave());
			Interface::nextWave();

			if (Interface::getCurrentWave() == Interface::MAX_WAVES && GameState::getState() == GameState::State::Game)
			{
				GameState::setState(GameState::State::Win);
				return;
			}

			return;
		}
	}

	timeUntilNextSpawn -= deltaTime.asSeconds();
	if (spawnEnemy && timeUntilNextSpawn <= 0.f)
	{
		const auto& config = (*currentWavePattern)[currentEnemyTypeIndex];

		enemies.emplace_back(std::make_unique<Enemy>(
			config.enemyType,
			config.enemyTexture,
			map.getStartMap(),
			config.enemyFrames
		));

		spawnedEnemiesOfType++;
		timeUntilNextSpawn = config.timeBetweenSpawn;

		if (spawnedEnemiesOfType >= config.numberEnemies)
		{
			currentEnemyTypeIndex++;
			spawnedEnemiesOfType = 0;
		}
	}
}

void WavesManager::initializeWavesPattern()
{
	wavesPattern[0] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 15, 1.f, EnemiesFrames::GOBLIN_MAX_FRAMES}
	};
	wavesPattern[1] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 23, 0.65f, EnemiesFrames::GOBLIN_MAX_FRAMES}
	};
	wavesPattern[2] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 25, 0.65f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 4, 1.1f, EnemiesFrames::ORC_MAX_FRAMES}
	};
	wavesPattern[3] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 25, 0.65f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 6, 0.95f, EnemiesFrames::ORC_MAX_FRAMES}
	};
	wavesPattern[4] = {
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 2, 0.8f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 28, 0.65f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 3, 0.8f, EnemiesFrames::ORC_MAX_FRAMES}
	};
	wavesPattern[5] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 5, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 3, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 10, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 6, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 15, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES }
	};
	wavesPattern[6] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 5, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 3, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 10, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 6, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 15, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES },
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 3, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES}
	};
	wavesPattern[7] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 5, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 3, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 10, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 8, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 15, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES },
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 5, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES}
	};
	wavesPattern[8] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 8, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 3, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 12, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 8, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 15, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES },
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 5, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES}
	};
	wavesPattern[9] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 15, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 5, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 15, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 10, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 15, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES },
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 5, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES}
	};
	wavesPattern[10] = {
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 10, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 18, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 8, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 18, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES },
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 5, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES}
	};
	wavesPattern[11] = {
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 25, 0.9f, EnemiesFrames::ORC_MAX_FRAMES}
	};
	wavesPattern[12] = {
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 10, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 20, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 15, 0.75f, EnemiesFrames::ORC_MAX_FRAMES}
	};
	wavesPattern[13] = {
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 10, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 25, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 5, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 10, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 10, 0.75f, EnemiesFrames::ORC_MAX_FRAMES}
	};
	wavesPattern[14] = {
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 25, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES}
	};
	wavesPattern[15] = {
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 10, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 20, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 10, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 10, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 10, 0.75f, EnemiesFrames::ORC_MAX_FRAMES}
	};
	wavesPattern[16] = {
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 8, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 10, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 15, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 10, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES}
	};
	wavesPattern[17] = {
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 10, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 13, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 8, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 7, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES}
	};
	wavesPattern[18] = {
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 4, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 10, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 4, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 10, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 8, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES}
	};
	wavesPattern[19] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 55, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES}
	};
	wavesPattern[20] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 15, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 5, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 15, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 5, 0.75f, EnemiesFrames::ORC_MAX_FRAMES },
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 10, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
	};
	wavesPattern[21] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 25, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 18, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 15, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
	};
	wavesPattern[22] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 5, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 5, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 5, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 5, 0.75f, EnemiesFrames::ORC_MAX_FRAMES },
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 10, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 5, 0.75f, EnemiesFrames::ORC_MAX_FRAMES },
	};
	wavesPattern[23] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 30, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 30, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 30, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 15, 0.75f, EnemiesFrames::ORC_MAX_FRAMES}
	};
	wavesPattern[24] = {
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 15, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 10, 0.75f, EnemiesFrames::ORC_MAX_FRAMES},
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 15, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 10, 0.75f, EnemiesFrames::ORC_MAX_FRAMES },
		{ Enemy::EnemyType::Wolf, Resources::Texture::Wolf, 20, 0.8f, EnemiesFrames::WOLF_MAX_FRAMES},
		{ Enemy::EnemyType::Orc, Resources::Texture::Orc, 15, 0.75f, EnemiesFrames::ORC_MAX_FRAMES },
		{ Enemy::EnemyType::Goblin, Resources::Texture::Goblin, 10, 0.60f, EnemiesFrames::GOBLIN_MAX_FRAMES}
	};
}