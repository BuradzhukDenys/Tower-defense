#include "Map.h"
#include <SFML/Graphics.hpp>
#include "Resources.h"

Map::Map(const sf::Vector2f& size)
	: mapSprite(Resources::textures.Get(Resources::Texture::MapTiles))/*,
	MAP_WIDTH((size.x - 400.f) / 40.f),
	MAP_HEIGHT(size.y / 40.f)*/
{
	mapSprite.setTextureRect(sf::IntRect({0, 0}, {40, 40}));
	mapSprite.setScale(sf::Vector2f(MAP_WIDTH, MAP_HEIGHT));
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mapSprite, states);
}
