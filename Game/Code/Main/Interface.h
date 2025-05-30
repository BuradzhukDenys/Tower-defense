#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>

namespace sf
{
	class Time;
	class RenderWindow;
}

class Interface : public sf::Drawable
{
public:
	enum class TowerType
	{
		Ballista,
		Bomber,
		Wizzard,
		None
	};

	enum class States
	{
		Game,
		Pause
	};

	Interface(const sf::Vector2f& size, const sf::Vector2f& position);

	virtual ~Interface();

	static TowerType getSelectedTower();
	static void setSelectedTower(TowerType towerType);

	const sf::Text& getCurrentRoundText() const;
	const sf::Text& getMoneyText() const;
	const sf::Text& getHealthText() const;

	static int getCurrentRound();
	static int getMoney();

	static int getHealth();
	const sf::Vector2f& getSize();
	const sf::Vector2f& getPosition();

	static void nextRound();
	static void addMoney(const int value);
	static void substractMoney(const int value);
	static void lostHealth();

	void updateCurrentRound();
	void updateMoney();
	void updateHealth();

	static States getCurrentState();
	static void setState(const States& state);

	virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window) = 0;
protected:
	sf::RectangleShape GUI;
private:
	sf::Text currentRoundText;
	sf::Text moneyText;
	sf::Text healthText;
	static States currentState;
	static TowerType selectedTower;
	static int currentRound;
	static int money;
	static int health;
	static constexpr int GAME_FONT_SIZE = 35;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};