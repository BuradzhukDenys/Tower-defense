#include "Game.h"
#include "InterfaceContainer.h"
#include <iostream>
#include "AOEProjectile.h"

Game::Game(const sf::State& windowState)
	: windowTitle("Tower defense"),
	windowSize(sf::VideoMode::getDesktopMode()),
	window(windowSize, windowTitle, windowState),
	map(windowSize.size),
	currentWaveText(Resources::fonts.Get(Resources::Font::BasicFont)),
	moneyText(Resources::fonts.Get(Resources::Font::BasicFont)),
	livesText(Resources::fonts.Get(Resources::Font::BasicFont)),
	music(Resources::music.Get(Resources::Music::BasicMusic))
{
	initializeInterface();
	initializeGameInfo();
	window.setIcon(Resources::getIcon());
	music.setLooping(true);
	music.setVolume(25);
	music.play();
}

void Game::Run()
{
	sf::Clock deltaTime;

	while (window.isOpen())
	{
		Events();
		Update(deltaTime.restart());
		Render();
	}
}

void Game::checkSelectedTower()
{
	if (Interface::getSelectedTower() == Interface::TowerType::Ballista)
	{
		if (!pickableTower || pickableTower->getType() != Tower::TowerType::Ballista)
		{
			pickableTower = std::make_unique<Tower>(Tower::TowerType::Ballista, Resources::Texture::Ballista, mousePosition, TowersFrames::BALLISTA_MAX_FRAMES);
		}
	}
	else if (Interface::getSelectedTower() == Interface::TowerType::Bomber)
	{
		if (!pickableTower || pickableTower->getType() != Tower::TowerType::Bomber)
		{
			pickableTower = std::make_unique<Tower>(Tower::TowerType::Bomber, Resources::Texture::Bomber, mousePosition, TowersFrames::BOMBER_MAX_FRAMES);
		}
	}
	else if (Interface::getSelectedTower() == Interface::TowerType::Wizzard)
	{
		if (!pickableTower || pickableTower->getType() != Tower::TowerType::Wizzard)
		{
			pickableTower = std::make_unique<Tower>(Tower::TowerType::Wizzard, Resources::Texture::Wizzard, mousePosition, TowersFrames::WIZZARD_MAX_FRAMES);
		}
	}
	else
		pickableTower.reset();
}

void Game::updatePickableTower()
{
	checkSelectedTower();
	checkCanPlace();
	if (pickableTower)
	{
		pickableTower->showRadius();
		pickableTower->setPosition(mousePosition);

		if (!canPlaceTower)
		{
			pickableTower->setRadiusColor(Tower::WRONG_PLACE_TOWER_RADIUS_COLOR);
		}
	}
}

void Game::checkCanPlace()
{
	canPlaceTower = true;

	for (const auto& tower : towers)
	{
		if (tower && pickableTower && tower->intersects(*pickableTower))
		{
			canPlaceTower = false;
			break;
		}
	}

	for (const auto& interfaceContainer : interface)
	{
		if (auto* container = dynamic_cast<InterfaceContainer*>(interfaceContainer.second.get()))
		{
			if (pickableTower && container && pickableTower->intersects(container->getGUI().getGlobalBounds()))
			{
				canPlaceTower = false;
				break;
			}
		}
	}

	if (pickableTower && !map.canPlaceTower(*pickableTower))
	{
		canPlaceTower = false;
	}
}

void Game::placeTower()
{
	if (pickableTower && canPlaceTower)
	{
		if (Interface::getMoney() >= pickableTower->getPrice() &&
			pickableTower->getType() == Tower::TowerType::Ballista)
		{
			towers.emplace_back(std::make_unique<Tower>(Tower::TowerType::Ballista, Resources::Texture::Ballista, mousePosition, TowersFrames::BALLISTA_MAX_FRAMES));
			Interface::substractMoney(pickableTower->getPrice());
		}
		else if (Interface::getMoney() >= pickableTower->getPrice() &&
			pickableTower->getType() == Tower::TowerType::Bomber)
		{
			towers.emplace_back(std::make_unique<Tower>(Tower::TowerType::Bomber, Resources::Texture::Bomber, mousePosition, TowersFrames::BOMBER_MAX_FRAMES));
			Interface::substractMoney(pickableTower->getPrice());
		}
		else if (Interface::getMoney() >= pickableTower->getPrice() &&
			pickableTower->getType() == Tower::TowerType::Wizzard)
		{
			towers.emplace_back(std::make_unique<Tower>(Tower::TowerType::Wizzard, Resources::Texture::Wizzard, mousePosition, TowersFrames::WIZZARD_MAX_FRAMES));
			Interface::substractMoney(pickableTower->getPrice());
		}
		else
		{
			moneyText.setFillColor(sf::Color::Red);
			timeToShowMoneyTextColor = 0.3;
			notEnoughMoney = true;
		}
	}
}

void Game::addInterfaceContainer(const Interface::InterfaceType& interfaceType, const sf::Vector2f& containerSize, const sf::Vector2f& containerPosition, const sf::Color& containerColor)
{
	interface[interfaceType] = std::make_unique<InterfaceContainer>(
		containerSize,
		containerPosition,
		containerColor
	);
}

InterfaceContainer& Game::getInterfaceContainer(const Interface::InterfaceType& interfaceType)
{
	auto it = interface.find(interfaceType);
	if (it != interface.end())
	{
		if (auto* container = dynamic_cast<InterfaceContainer*>(it->second.get()))
		{
			return *container;
		}
	}
	throw std::runtime_error("Not found interface container");
}

void Game::initializeInterface()
{
	addInterfaceContainer(Interface::InterfaceType::SelectTowerInterface,
		sf::Vector2f(300.f, windowSize.size.y),
		sf::Vector2f(windowSize.size.x - 300.f, 0.f),
		sf::Color(153, 76, 0)
	);

	auto& container = getInterfaceContainer(Interface::InterfaceType::SelectTowerInterface);
	float containerCenterX = (container.getSize().x / 2.f) + container.getPosition().x;

	container.addContainerText(std::string("Towers"), sf::Vector2f(containerCenterX, container.getPosition().y));
	container.addButtons(
		4,//Кількість кнопок
		std::vector<sf::Vector2f>{//Розміри кнопок
		sf::Vector2f(container.getSize().x * 0.7f, 150.f),
			sf::Vector2f(container.getSize().x * 0.7f, 150.f),
			sf::Vector2f(container.getSize().x * 0.7f, 150.f),
			sf::Vector2f(container.getSize().x * 0.7f, 165.f)
	},
		sf::Vector2f(containerCenterX, container.getPosition().y),//Позиція першої кнопки
		std::vector<sf::Color>{sf::Color::Blue, sf::Color::Blue, sf::Color::Blue, sf::Color(0, 130, 0)/*Зелений колір*/},//Кольори кнопок
		std::vector<std::string>{//Текст кнопок
		"Ballista\n" + std::to_string(Tower::getPrice(Tower::TowerType::Ballista)) + "$",
			"Bomber\n" + std::to_string(Tower::getPrice(Tower::TowerType::Bomber)) + "$",
			"Wizzard\n" + std::to_string(Tower::getPrice(Tower::TowerType::Wizzard)) + "$",
			"Play"
	},
		std::vector<Button::ButtonType>{//Типи кнопок
		Button::ButtonType::Ballista,
			Button::ButtonType::Bomber,
			Button::ButtonType::Wizzard,
			Button::ButtonType::Play
	}
	);
}

void Game::initializeGameInfo()
{
	sf::Vector2f textPosition;

	auto& container = getInterfaceContainer(Interface::InterfaceType::SelectTowerInterface);
	textPosition = container.getPosition();

	currentWaveText.setString("Wave: " + std::to_string(Interface::getCurrentWave()) + "/" + std::to_string(Interface::MAX_WAVES));
	currentWaveText.setCharacterSize(GAME_FONT_SIZE);
	currentWaveText.setPosition({ textPosition.x - currentWaveText.getGlobalBounds().size.x - MARGIN_BORDERS, MARGIN_ROWS });

	livesText.setString("Lives: " + std::to_string(Interface::getLives()));
	livesText.setCharacterSize(GAME_FONT_SIZE);
	livesText.setPosition({ textPosition.x - livesText.getGlobalBounds().size.x - MARGIN_BORDERS, currentWaveText.getPosition().y + currentWaveText.getGlobalBounds().size.y + MARGIN_ROWS });

	moneyText.setString("Money: " + std::to_string(Interface::getMoney()));
	moneyText.setCharacterSize(GAME_FONT_SIZE);
	moneyText.setPosition({ textPosition.x - moneyText.getGlobalBounds().size.x - MARGIN_BORDERS, livesText.getPosition().y + livesText.getGlobalBounds().size.y + MARGIN_ROWS });
}

void Game::updateGameInfo(sf::Time deltaTime)
{
	currentWaveText.setString("Wave: " + std::to_string(Interface::getCurrentWave()) + "/" + std::to_string(Interface::MAX_WAVES));
	livesText.setString("Lives: " + std::to_string(Interface::getLives()));

	moneyText.setString("Money: " + std::to_string(Interface::getMoney()));
	if (moneyText.getFillColor() != sf::Color::White)
	{
		timeToShowMoneyTextColor -= deltaTime.asSeconds();
		if (timeToShowMoneyTextColor <= 0)
		{
			moneyText.setFillColor(sf::Color::White);
		}
	}
}

void Game::checkVictory()
{
	if (GameState::getState() == GameState::State::Win && GameState::getState() != GameState::State::Loss)
	{
		addInterfaceContainer(Interface::InterfaceType::WinnerInterface,
			sf::Vector2f(windowSize.size),
			sf::Vector2f(0, 0),
			sf::Color(Interface::BASE_BACKGROUND_COLOR)
		);
		auto& container = getInterfaceContainer(Interface::InterfaceType::WinnerInterface);
		sf::Vector2f containerCenter = (container.getSize() / 2.f) + container.getPosition();

		container.addContainerText("You win", sf::Vector2f(containerCenter.x, containerCenter.y - 150.f), sf::Color::Green);
		container.addButtons(
			2,//Кількість кнопок
			std::vector<sf::Vector2f>{//Розміри кнопок
			sf::Vector2f(350.f, 80.f),
				sf::Vector2f(350.f, 80.f)
		},
			sf::Vector2f(containerCenter),//Позиція першої кнопки
			std::vector<sf::Color>{//Кольори кнопок
			sf::Color::Magenta,
				sf::Color::Magenta
		},
			std::vector<std::string>{//Тексти кнопок
			"Restart",
				"Exit"
		},
			std::vector<Button::ButtonType>{//Типи кнопок
			Button::ButtonType::Restart,
				Button::ButtonType::Exit
		}
		);
	}
}

void Game::checkLoss()
{
	if (Interface::getLives() == 0)
	{
		GameState::setState(GameState::State::Loss);
		addInterfaceContainer(Interface::InterfaceType::LoserInterface,
			sf::Vector2f(windowSize.size),
			sf::Vector2f(0, 0),
			sf::Color(Interface::BASE_BACKGROUND_COLOR)
		);
		auto& container = getInterfaceContainer(Interface::InterfaceType::LoserInterface);
		sf::Vector2f containerCenter = (container.getSize() / 2.f) + container.getPosition();

		container.addContainerText("You loss", sf::Vector2f(containerCenter.x, containerCenter.y - 150.f), sf::Color::Red);
		container.addButtons(
			2,//Кількість кнопок
			std::vector<sf::Vector2f>{//Розміри кнопок
			sf::Vector2f(350.f, 80.f),
				sf::Vector2f(350.f, 80.f)
		},
			sf::Vector2f(containerCenter),//Позиція першої кнопки
			std::vector<sf::Color>{//Кольори кнопок
			sf::Color::Magenta,
				sf::Color::Magenta
		},
			std::vector<std::string>{//Тексти кнопок
			"Restart",
				"Exit"
		},
			std::vector<Button::ButtonType>{//Типи кнопок
			Button::ButtonType::Restart,
				Button::ButtonType::Exit
		}
		);
	}
}

void Game::Pause()
{
	if (GameState::getState() == GameState::State::Pause)
	{
		addInterfaceContainer(Interface::InterfaceType::PauseInterface,
			sf::Vector2f(windowSize.size),
			sf::Vector2f(0, 0),
			sf::Color(Interface::BASE_BACKGROUND_COLOR)
		);

		auto& container = getInterfaceContainer(Interface::InterfaceType::PauseInterface);
		sf::Vector2f containerCenter = (container.getSize() / 2.f) + container.getPosition();

		container.addContainerText("Pause", sf::Vector2f(containerCenter.x, containerCenter.y - 220.f));
		container.addButtons(
			3,//Кількість кнопок
			std::vector<sf::Vector2f>{//Розміри кнопок
			sf::Vector2f(350.f, 80.f),
				sf::Vector2f(350.f, 80.f),
				sf::Vector2f(350.f, 80.f)
		},
			sf::Vector2f(containerCenter),//Позиція першої кнопки
			std::vector<sf::Color>{//Кольори кнопок
			sf::Color::Magenta,
				sf::Color::Magenta,
				sf::Color::Magenta
		},
			std::vector<std::string>{//Тексти кнопок
			"Resume",
				"Restart",
				"Exit"
		},
			std::vector<Button::ButtonType>{//Типи кнопок
			Button::ButtonType::Resume,
				Button::ButtonType::Restart,
				Button::ButtonType::Exit
		}
		);
	}
}

void Game::playSounds()
{
	std::unique_ptr<sf::Sound> soundPtr = nullptr;
	for (const auto& enemy : enemies)
	{
		if (!enemy->isAlive())
		{
			switch (enemy->getType())
			{
			case Enemy::EnemyType::Goblin:
				soundPtr = std::make_unique<sf::Sound>(Resources::sounds.Get(Resources::Sound::GoblinDeath));
				soundPtr->setVolume(40);
				break;
			case Enemy::EnemyType::Orc:
				soundPtr = std::make_unique<sf::Sound>(Resources::sounds.Get(Resources::Sound::OrcDeath));
				soundPtr->setVolume(35);
				break;
			case Enemy::EnemyType::Wolf:
				soundPtr = std::make_unique<sf::Sound>(Resources::sounds.Get(Resources::Sound::WolfDeath));
				soundPtr->setVolume(15);
				break;
			default:
				break;
			}
		}
	}

	if (towerSelled)
	{
		soundPtr = std::make_unique<sf::Sound>(Resources::sounds.Get(Resources::Sound::SellTower));
		towerSelled = false;
	}

	if (notEnoughMoney)
	{
		soundPtr = std::make_unique<sf::Sound>(Resources::sounds.Get(Resources::Sound::NotEnoughMoney));
		notEnoughMoney = false;
	}

	if (soundPtr)
	{
		soundPtr->play();
		sounds.emplace_back(std::move(soundPtr));
	}
}

void Game::showGameInfo()
{
	window.draw(currentWaveText);
	window.draw(moneyText);
	window.draw(livesText);
}

void Game::Events()
{
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}

		if (event->is<sf::Event::FocusLost>() && (GameState::getState() == GameState::State::Game ||
			GameState::getState() == GameState::State::WavePlay))
		{
			GameState::setState(GameState::State::Pause);
		}

		if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			if (keyReleased->scancode == sf::Keyboard::Scan::Escape &&
				(GameState::getState() == GameState::State::Game || GameState::getState() == GameState::State::WavePlay))
			{
				GameState::setState(GameState::State::Pause);
			}

			if (keyReleased->scancode == sf::Keyboard::Scancode::Space)
			{
				if (GameState::getState() == GameState::State::Game)
				{
					WavesManager::startWave();
					GameState::setState(GameState::State::WavePlay);
				}
			}
		}

		if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseReleased->button == sf::Mouse::Button::Left)
			{
				for (auto& interfaceComponent : interface) {

					if (auto* container = dynamic_cast<InterfaceContainer*>(interfaceComponent.second.get())) {
						if (container->contains(mousePosition))
						{
							canPlaceTower = false;
							container->handleClick(mousePosition);
						}
					}
				}
				placeTower();

				switch (GameState::getState())
				{
				case GameState::State::Game:
				case GameState::State::WavePlay:
					if (interface.find(Interface::InterfaceType::PauseInterface) != interface.end())
					{
						interface.erase(Interface::InterfaceType::PauseInterface);
					}
					break;
				case GameState::State::Restart:
					if (interface.find(Interface::InterfaceType::WinnerInterface) != interface.end() ||
						interface.find(Interface::InterfaceType::LoserInterface) != interface.end() ||
						interface.find(Interface::InterfaceType::PauseInterface) != interface.end())
					{
						interface.erase(Interface::InterfaceType::WinnerInterface);
						interface.erase(Interface::InterfaceType::LoserInterface);
						interface.erase(Interface::InterfaceType::PauseInterface);
					}
					Interface::reset();
					towers.clear();
					enemies.clear();
					GameState::setState(GameState::State::Game);
					music.stop();
					music.play();
					break;
				case GameState::State::Exit:
					window.close();
					break;
				default:
					break;
				}
			}
			else if (mouseReleased->button == sf::Mouse::Button::Right)
			{
				for (const auto& tower : towers)
				{
					if (tower->intersects(mousePosition) && !pickableTower)
					{
						Interface::addMoney(tower->getPrice() * 0.65f);
						moneyText.setFillColor(sf::Color::Green);
						timeToShowMoneyTextColor = 0.3;
						towers.remove(tower);
						towerSelled = true;
						break;
					}
				}

				Interface::setSelectedTower(Interface::TowerType::None);
			}
		}
	}
}

void Game::Update(sf::Time deltaTime)
{
	if (GameState::getState() != GameState::State::Pause)
	{
		for (const auto& enemy : enemies)
		{
			enemy->Update(deltaTime, mousePosition, enemies);
			map.updateTurnEnemy(*enemy);
			if (!enemy->isAlive())
			{
				Interface::addMoney(enemy->getMoneyReward());
				enemies.remove(enemy);
				break;
			}
			if (enemy->getPosition().x > map.getMapWidth())
			{
				Interface::lostlives();
				enemies.remove(enemy);
				break;
			}
		}

		updatePickableTower();
		for (auto& tower : towers)
		{
			tower->Update(deltaTime, mousePosition, enemies);
		}

		updateGameInfo(deltaTime);

		if (GameState::getState() == GameState::State::WavePlay)
		{
			WavesManager::Update(deltaTime, enemies, map);
		}
	}
	mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	checkVictory();
	checkLoss();
	Pause();
	for (auto& interfaceComponent : interface)
	{
		interfaceComponent.second->Update(deltaTime, mousePosition);
	}

	playSounds();
	sounds.remove_if(
		[](const std::unique_ptr<sf::Sound>& sound)
		{
			return sound->getStatus() != sf::Sound::Status::Playing;
		}
	);
}

void Game::Render()
{
	window.clear();

	window.draw(map);
	for (auto& enemy : enemies)
	{
		window.draw(*enemy);
	}
	for (auto& tower : towers)
	{
		window.draw(*tower);
	}

	showGameInfo();
	for (auto& interfaceComponent : interface)
	{
		window.draw(*interfaceComponent.second);
	}

	if (pickableTower)
	{
		window.draw(*pickableTower);
	}

	window.display();
}


