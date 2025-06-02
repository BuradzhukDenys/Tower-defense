#include "InterfaceContainer.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include "Ballista.h"
#include <iostream>

InterfaceContainer::InterfaceContainer(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& backgroundColor)
	: GUI(size), backgroundColor(backgroundColor)
{
	GUI.setFillColor(backgroundColor);
	GUI.setPosition(position);
	/*buttons.emplace_back(std::make_unique<Button>(
		sf::Vector2f(200.f, 200.f),
		sf::Vector2f(600.f, 800.f),
		sf::Color::Yellow,
		"Hello",
		Button::ButtonType::None
	));*/
}

//void InterfaceContainer::addButton(const InterfaceContainer& container, const sf::Color& color, const std::string& buttonText)
//{
//	sf::Vector2f realSize(container.getSize().x * 0.7f, container.getSize().y / buttonsNumber);
//	buttons.emplace_back(std::make_unique<Button>(
//		realSize,
//		sf::Vector2f(container.getSize().x / 2.f + container.getPosition().x, container.getPosition().y + (realSize.y / 2.f) + MARGIN_BETWEEN_BUTTONS),
//		color,
//		buttonText
//	));
//}

//void InterfaceContainer::addButtons(int count, const sf::Color& color, const std::vector<std::string>& texts, const std::vector<Button::ButtonType>& buttonType)
//{
//	if (count <= 0) return;
//
//	float W = GUI.getSize().x;
//	float H = GUI.getSize().y;
//	float margin = 20.f; // Відступ зверху і знизу
//
//	float buttonWidth = W * 0.7f;
//	float buttonHeight = (H - 2 * margin) / count; // Висота кнопки
//
//	float spacing = (H - 2 * margin) / (count - 1); // Відстань між центрами
//
//	for (int i = 0; i < count; ++i)
//	{
//		float x = GUI.getPosition().x + W / 2.f;
//		float y;
//		if (count == 1) {
//			y = GUI.getPosition().y + H / 2.f;
//		}
//		else {
//			y = GUI.getPosition().y + margin + i * spacing;
//		}
//
//		buttons.emplace_back(std::make_unique<Button>(
//			sf::Vector2f(buttonWidth, buttonHeight),
//			sf::Vector2f(x, y),
//			color,
//			texts[i],
//			buttonType[i]
//		));
//	}
//}

void InterfaceContainer::addButton(const sf::Color& color, const std::string& text, const Button::ButtonType& buttonType)
{
	auto ptrButton = std::make_unique<Button>(
		sf::Vector2f(GUI.getSize().x * 0.7f, (GUI.getSize().y / 4.f) - MARGIN_BETWEEN_BUTTONS),
		sf::Vector2f(GUI.getPosition().x + (GUI.getSize().x / 2.f), GUI.getPosition().y + MARGIN_BETWEEN_BUTTONS),
		color,
		text,
		buttonType
	);

	ptrButton->setPosition({ ptrButton->getPosition().x, ptrButton->getPosition().y + ptrButton->getButtonShape().getOrigin().y });
	
	for (const auto& text : containerTexts)
	{
		if (text && GUI.getGlobalBounds().contains(text->getPosition()))
		{
			std::cout << "X: " << text->getPosition().x << ", Y: " << text->getPosition().y << "\n";
			ptrButton->setSize({ ptrButton->getSize().x, ptrButton->getSize().y - text->getGlobalBounds().size.y });
			ptrButton->setPosition({ ptrButton->getPosition().x, ptrButton->getPosition().y + text->getGlobalBounds().position.y + text->getGlobalBounds().size.y });
			break;
		}
	}

	int buttonsCount = buttons.size() + 1;

	ptrButton->setPosition({ ptrButton->getPosition().x, ptrButton->getPosition().y * buttonsCount });

	std::cout << buttonsCount << "\n";
	buttons.emplace_back(std::move(ptrButton));
}

void InterfaceContainer::addContainerText(const std::string& containerString, const sf::Vector2f& position, const float fontSize)
{
	auto ptrText = std::make_unique<sf::Text>(Resources::fonts.Get(Resources::Font::BasicFont), containerString, fontSize);
	ptrText->setOrigin(ptrText->getLocalBounds().size / 2.f);
	ptrText->setPosition({ position.x, position.y + ptrText->getOrigin().y });

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
