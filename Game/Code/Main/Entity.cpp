#include "Entity.h"
#include <SFML/Graphics/RenderTarget.hpp>

Entity::Entity(Resources::Texture textureID)
	: sprite(Resources::textures.Get(textureID))
{
	sprite.setOrigin(sf::Vector2f(Resources::textures.Get(textureID).getSize() / 2u));
}

Entity::~Entity() {}

const sf::Vector2f& Entity::getPosition() const
{
	return sprite.getPosition();
}

const void Entity::setPosition(const sf::Vector2f& position)
{
	sprite.setPosition(position);
}

const sf::Angle& Entity::getRotation() const
{
	return sprite.getRotation();
}

const sf::Sprite& Entity::getSprite() const
{
	return sprite;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}
