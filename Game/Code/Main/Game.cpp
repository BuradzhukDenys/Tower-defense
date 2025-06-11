#include "Game.h"
#include "InterfaceContainer.h"
#include <iostream>

Game::Game(const sf::State& windowState)
	: windowTitle("Tower defense"),
	windowSize(sf::VideoMode::getDesktopMode()),
	window(windowSize, windowTitle, windowState),
	map(windowSize.size),
	currentRoundText(Resources::fonts.Get(Resources::Font::BasicFont)),
	moneyText(Resources::fonts.Get(Resources::Font::BasicFont)),
	livesText(Resources::fonts.Get(Resources::Font::BasicFont))
{
	initializeInterface();
	initializeGameInfo();
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
		if (!pickableTower)
		{
			pickableTower = std::make_unique<Tower>(Tower::TowerType::Ballista ,Resources::Texture::BallistaSpriteSheet, mousePosition, TowersFrames::BALLISTA_MAX_FRAMES);
		}
	}
	/*else if (Interface::getSelectedTower() == Interface::TowerType::Bomber)
	{
		if (!pickableTower || dynamic_cast<Ballista*>(pickableTower.get()) == nullptr)
		{
			pickableTower = std::make_unique<Ballista>(Resources::Texture::BallistaSpriteSheet, mousePosition);
		}
	}*/
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
	if (canPlaceTower)
	{
		if (Interface::getMoney() >= pickableTower->getPrice() &&
			Interface::getSelectedTower() == Interface::TowerType::Ballista)
		{
			towers.emplace_back(std::make_unique<Tower>(Tower::TowerType::Ballista, Resources::Texture::BallistaSpriteSheet, mousePosition, TowersFrames::BALLISTA_MAX_FRAMES));
			Interface::substractMoney(pickableTower->getPrice());
		}
		else
		{
			auto soundPtr = std::make_unique<sf::Sound>(Resources::sounds.Get(Resources::Sound::NotEnoughMoney));
			soundPtr->play();

			sounds.emplace_back(std::move(soundPtr));
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

void Game::deleteInterfaceContainer(const Interface::InterfaceType& interfaceType)
{
	interface.erase(interfaceType);
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

	currentRoundText.setString("Round: " + std::to_string(Interface::getCurrentRound()) + "/" + std::to_string(Interface::getMaxRoudns()));
	currentRoundText.setCharacterSize(GAME_FONT_SIZE);
	currentRoundText.setPosition({ textPosition.x - currentRoundText.getGlobalBounds().size.x - MARGIN_BORDERS, MARGIN_ROWS });

	livesText.setString("Lives: " + std::to_string(Interface::getLives()));
	livesText.setCharacterSize(GAME_FONT_SIZE);
	livesText.setPosition({ textPosition.x - livesText.getGlobalBounds().size.x - MARGIN_BORDERS, currentRoundText.getPosition().y + currentRoundText.getGlobalBounds().size.y + MARGIN_ROWS });

	moneyText.setString("Money: " + std::to_string(Interface::getMoney()));
	moneyText.setCharacterSize(GAME_FONT_SIZE);
	moneyText.setPosition({ textPosition.x - moneyText.getGlobalBounds().size.x - MARGIN_BORDERS, livesText.getPosition().y + livesText.getGlobalBounds().size.y + MARGIN_ROWS });
}

void Game::updateGameInfo()
{
	currentRoundText.setString("Round: " + std::to_string(Interface::getCurrentRound()) + "/" + std::to_string(Interface::getMaxRoudns()));
	livesText.setString("Lives: " + std::to_string(Interface::getLives()));
	moneyText.setString("Money: " + std::to_string(Interface::getMoney()));
}

void Game::checkEnemyReachedEnd()
{
	for (auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		if ((*it)->getPosition().x > map.getMapWidth())
		{
			enemies.erase(it);
			Interface::lostlives();
			break;
		}
	}
}

void Game::checkVictory()
{
	if (Interface::getCurrentRound() == Interface::getMaxRoudns() &&
		(GameState::getState() == GameState::State::Game || GameState::getState() == GameState::State::RoundPlay) &&
		enemies.empty()/*delete*/)
	{
		GameState::setState(GameState::State::Win);
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

void Game::showGameInfo()
{
	window.draw(currentRoundText);
	window.draw(moneyText);
	window.draw(livesText);
}

void Game::Events()
{
	while (const std::optional event = window.pollEvent())
	{
		//delete
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}

		if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			if (keyReleased->scancode == sf::Keyboard::Scan::Escape &&
				(GameState::getState() == GameState::State::Game || GameState::getState() == GameState::State::RoundPlay))
			{
				GameState::setState(GameState::State::Pause);
				addInterfaceContainer(Interface::InterfaceType::PauseInterface,
					sf::Vector2f(windowSize.size),
					sf::Vector2f(0, 0),
					sf::Color(Interface::BASE_BACKGROUND_COLOR)
				);
				auto& container = getInterfaceContainer(Interface::InterfaceType::PauseInterface);
				sf::Vector2f containerCenter = (container.getSize() / 2.f) + container.getPosition();

				container.addContainerText("Pause", sf::Vector2f(containerCenter.x, containerCenter.y - 150.f));
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
					"Resume",
						"Exit"
				},
					std::vector<Button::ButtonType>{//Типи кнопок
					Button::ButtonType::Resume,
						Button::ButtonType::Exit
				}
				);
			}

			if (keyReleased->scancode == sf::Keyboard::Scancode::Space)
			{
				Interface::nextRound();
			}

			if (keyReleased->scancode == sf::Keyboard::Scancode::NumpadPlus)
			{
				Interface::addMoney(25);
				
				if (!towers.empty())
				{
					towers.begin()->get()->upgradeAttackSpeed(1, Tower::UpgradeType::additive);
				}
			}
			else if (keyReleased->scancode == sf::Keyboard::Scancode::NumpadMinus)
			{
				Interface::substractMoney(25);
			}

			if (keyReleased->scancode == sf::Keyboard::Scancode::Backspace)
			{
				Interface::lostlives();
			}

			if (keyReleased->scancode == sf::Keyboard::Scancode::Q)
			{
				enemies.emplace_back(std::make_unique<Enemy>(Enemy::EnemyType::Goblin, Resources::Texture::Goblin, sf::Vector2f(550, 600), EnemiesFrames::GOBLIN_MAX_FRAMES));
			}

			if (keyReleased->scancode == sf::Keyboard::Scancode::W)
			{
				enemies.emplace_back(std::make_unique<Enemy>(Enemy::EnemyType::Orc, Resources::Texture::Orc, map.getStartMap(), EnemiesFrames::ORC_MAX_FRAMES));
			}

			if (keyReleased->scancode == sf::Keyboard::Scancode::E)
			{
				enemies.emplace_back(std::make_unique<Enemy>(Enemy::EnemyType::Wolf, Resources::Texture::Wolf, map.getStartMap(), EnemiesFrames::WOLF_MAX_FRAMES));
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

				switch (GameState::getState())
				{
				case GameState::State::Game:
				case GameState::State::RoundPlay:
					if (interface.find(Interface::InterfaceType::PauseInterface) != interface.end())
					{
						interface.erase(Interface::InterfaceType::PauseInterface);
					}
					break;
				case GameState::State::Restart:
					if (interface.find(Interface::InterfaceType::WinnerInterface) != interface.end() ||
						interface.find(Interface::InterfaceType::LoserInterface) != interface.end())
					{
						interface.erase(Interface::InterfaceType::WinnerInterface);
						interface.erase(Interface::InterfaceType::LoserInterface);
					}
					Interface::reset();
					towers.clear();
					enemies.clear();
					GameState::setState(GameState::State::Game);
					break;
				case GameState::State::Exit:
					window.close();
					break;
				default:
					break;
				}

				placeTower();
			}
			else if (mouseReleased->button == sf::Mouse::Button::Right)
			{
				for (const auto& tower : towers)
				{
					if (tower->intersects(mousePosition) && !pickableTower)
					{
						towers.remove(tower);
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
			if (enemy)
			{
				enemy->Update(deltaTime, mousePosition, enemies);
				map.updateTurnEnemy(*enemy);
				if (!enemy->isAlive())
				{
					Interface::addMoney(enemy->getMoneyReward());
					enemies.remove(enemy);
					break;
				}
			}
		}
		checkEnemyReachedEnd();

		updatePickableTower();
		for (auto& tower : towers)
		{
			tower->Update(deltaTime, mousePosition, enemies);
		}

		updateGameInfo();
	}
	mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	checkVictory();
	checkLoss();

	for (auto& interfaceComponent : interface)
	{
		interfaceComponent.second->Update(deltaTime, window);
	}

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


