#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <fstream>

int main() {
	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Text Input Example");
	sf::Font font;
	if (!font.openFromFile("Assets/Fonts/arial.ttf")) return -1;

	sf::RectangleShape inputBox(sf::Vector2f(400, 40));
	inputBox.setPosition({ 50, 50 });
	inputBox.setFillColor(sf::Color::Transparent);
	inputBox.setOutlineThickness(2);
	inputBox.setOutlineColor(sf::Color::White);

	sf::Text text(font, "", inputBox.getSize().y);
	text.setPosition({ inputBox.getPosition().x + 10, inputBox.getPosition().y});
	text.setFillColor(sf::Color::White);

	sf::String inputText = L"";
	bool isTyping = false;

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			
			if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mousePressed->button == sf::Mouse::Button::Left) {
					sf::FloatRect inputBoxBounds = inputBox.getGlobalBounds();
					if (inputBoxBounds.contains(sf::Vector2f( mousePressed->position.x, mousePressed->position.y ))) {
						isTyping = true;
						inputBox.setOutlineColor(sf::Color::Green);
					}
					else {
						isTyping = false;
						inputBox.setOutlineColor(sf::Color::White);
					}
				}
			}
			
			if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
				if (isTyping)
				{
					if (!inputText.isEmpty() && textEntered->unicode == '\b') {
						inputText.toWideString().pop_back();
					}
					
					inputText += static_cast<wchar_t>(textEntered->unicode);

					text.setString(inputText);
				}
			}

			if (const auto* keyPressed = event->getIf<sf::Event::KeyReleased>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scancode::Enter && isTyping && !inputText.isEmpty())
				{
					std::ofstream enteredText("Code/Entered text.txt");
					if (enteredText.is_open())
					{
						enteredText << text.getString().toAnsiString();
						text.setString("");
						inputBox.setOutlineColor(sf::Color::White);
						isTyping = false;
					}
					else
					{
						throw std::exception("Couldn't open file");
					}
					enteredText.close();
				}
			}
		}

		window.clear();
		window.draw(inputBox);
		window.draw(text);
		window.display();
	}
	return 0;
}