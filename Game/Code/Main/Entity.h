#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"

class Enemy;

class Entity : public sf::Transformable, public sf::Drawable
{
public:
	Entity(Resources::Texture textureID, const int framesCount = 1);

	virtual ~Entity();

	const sf::Vector2f& getPosition() const;
	void setPosition(const sf::Vector2f& position);
	const sf::Angle& getRotation() const;
	void setRotation(const sf::Angle& angle);
	const sf::Sprite& getSprite() const;
	const sf::Vector2f& getSize() const;
	virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window, const std::vector<std::unique_ptr<Enemy>>& enemies) = 0;
protected:
	sf::Sprite sprite;
	sf::Vector2i frameSize;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

