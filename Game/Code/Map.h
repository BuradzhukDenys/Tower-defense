#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include "Tower.h"
#include "Enemy.h"

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
	sf::Vector2f getStartRoad() const;
	const float getMapWidth() const;
private:
	sf::Sprite mapSprite;
	sf::Vector2i mapTileSize;

	//!!!!!Тільки для розміру екрана 1920x1080!!!!!
	//Розмір та шаблон карти
	static constexpr int MAP_WIDTH = (1920 - 300) / 60;//300 - розмір інтерфейсу
	static constexpr int MAP_HEIGHT = 1080 / 60;//60 - розмір тайлу
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

