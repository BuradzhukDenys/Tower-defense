#pragma once
#include "../Tools/ResourceManager.h"

class Resources
{
public:
	enum class Texture
	{
		BallistaSpriteSheet,
		BallistaShot,
		MapTiles
	};

	enum class Font
	{
		BasicFont
	};

	Resources() = delete;

	static void Initialize();

	static ResourceManager<sf::Texture, Texture> textures;
	static ResourceManager<sf::Font, Font> fonts;
private:
	static void InitializeTextures();
	static void InitializeFonts();
};

