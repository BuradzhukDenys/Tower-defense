#include "Game.h"
#include <iostream>

Game::Game(const sf::State& windowState)
	: windowTitle("Tower defense"),
	windowSize(sf::Vector2u(1920, 1080)),
	window(windowSize, windowTitle, windowState),
	map(windowSize.size),
	currentWaveText(Resources::fonts.Get(Resources::Font::BasicFont)),
	moneyText(Resources::fonts.Get(Resources::Font::BasicFont)),
	livesText(Resources::fonts.Get(Resources::Font::BasicFont)),
	fps(Resources::fonts.Get(Resources::Font::BasicFont)),
	control(Resources::fonts.Get(Resources::Font::BasicFont)),
	music(Resources::music.Get(Resources::Music::BasicMusic))
{
	initializeInterface();
	initializeGameInfo();
	window.setFramerateLimit(360);
	window.setIcon(Resources::getIcon());
	music.setLooping(true);
	music.setVolume(25);
	music.play();
	control.setString(L"Escape - Пауза\nSpace - Запустити хвилю\nПКМ - скинути вибрану вежу\nПКМ(по вежі) - продати вежу");
	fps.setString("FPS:");
	fps.setPosition(sf::Vector2f(25.f, windowSize.size.y - fps.getGlobalBounds().size.y - 25.f));
	control.setPosition(sf::Vector2f(25.f, windowSize.size.y - control.getGlobalBounds().size.y - 28.f - fps.getGlobalBounds().size.y));
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
	//Створюємо статичну змінну з останнім вибраним тоипом вежі
	static Interface::SelectedTowerType lastSelectedTower = Interface::SelectedTowerType::None;
	auto currentSelectedTower = Interface::getSelectedTower();

	//Якщо вибраний тип вежі та останній вибраний тип вежі відрізняються міняємо вежу
	if (currentSelectedTower != lastSelectedTower)
	{
		lastSelectedTower = currentSelectedTower;
		//Якщо вказівник на вежу - nullptr, створюємо данні в памяті, якщо ж вказівник вже реалізований,
		//Просто міняємо всі характеристики вежі
		switch (currentSelectedTower)
		{
		case Interface::SelectedTowerType::Ballista:
			if (!pickableTower)
				pickableTower = std::make_unique<Tower>(Tower::TowerType::Ballista, Resources::Texture::Ballista, mousePosition, TowersFrames::BALLISTA_MAX_FRAMES);
			else
				pickableTower->changeTower(Tower::TowerType::Ballista);
			break;

		case Interface::SelectedTowerType::Bomber:
			if (!pickableTower)
				pickableTower = std::make_unique<Tower>(Tower::TowerType::Bomber, Resources::Texture::Bomber, mousePosition, TowersFrames::BOMBER_MAX_FRAMES);
			else
				pickableTower->changeTower(Tower::TowerType::Bomber);
			break;

		case Interface::SelectedTowerType::Wizzard:
			if (!pickableTower)
				pickableTower = std::make_unique<Tower>(Tower::TowerType::Wizzard, Resources::Texture::Wizzard, mousePosition, TowersFrames::WIZZARD_MAX_FRAMES);
			else
				pickableTower->changeTower(Tower::TowerType::Wizzard);
			break;

		default:
			pickableTower.reset();
			break;
		}
	}
}

void Game::updatePickableTower()
{
	//Перевіряємо яка вежа вибрана, чи можна її розмістити, і якщо вона вибрана, переміщуємо за курсором
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

	//Якщо вежі пересікаються, тоді не можна розмістити нову вежу
	for (const auto& tower : towers)
	{
		if (tower && pickableTower && tower->intersects(*pickableTower))
		{
			canPlaceTower = false;
			break;
		}
	}
	//Якщо вежа пересікається з інтерфейосм, тоді не можна розмістити вежу
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
	//Якщо вежа пересікається з картою, тоді не можна розмістити вежу
	if (pickableTower && !map.canPlaceTower(*pickableTower))
	{
		canPlaceTower = false;
	}
}

void Game::placeTower()
{
	//Якщо вежа вибрана, її можна розмістити і вистачає грошей, додаємо до контейнеру
	//веж нову вежу, та віднімаємо гроші
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
	//Повертаємо посилання на контейнер інтерфейсу, якщо він існує
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
	//Створюємо інтерфейс вибору веж, додаємо до нього текст та кнопки
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
	//Створюємо текст з інформацією, та росташовуємо його зліва від інтерфейсу вибору веж
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
		//Якщо стан гри Win створюємо інтерфейс перемоги, та додаємо текст і кнопки до нього
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
		//Якщо залишилось 0 життів створюємо інтерфейс поразки, та додаємо текст і кнопки до нього
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
		//Якщо стан гри Pause створюємо інтерфейс паузи, та додаємо текст і кнопки до нього
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

void Game::playSound(const Resources::Sound soundId, float volume)
{
	auto sound = std::make_unique<sf::Sound>((Resources::sounds.Get(soundId)));
	sound->setVolume(volume);
	sound->play();
	sounds.emplace_back(std::move(sound));
}

void Game::SoundsManager()
{
	for (const auto& enemy : enemies)
	{
		if (!enemy->isAlive())
		{
			switch (enemy->getType())
			{
			case Enemy::EnemyType::Goblin:
				playSound(Resources::Sound::GoblinDeath, 40.f);
				break;
			case Enemy::EnemyType::Orc:
				playSound(Resources::Sound::OrcDeath, 35.f);
				break;
			case Enemy::EnemyType::Wolf:
				playSound(Resources::Sound::WolfDeath, 15.f);
				break;
			default:
				continue;
			}
		}
	}

	if (towerSelled)
	{
		playSound(Resources::Sound::SellTower, 90.f);
		towerSelled = false;
	}

	if (notEnoughMoney)
	{
		playSound(Resources::Sound::NotEnoughMoney, 100.f);
		notEnoughMoney = false;
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

		//Якщо перейти на інше вікно, ставиться пауза
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
				//Якщо нажата ліва кнопка миші, перевіряємо чи в interface знаходиться похідний клас
				//InterfaceContainer, і якщо курсор в цьому контейнері, обробляємо натискання
				for (auto& interfaceComponent : interface)
				{
					if (auto* container = dynamic_cast<InterfaceContainer*>(interfaceComponent.second.get()))
					{
						if (container->contains(mousePosition))
						{
							canPlaceTower = false;
							container->handleClick(mousePosition);
						}
					}
				}
				placeTower();

				//Якщо ми натиснули кнопку Resume, то в залежності від стану гри, прибираємо інтерфейс цього стану
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
				//Якщо натиснута права кнопка миші і курсор зноходиться на вежі, продаємо її
				for (const auto& tower : towers)
				{
					if (tower->intersects(mousePosition) && !pickableTower)
					{
						Interface::addMoney(tower->getPrice() * 0.75f);
						moneyText.setFillColor(sf::Color::Green);
						timeToShowMoneyTextColor = 0.3;
						towers.remove(tower);
						towerSelled = true;
						break;
					}
				}
				//Прибираємо вибрану вежі
				Interface::setSelectedTower(Interface::SelectedTowerType::None);
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

	SoundsManager();
	sounds.remove_if(
		[](const std::unique_ptr<sf::Sound>& sound)
		{
			return sound->getStatus() != sf::Sound::Status::Playing;
		}
	);
	fps.setString("FPS: " + std::to_string(static_cast<int>(1 / deltaTime.asSeconds())));
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

	for (auto& interfaceComponent : interface)
	{
		window.draw(*interfaceComponent.second);
	}

	if (pickableTower)
	{
		window.draw(*pickableTower);
	}
	showGameInfo();
	window.draw(fps);
	window.draw(control);

	window.display();
}


