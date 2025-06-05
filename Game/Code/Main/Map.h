#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include "Tower.h"
#include "Enemy.h"

namespace sf
{
	class RenderTarget;
	class Time;
	class RenderWindow;
}

class Map : public sf::Transformable, public sf::Drawable
{
public:
	enum class tileType
	{
		Grass,
		HorizontalRoad,
		VerticalRoad,
		TurnRightDown,
		TurnUp,
		TurnRightUp,
		TurnDown
	};

	Map(const sf::Vector2u& size, const int tilesCount = BASE_TILE_NUMBER);
	bool canPlaceTower(const Tower& tower);
	void updateTurnEnemy(Enemy& enemy);
	sf::Vector2f getStartMap() const;
	sf::Vector2f getEndMap() const;
	const float getMapWidth() const;
	const float getMapHeight() const;
private:
	sf::Sprite mapSprite;
	
	sf::Vector2i mapTileSize;
	static constexpr int MAP_WIDTH = (1920 - 300) / 60;
	static constexpr int MAP_HEIGHT = 1080 / 60;
	static constexpr int BASE_TILE_NUMBER = 7;
	std::string mapPattern[MAP_HEIGHT] = {
		"                           ",
		"                           ",
		"                           ",
		"                           ",
		"      >HHHHHHHHV           ",
		"      R        R           ",
		"      R        R           ",
		"      R        R           ",
		"      R        R           ",
		"HHHHHH^        R           ",
		"               R     >HHHHH",
		"               R     R     ",
		"               R     R     ",
		"               R     R     ",
		"               <HHHHH^     ",
		"                           ",
		"                           ",
		"                           "
	};
	std::vector<std::vector<tileType>> map;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

