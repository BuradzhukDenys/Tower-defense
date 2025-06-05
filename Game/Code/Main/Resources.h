#pragma once
#include "../Tools/ResourceManager.h"

class Resources
{
public:
	enum class Texture
	{
		BallistaSpriteSheet,
		BallistaShot,
		MapTiles,
		Goblin
	};

	enum class Font
	{
		BasicFont
	};

	enum class Sound
	{
		NotEnoughMoney
	};

	Resources() = delete;

	static void Initialize();

	static ResourceManager<sf::Texture, Texture> textures;
	static ResourceManager<sf::Font, Font> fonts;
	static ResourceManager<sf::SoundBuffer, Sound> sounds;
private:
	static void InitializeTextures();
	static void InitializeFonts();
	static void InitializeSounds();
};

