#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace sf
{
	class RenderTarget;
}

class Map : public sf::Transformable, public sf::Drawable
{
public:
	Map(const sf::Vector2f& size);
private:
	enum class tileType
	{
		Grass,
		Road,

	};
	sf::Sprite mapSprite;

	static constexpr int MAP_WIDTH = (1920 - 400) / 40;
	static constexpr int MAP_HEIGHT = 1920 / 40;
	std::vector<std::vector<std::string>> map;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

