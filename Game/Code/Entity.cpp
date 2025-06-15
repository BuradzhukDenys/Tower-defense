#include "Entity.h"

Entity::Entity(Resources::Texture textureID, const int framesCount)
	: sprite(Resources::textures.Get(textureID)),
	framesCount(framesCount), currentFrame(0), timeForLastAnimationPlay(0)
{
	//¬ираховую розм≥р одного кадру спрайта, в залежност≥ в≥д розм≥ру картинки з≥ спрайтами,
	//та к≥льк≥стю кадр≥в
	sf::Vector2i textureSize(Resources::textures.Get(textureID).getSize());
	frameSize = { textureSize.x / framesCount, textureSize.y };
	sprite.setTextureRect(sf::IntRect({ 0, 0 }, frameSize));
}

Entity::~Entity() {}

sf::Vector2f Entity::getPosition() const
{
	return sprite.getPosition();
}

void Entity::setPosition(const sf::Vector2f& position)
{
	sprite.setPosition(position);
}

sf::Angle Entity::getRotation() const
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
