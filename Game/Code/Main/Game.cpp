#include "Game.h"
#include "Ballista.h"
#include "InterfaceContainer.h"
#include <iostream>

Game::Game()
	: windowTitle("Tower defense"),
	windowSize(sf::VideoMode::getDesktopMode()),
	window(windowSize, windowTitle)
{
	interface.emplace_back(std::make_unique<InterfaceContainer>(
		sf::Vector2f(500.f, windowSize.size.y),
		sf::Vector2f(windowSize.size.x - 500.f, 0.f),
		sf::Color(153, 76, 0)
	));
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
				Interface::setState(Interface::States::Game);

				if (Interface::getCurrentState() == Interface::States::Game)
				{
					Interface::setState(Interface::States::Pause);
				}
			}

			if (keyReleased->scancode == sf::Keyboard::Scancode::NumpadPlus)
			{
				Interface::addMoney(25);
				if (towers[0])
				{
					towers[0]->upgradeAttackSpeed(0.1f, Tower::UpgradeType::additive);
				}
			}
			else if (keyReleased->scancode == sf::Keyboard::Scancode::NumpadMinus && Interface::getMoney() > 0)
			{
				Interface::substractMoney(25);
			}

			if (keyReleased->scancode == sf::Keyboard::Scancode::Backspace && Interface::getHealth() > 0)
			{
				Interface::lostHealth();
			}
		}

		if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mousePressed->button == sf::Mouse::Button::Left)
			{
				bool clickedOnInterface = false;
				for (auto& interfaceComponent : interface) {

					if (auto* container = dynamic_cast<InterfaceContainer*>(interfaceComponent.get())) {
						if (container->contains(mousePosition))
						{
							container->handleClick(mousePosition);
							clickedOnInterface = true;
						}
					}
				}

				if (towers.size() < 20
					&& Interface::getSelectedTower() == Interface::TowerType::Ballista
					&& !clickedOnInterface)
				{
					if (canPlaceTower)
					{
						towers.emplace_back(std::make_unique<Ballista>(Resources::Texture::BallistaSpriteSheet, mousePosition));
					}
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

	for (auto& interfaceComponent : interface)
	{
		interfaceComponent->Update(deltaTime, window);
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
		window.draw(*interfaceComponent);
	}

	if (pickableTower && Interface::getSelectedTower() == Interface::TowerType::Ballista)
	{
		window.draw(*pickableTower);
	}

	window.display();
}


