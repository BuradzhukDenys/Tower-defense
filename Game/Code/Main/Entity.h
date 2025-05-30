#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"

class Entity : public sf::Transformable, public sf::Drawable
{
public:
	Entity(Resources::Texture textureID);

	virtual ~Entity();

	const sf::Vector2f& getPosition() const;
	void setPosition(const sf::Vector2f& position);
	const sf::Angle& getRotation() const;
	void setRotation(const sf::Angle& angle);
	const sf::Sprite& getSprite() const;
	virtual void Update(sf::Time deltaTime, const sf::RenderWindow& window) = 0;
protected:
	sf::Sprite sprite;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

