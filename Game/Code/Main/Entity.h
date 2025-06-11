#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"

class Enemy;

class Entity : public sf::Transformable, public sf::Drawable
{
public:
	Entity(Resources::Texture textureID, const int framesCount = 1);

	virtual ~Entity();

	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f& position);
	sf::Angle getRotation() const;
	void setRotation(const sf::Angle& angle);
	const sf::Sprite& getSprite() const;
	sf::Vector2f getSize() const;

	virtual void playAnimation(sf::Time deltaTime) {};
	virtual void Update(sf::Time deltaTime, const sf::Vector2f& mousePosition, const std::list<std::unique_ptr<Enemy>>& enemies) = 0;
protected:
	sf::Sprite sprite;
	sf::Vector2i frameSize;
	int framesCount;
	int currentFrame;
	float timeForLastAnimationPlay;
	bool isAnimationPlaying = false;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

