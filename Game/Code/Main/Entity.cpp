#include "Entity.h"
#include <SFML/Graphics/RenderTarget.hpp>

Entity::Entity(Resources::Texture textureID, const int framesCount)
	: sprite(Resources::textures.Get(textureID))
{
	sf::Vector2i textureSize(Resources::textures.Get(textureID).getSize());
	frameSize = { textureSize.x / framesCount, textureSize.y };
	sprite.setOrigin(sf::Vector2f(textureSize / 2));
	sprite.setTextureRect(sf::IntRect({ 0, 0 }, frameSize));
}

Entity::~Entity() {}

const sf::Vector2f& Entity::getPosition() const
{
	return this->sprite.getPosition();
}

void Entity::setPosition(const sf::Vector2f& position)
{
	sprite.setPosition(position);
}

const sf::Angle& Entity::getRotation() const
{
	return sprite.getRotation();
}

void Entity::setRotation(const sf::Angle& angle)
{
	sprite.setRotation(angle);
}

const sf::Sprite& Entity::getSprite() const
{
	return sprite;
}

const sf::Vector2f& Entity::getSize() const
{
	return sprite.getGlobalBounds().size;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}
