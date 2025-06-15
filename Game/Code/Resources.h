#pragma once
#include "Tools/ResourceManager.h"

class Resources
{
public:
	//Ідентифікатори доступу до ресурсів
	enum class Texture
	{
		Ballista,
		Bomber,
		Wizzard,
		BallistaShot,
		BomberShot,
		WizzardShot,
		MapTiles,
		Goblin,
		Orc,
		Wolf
	};

	enum class Font
	{
		BasicFont
	};

	enum class Sound
	{
		NotEnoughMoney,
		GoblinDeath,
		OrcDeath,
		WolfDeath,
		SellTower,
	};

	enum class Music
	{
		BasicMusic
	};

	Resources() = delete;

	static void Initialize();
	static const sf::Image& getIcon();

	static ResourceManager<sf::Texture, Texture> textures;
	static ResourceManager<sf::Font, Font> fonts;
	static ResourceManager<sf::SoundBuffer, Sound> sounds;
	static ResourceManager<sf::Music, Music> music;
private:
	//Загружаємо всі текстури, звуки, музику, шрифт та іконку, один раз, щоб потім до них доступатися,
	//та не загружати кожен раз
	static void InitializeTextures();
	static void InitializeFonts();
	static void InitializeSounds();
	static void InitializeIcon();
	static void InitializeMusic();

	static sf::Image icon;
};

