#include "InterfaceContainer.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>

InterfaceContainer::InterfaceContainer(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& backgroundColor)
	: GUI(size), backgroundColor(backgroundColor)
{
	GUI.setFillColor(backgroundColor);
	GUI.setPosition(position);
}

void InterfaceContainer::addButtons(const int buttonsCount, const std::vector<sf::Vector2f>& sizes, const std::vector<sf::Vector2f>& positions, const std::vector<sf::Color>& colors, const std::vector<std::string>& texts, const std::vector<Button::ButtonType>& buttonTypes)
{
	for (size_t i = 0; i < buttonsCount; i++)
	{
		auto ptrButton = std::make_unique<Button>(
			sf::Vector2f(GUI.getSize().x * 0.7f, sizes[i].y),
			sf::Vector2f(positions[i].x, positions[i].y + MARGIN_BETWEEN_COMPONENTS),
			colors[i],
			texts[i],
			buttonTypes[i]
		);

		if (!this->containerTexts.empty() && buttons.empty())
		{
			for (const auto& text : containerTexts)
			{
				ptrButton->setPosition(sf::Vector2f(ptrButton->getPosition().x, ptrButton->getPosition().y + text->getPosition().y + text->getGlobalBounds().size.y));
			}
		}

		if (!this->buttons.empty())
		{
			ptrButton->setPosition(sf::Vector2f(ptrButton->getPosition().x, ptrButton->getPosition().y + buttons[i - 1]->getPosition().y + buttons[i - 1]->getSize().y));
		}
		
		buttons.emplace_back(std::move(ptrButton));
	}
}

void InterfaceContainer::addContainerText(const std::string& containerString, const sf::Vector2f& position, const float fontSize)
{
	auto ptrText = std::make_unique<sf::Text>(Resources::fonts.Get(Resources::Font::BasicFont), containerString, fontSize);
	ptrText->setOrigin(ptrText->getLocalBounds().size / 2.f);

	sf::Vector2f positionInGUI = position;

	if (!GUI.getGlobalBounds().contains(positionInGUI))
	{
		positionInGUI = GUI.getPosition();
	}

	ptrText->setPosition(sf::Vector2f(positionInGUI.x, positionInGUI.y + ptrText->getOrigin().y + MARGIN_BETWEEN_COMPONENTS));

	containerTexts.emplace_back(std::move(ptrText));
}

const sf::RectangleShape& InterfaceContainer::getGUI() const
{
	return GUI;
}

const sf::Vector2f& InterfaceContainer::getSize() const
{
	return GUI.getSize();
}

const sf::Vector2f& InterfaceContainer::getPosition() const
{
	return GUI.getPosition();
}

const sf::Vector2f& InterfaceContainer::getContainerTextSize(const std::string& text) const
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
