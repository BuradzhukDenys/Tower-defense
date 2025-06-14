#include "InterfaceContainer.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>
#include "WavesManager.h"

InterfaceContainer::InterfaceContainer(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& backgroundColor)
	: GUI(size), backgroundColor(backgroundColor)
{
	GUI.setFillColor(backgroundColor);
	GUI.setPosition(position);
}

void InterfaceContainer::addButtons(const int buttonsCount, const std::vector<sf::Vector2f>& sizes, const sf::Vector2f& firstButtonPosition, const std::vector<sf::Color>& colors, const std::vector<std::string>& texts, const std::vector<Button::ButtonType>& buttonTypes)
{
	for (size_t i = 0; i < buttonsCount; i++)
	{
		auto ptrButton = std::make_unique<Button>(
			sf::Vector2f(sizes[i].x, sizes[i].y),
			sf::Vector2f(firstButtonPosition.x, firstButtonPosition.y + MARGIN_BETWEEN_COMPONENTS),
			colors[i],
			texts[i],
			buttonTypes[i]
		);

		if (!this->containerTexts.empty() && buttons.empty())
		{
			float lastTextLowerBound = 0;
			for (const auto& text : containerTexts)
			{
				if (text->getPosition().y + text->getGlobalBounds().size.y > lastTextLowerBound)
				{
					lastTextLowerBound = text->getPosition().y + text->getGlobalBounds().size.y;
				}
			}
			ptrButton->setPosition(sf::Vector2f(ptrButton->getPosition().x, lastTextLowerBound + MARGIN_BETWEEN_COMPONENTS));
		}

		if (!this->buttons.empty())
		{
			ptrButton->setPosition(sf::Vector2f(ptrButton->getPosition().x, (buttons[i - 1]->getPosition().y + buttons[i - 1]->getSize().y) + MARGIN_BETWEEN_COMPONENTS));
		}

		buttons.emplace_back(std::move(ptrButton));
	}
}


void InterfaceContainer::addContainerText(const std::string& containerString, const sf::Vector2f& position, const sf::Color& colorText, const float fontSize)
{
	auto ptrText = std::make_unique<sf::Text>(Resources::fonts.Get(Resources::Font::BasicFont), containerString, fontSize);
	ptrText->setOrigin(ptrText->getLocalBounds().size / 2.f);

	sf::Vector2f positionInGUI = position;

	if (!GUI.getGlobalBounds().contains(positionInGUI))
	{
		positionInGUI = GUI.getPosition();
	}

	ptrText->setPosition(sf::Vector2f(positionInGUI.x, positionInGUI.y + ptrText->getOrigin().y + MARGIN_BETWEEN_COMPONENTS));
	ptrText->setFillColor(colorText);

	containerTexts.emplace_back(std::move(ptrText));
}

float InterfaceContainer::getButtonLowerBound(const Button::ButtonType& buttonType)
{
	for (const auto& button : buttons)
	{
		if (button->getButtonType() == buttonType)
		{
			return button->getPosition().y + button->getSize().y;
		}
	}
}

sf::RectangleShape InterfaceContainer::getGUI() const
{
	return GUI;
}

sf::Vector2f InterfaceContainer::getSize() const
{
	return GUI.getSize();
}

sf::Vector2f InterfaceContainer::getPosition() const
{
	return GUI.getPosition();
}

sf::Vector2f InterfaceContainer::getContainerTextPositionSize(const std::string& text) const
{
	if (!containerTexts.empty())
	{
		for (const auto& containerText : containerTexts)
		{
			if (containerText->getString() == text)
			{
				return containerText->getPosition() + containerText->getGlobalBounds().size;
			}
		}
	}

	return { 0, MARGIN_BETWEEN_COMPONENTS };
}

void InterfaceContainer::handleClick(const sf::Vector2f& mousePos)
{
	for (const auto& button : buttons) {
		if (button->isClicked(mousePos)) {
			switch (button->getButtonType())
			{
			case Button::ButtonType::Ballista:
				Interface::setSelectedTower(Interface::TowerType::Ballista);
				break;
			case Button::ButtonType::Bomber:
				Interface::setSelectedTower(Interface::TowerType::Bomber);
				break;
			case Button::ButtonType::Wizzard:
				Interface::setSelectedTower(Interface::TowerType::Wizzard);
				break;
			case Button::ButtonType::Play:
				if (GameState::getState() == GameState::State::Game)
				{
					GameState::setStateBeforePause(GameState::State::WavePlay);
					GameState::setState(GameState::State::WavePlay);
					WavesManager::startWave();
				}
				break;
			case Button::ButtonType::Resume:
				GameState::setState(GameState::getStateBeforePause());
				break;
			case Button::ButtonType::Restart:
				GameState::setState(GameState::State::Restart);
				break;
			case Button::ButtonType::Exit:
				GameState::setState(GameState::State::Exit);
				break;
			default:
				break;
			}
		}
	}
}

void InterfaceContainer::Update(sf::Time deltaTime, const sf::RenderWindow& window)
{
	sf::Vector2f mousePosition(sf::Mouse::getPosition(window));
	for (const auto& button : buttons) {
		button->Update(deltaTime, window);
	}
}

bool InterfaceContainer::contains(const sf::Vector2f& point) const
{
	return GUI.getGlobalBounds().contains(point);
}

void InterfaceContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(GUI, states);

	for (const auto& button : buttons)
	{
		target.draw(*button, states);
	}

	for (const auto& text : containerTexts)
	{
		target.draw(*text, states);
	}
}
