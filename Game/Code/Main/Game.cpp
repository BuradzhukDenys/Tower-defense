#include "Game.h"
#include "Ballista.h"
#include "Goblin.h"
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
		if (GameState::getState() != GameState::State::Pause)
		{
			Update(deltaTime.restart());
		}
		Render();
	}
}

void Game::checkSelectedTower()
{
	if (Interface::getSelectedTower() == Interface::TowerType::Ballista)
	{
		if (!pickableTower || dynamic_cast<Ballista*>(pickableTower.get()) == nullptr)
		{
			pickableTower = std::make_unique<Ballista>(Resources::Texture::BallistaSpriteSheet, mousePosition);
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
		if (Interface::getMoney() >= Ballista::getPrice() &&
			Interface::getSelectedTower() == Interface::TowerType::Ballista)
		{
			towers.emplace_back(std::make_unique<Ballista>(Resources::Texture::BallistaSpriteSheet, mousePosition));
			Interface::substractMoney(Ballista::getPrice());
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
	interface.emplace(interfaceType,
		std::make_unique<InterfaceContainer>(
			containerSize,
			containerPosition,
			containerColor
		));
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

	auto it = interface.find(Interface::InterfaceType::SelectTowerInterface);
	if (it != interface.end())
	{
		if (auto* container = dynamic_cast<InterfaceContainer*>(it->second.get()))
		{
			float containerCenterX = (container->getSize().x / 2.f) + container->getPosition().x;

			container->addContainerText(std::string("Towers"), sf::Vector2f(containerCenterX, container->getPosition().y));
			container->addButtons(
				4,//Кількість кнопок
				std::vector<sf::Vector2f>{//Розміри кнопок
					sf::Vector2f(0, 150.f),
					sf::Vector2f(0, 150.f),
					sf::Vector2f(0, 150.f),
					sf::Vector2f(0, 165.f)
				},
				std::vector<sf::Vector2f>{//Позиції кнопок
					sf::Vector2f(containerCenterX, container->getPosition().y),
					sf::Vector2f(containerCenterX, container->getPosition().y),
					sf::Vector2f(containerCenterX, container->getPosition().y),
					sf::Vector2f(containerCenterX, container->getPosition().y)
				},
				std::vector<sf::Color>{sf::Color::Blue, sf::Color::Blue, sf::Color::Blue, sf::Color(0, 130, 0)/*Зелений колір*/},//Кольори кнопок
				std::vector<std::string>{//Текст кнопок
					"Ballista\n" + std::to_string(Ballista::getPrice()) + "$",
					"Bomber\n" + std::to_string(Ballista::getPrice()) + "$",
					"Wizzard\n" + std::to_string(Ballista::getPrice()) + "$",
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
	}
}

void Game::initializeGameInfo()
{
	sf::Vector2f textPosition;

	auto it = interface.find(Interface::InterfaceType::SelectTowerInterface);
	if (it != interface.end())
	{
		if (auto* container = dynamic_cast<InterfaceContainer*>(it->second.get()))
		{
			textPosition = container->getPosition();
		}
	}

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
		if (event->is<sf::Event::Closed>()
			|| (event->is<sf::Event::KeyPressed>()
				&& event->getIf< sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scan::Escape))
		{
			window.close();
		}

		if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			if (keyReleased->scancode == sf::Keyboard::Scancode::Space)
			{
				Interface::nextRound();

				if (GameState::getState() == GameState::State::Game)
				{
					GameState::setState(GameState::State::Pause);
					addInterfaceContainer(Interface::InterfaceType::PauseInterface,
						sf::Vector2f(windowSize.size),
						sf::Vector2f(0, 0),
						sf::Color(Interface::PAUSE_BACKGROUND_COLOR)
					);
				}
				else
				{
					deleteInterfaceContainer(Interface::InterfaceType::PauseInterface);
					GameState::setState(GameState::State::Game);
				}
			}

			if (keyReleased->scancode == sf::Keyboard::Scancode::NumpadPlus)
			{
				Interface::addMoney(25);
				std::cout << GameState::getState();
			}
			else if (keyReleased->scancode == sf::Keyboard::Scancode::NumpadMinus)
			{
				Interface::substractMoney(25);
			}

			if (keyReleased->scancode == sf::Keyboard::Scancode::Backspace)
			{
				Interface::lostlives();
			}

			if (keyReleased->scancode == sf::Keyboard::Scancode::W)
			{
				enemies.emplace_back(std::make_unique<Goblin>(Resources::Texture::Goblin, map.getStartMap()));
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
	mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	updatePickableTower();

	for (const auto& enemy : enemies)
	{
		if (enemy)
		{
			enemy->Update(deltaTime, window, enemies);
			map.updateTurnEnemy(*enemy);
			if (!enemy->isAlive())
			{
				Interface::addMoney(enemy->getMoney());
				enemies.remove(enemy);
				break;
			}
		}
	}
	checkEnemyReachedEnd();

	for (auto& tower : towers)
	{
		tower->Update(deltaTime, window, enemies);
	}

	updateGameInfo();
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


