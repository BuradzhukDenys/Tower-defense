#include "Game.h"
#include "Ballista.h"
#include "InterfaceContainer.h"
#include <iostream>

Game::Game()
	: windowTitle("Tower defense"),
	windowSize(sf::VideoMode::getDesktopMode()),
	window(windowSize, windowTitle),
	currentRoundText(Resources::fonts.Get(Resources::Font::BasicFont)),
	moneyText(Resources::fonts.Get(Resources::Font::BasicFont)),
	livesText(Resources::fonts.Get(Resources::Font::BasicFont))
{
	interface.emplace(Interface::InterfaceType::SelectTowerInterface, std::make_unique<InterfaceContainer>(
		sf::Vector2f(400.f, windowSize.size.y),
		sf::Vector2f(windowSize.size.x - 400.f, 0.f),
		sf::Color(153, 76, 0)
	));

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
			pickableTower = std::make_unique<Ballista>(Resources::Texture::BallistaSpriteSheet, mousePosition);
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

	currentRoundText.setString("Round: " + std::to_string(Interface::getCurrentRound()) + "/25");
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
	currentRoundText.setString("Round: " + std::to_string(Interface::getCurrentRound()) + "/25");
	livesText.setString("Lives: " + std::to_string(Interface::getLives()));
	moneyText.setString("Money: " + std::to_string(Interface::getMoney()));
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
			if (keyReleased->scancode == sf::Keyboard::Scancode::Space && Interface::getCurrentRound() < 25)
			{
				Interface::nextRound();

				if (Interface::getCurrentState() == Interface::States::Game)
				{
					Interface::setState(Interface::States::Pause);
					interface.emplace(Interface::InterfaceType::PauseInterface, std::make_unique<InterfaceContainer>(
						sf::Vector2f(windowSize.size),
						sf::Vector2f(0, 0),
						sf::Color(Interface::PAUSE_BACKGROUND_COLOR)
					));
				}
				else
				{
					interface.erase(Interface::InterfaceType::PauseInterface);
					Interface::setState(Interface::States::Game);
				}
			}

			if (keyReleased->scancode == sf::Keyboard::Scancode::NumpadPlus)
			{
				Interface::addMoney(25);
			}
			else if (keyReleased->scancode == sf::Keyboard::Scancode::NumpadMinus && Interface::getMoney() > 0)
			{
				Interface::substractMoney(25);
			}

			if (keyReleased->scancode == sf::Keyboard::Scancode::Backspace && Interface::getLives() > 0)
			{
				Interface::lostlives();
			}
		}

		if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mousePressed->button == sf::Mouse::Button::Left)
			{
				bool clickedOnInterface = false;
				for (auto& interfaceComponent : interface) {

					if (auto* container = dynamic_cast<InterfaceContainer*>(interfaceComponent.second.get())) {
						if (container->contains(mousePosition))
						{
							container->handleClick(mousePosition);
							clickedOnInterface = true;
						}
					}
				}

				if (canPlaceTower && !clickedOnInterface)
				{
					if (Interface::getSelectedTower() == Interface::TowerType::Ballista)
					{

					}
					towers.emplace_back(std::make_unique<Ballista>(Resources::Texture::BallistaSpriteSheet, mousePosition));
				}
			}
			else if (mousePressed->button == sf::Mouse::Button::Right)
			{
				for (auto it = towers.begin(); it != towers.end(); ++it)
				{
					if ((*it)->intersects(sf::Vector2f(mousePosition)) && !pickableTower)
					{
						towers.erase(it);
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
	mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
	updatePickableTower();

	for (auto& tower : towers)
	{
		tower->Update(deltaTime, window);
	}

	updateGameInfo();
	for (auto& interfaceComponent : interface)
	{
		interfaceComponent.second->Update(deltaTime, window);
	}
}

void Game::Render()
{
	window.clear();

	for (auto& tower : towers)
	{
		window.draw(*tower);
	}

	for (auto& interfaceComponent : interface)
	{
		window.draw(*interfaceComponent.second);
	}
	showGameInfo();

	if (pickableTower && Interface::getSelectedTower() == Interface::TowerType::Ballista)
	{
		window.draw(*pickableTower);
	}

	window.display();
}


