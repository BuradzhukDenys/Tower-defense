#include "Resources.h"

ResourceManager<sf::Texture, Resources::Texture> Resources::textures;
ResourceManager<sf::Font, Resources::Font> Resources::fonts;
ResourceManager<sf::SoundBuffer, Resources::Sound> Resources::sounds;

void Resources::Initialize()
{
	InitializeTextures();
	InitializeFonts();
	InitializeSounds();
}

void Resources::InitializeTextures()
{
	textures.Load(Texture::BallistaSpriteSheet, "Assets/Textures/SpriteSheetBallista.png");
	textures.Load(Texture::MapTiles, "Assets/Textures/MapTiles.png");
	textures.Load(Texture::BallistaShot, "Assets/Textures/BallistaShot.png");
	textures.Load(Texture::Goblin, "Assets/Textures/Goblin.png");
}

void Resources::InitializeFonts()  
{  
   fonts.Load(Font::BasicFont, "Assets/Fonts/arial.ttf");
}

void Resources::InitializeSounds()
{
	sounds.Load(Sound::NotEnoughMoney, "Assets/Sounds/NotEnoughMoneySound.mp3");
}
