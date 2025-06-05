#include "Map.h"
#include <SFML/Graphics.hpp>
#include "Resources.h"
#include <iostream>

Map::Map(const sf::Vector2u& size, const int tilesCount)
	: mapSprite(Resources::textures.Get(Resources::Texture::MapTiles))/*,
	MAP_WIDTH((size.x - 400.f) / 40.f),
	MAP_HEIGHT(size.y / 40.f)*/
{
	sf::Vector2i mapTilesTextureSize(Resources::textures.Get(Resources::Texture::MapTiles).getSize());
	mapTileSize = { mapTilesTextureSize.x / tilesCount, mapTilesTextureSize.y };
	mapSprite.setTextureRect(sf::IntRect({ 0, 0 }, mapTileSize));
	map.resize(MAP_HEIGHT, std::vector<tileType>(MAP_WIDTH, tileType::Grass));

	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		for (int x = 0; x < MAP_WIDTH; ++x)
		{
			switch (mapPattern[y][x])
			{
			case ' ':
				map[y][x] = tileType::Grass;
				break;
			case 'H':
				map[y][x] = tileType::HorizontalRoad;
				break;
			case 'R':
				map[y][x] = tileType::VerticalRoad;
				break;
			case '<':
				map[y][x] = tileType::TurnRightDown;
				break;
			case '^':
				map[y][x] = tileType::TurnUp;
				break;
			case '>':
				map[y][x] = tileType::TurnRightUp;
				break;
			case 'V':
				map[y][x] = tileType::TurnDown;
				break;
			default:
				break;
			}
		}
	}
}

bool Map::canPlaceTower(const Tower& tower)
{
	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		for (int x = 0; x < MAP_WIDTH; ++x)
		{
			sf::FloatRect tileRect(
				sf::Vector2f(x * mapTileSize.x,
					y * mapTileSize.y),
				sf::Vector2f(mapTileSize.x,
					mapTileSize.y)
			);

			if (tower.intersects(tileRect))
			{
				if (map[y][x] != Map::tileType::Grass)
				{
					return false;
				}
			}
		}
	}

	return true;
}

void Map::updateTurnEnemy(Enemy& enemy)
{
	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		for (int x = 0; x < MAP_WIDTH; ++x)
		{
			sf::FloatRect tileRect(
				sf::Vector2f(x * mapTileSize.x,
					y * mapTileSize.y),
				sf::Vector2f(mapTileSize.x,
					mapTileSize.y)
			);

			//Перевіряємо чи ворог знаходиться в центрі тайла, щоб повернути його. Ми перевіряємо позицію
			//центра ворога та позицію центра тайла, перевіряємо чи їх різниця менша за погрішність в 2 пікселя
			const float epsilon = 2.f;
			if (std::abs(enemy.getSprite().getGlobalBounds().getCenter().x - tileRect.getCenter().x) < epsilon &&
				std::abs(enemy.getSprite().getGlobalBounds().getCenter().y - tileRect.getCenter().y) < epsilon)
			{
				if (map[y][x] == Map::tileType::TurnUp)
				{
					enemy.setDirection(Enemy::Direction::Up);
					break;
				}
				else if (map[y][x] == Map::tileType::TurnDown)
				{
					enemy.setDirection(Enemy::Direction::Down);
					break;
				}
				else if (map[y][x] == Map::tileType::TurnRightDown ||
					map[y][x] == Map::tileType::TurnRightUp)
				{
					enemy.setDirection(Enemy::Direction::Right);
					break;
				}
			}
		}
	}
}

sf::Vector2f Map::getStartMap() const
{
	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		if (map[y][0] != Map::tileType::Grass)
		{
			sf::FloatRect tileRect(
				sf::Vector2f(0, y * mapTileSize.y),
				sf::Vector2f(mapTileSize.x,
					mapTileSize.y)
			);

			return sf::Vector2f(tileRect.position.x, tileRect.getCenter().y);
		}
	}
}

sf::Vector2f Map::getEndMap() const
{
	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		if (map[y][MAP_WIDTH - 1] != Map::tileType::Grass)
		{
			sf::FloatRect tileRect(
				sf::Vector2f(MAP_WIDTH * mapTileSize.x, y * mapTileSize.y),
				sf::Vector2f(mapTileSize.x,
					mapTileSize.y)
			);

			return sf::Vector2f(tileRect.position.x, tileRect.getCenter().y);
		}
	}
}

const float Map::getMapWidth() const
{
	return MAP_WIDTH * mapTileSize.x;
}

const float Map::getMapHeight() const
{
	return MAP_HEIGHT * mapTileSize.y;
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	int currentTile = 0;
	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		for (int x = 0; x < MAP_WIDTH; ++x)
		{
			sf::Sprite tileSprite = mapSprite;
			tileSprite.setPosition(sf::Vector2f(x * mapTileSize.x, y * mapTileSize.y));
			switch (map[y][x])
			{
			case tileType::Grass:
				currentTile = 0;
				tileSprite.setTextureRect(sf::IntRect({ currentTile * mapTileSize.x, 0 }, mapTileSize));
				break;
			case tileType::HorizontalRoad:
				currentTile = 1;
				tileSprite.setTextureRect(sf::IntRect({ currentTile * mapTileSize.x, 0 }, mapTileSize));
				break;
			case tileType::VerticalRoad:
				currentTile = 2;
				tileSprite.setTextureRect(sf::IntRect({ currentTile * mapTileSize.x, 0 }, mapTileSize));
				break;
			case tileType::TurnRightDown:
				currentTile = 3;
				tileSprite.setTextureRect(sf::IntRect({ currentTile * mapTileSize.x, 0 }, mapTileSize));
				break;
			case tileType::TurnUp:
				currentTile = 4;
				tileSprite.setTextureRect(sf::IntRect({ currentTile * mapTileSize.x, 0 }, mapTileSize));
				break;
			case tileType::TurnRightUp:
				currentTile = 5;
				tileSprite.setTextureRect(sf::IntRect({ currentTile * mapTileSize.x, 0 }, mapTileSize));
				break;
			case tileType::TurnDown:
				currentTile = 6;
				tileSprite.setTextureRect(sf::IntRect({ currentTile * mapTileSize.x, 0 }, mapTileSize));
				break;
			default:
				break;
			}

			target.draw(tileSprite, states);
		}
	}
}
