#include "Map.h"
#include <SFML/Graphics.hpp>
#include "Resources.h"

Map::Map()
	: mapSprite(Resources::textures.Get(Resources::Texture::MapTiles))
{
	mapSprite.setTextureRect(sf::IntRect({0, 0}, {40, 40}));
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			if (tileMap[i][j] == 0)
			{
				mapSprite.setTextureRect(sf::IntRect({ 0, 0 }, { 40, 40 }));
			}
			target.draw(mapSprite, states);
		}
	}
}
