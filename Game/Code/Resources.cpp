#include "Resources.h"
#include <SFML/Graphics/Image.hpp>

ResourceManager<sf::Texture, Resources::Texture> Resources::textures;
ResourceManager<sf::Font, Resources::Font> Resources::fonts;
ResourceManager<sf::SoundBuffer, Resources::Sound> Resources::sounds;
ResourceManager<sf::Music, Resources::Music> Resources::music;
sf::Image Resources::icon;

void Resources::Initialize()
{
	InitializeTextures();
	InitializeFonts();
	InitializeSounds();
	InitializeMusic();
	InitializeIcon();
}

const sf::Image& Resources::getIcon()
{
	return icon;
}

void Resources::InitializeTextures()
{
	textures.Load(Texture::Ballista, "Assets/Textures/SpriteSheetBallista.png");
	textures.Load(Texture::Bomber, "Assets/Textures/BomberSpriteSheet.png");
	textures.Load(Texture::Wizzard, "Assets/Textures/WizzardSpriteSheet.png");
	textures.Load(Texture::MapTiles, "Assets/Textures/MapTiles.png");
	textures.Load(Texture::BallistaShot, "Assets/Textures/BallistaShot.png");
	textures.Load(Texture::BomberShot, "Assets/Textures/BomberShot.png");
	textures.Load(Texture::WizzardShot, "Assets/Textures/WizzardShot.png");
	textures.Load(Texture::Goblin, "Assets/Textures/GoblinSpriteSheet.png");
	textures.Load(Texture::Orc, "Assets/Textures/OrcSpriteSheet.png");
	textures.Load(Texture::Wolf, "Assets/Textures/WolfSpriteSheet.png");
}

void Resources::InitializeFonts()  
{  
   fonts.Load(Font::BasicFont, "Assets/Fonts/arial.ttf");
}

void Resources::InitializeSounds()
{
	sounds.Load(Sound::NotEnoughMoney, "Assets/Sounds/NotEnoughMoneySound.mp3");
	sounds.Load(Sound::GoblinDeath, "Assets/Sounds/goblin-death.mp3");
	sounds.Load(Sound::OrcDeath, "Assets/Sounds/orc-death.mp3");
	sounds.Load(Sound::WolfDeath, "Assets/Sounds/wolf-death.mp3");
	sounds.Load(Sound::SellTower, "Assets/Sounds/sell-tower.mp3");
}

void Resources::InitializeIcon()
{
	if (!icon.loadFromFile("Assets/Icon/towerDefenseIcon.png"))
	{
		return;
	}
}

void Resources::InitializeMusic()
{
	music.Load(Music::BasicMusic, "Assets/Music/One-Bard-Band.mp3");
}
