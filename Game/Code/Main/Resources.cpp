#include "Resources.h"

ResourceManager<sf::Texture, Resources::Texture> Resources::textures;
ResourceManager<sf::Font, Resources::Font> Resources::fonts;

void Resources::Initialize()
{
	InitializeTextures();
	InitializeFonts();
}

void Resources::InitializeTextures()
{
	textures.Load(Texture::BallistaSpriteSheet, "Assets/Textures/SpriteSheetBallista.png");
	textures.Load(Texture::MapTiles, "Assets/Textures/MapTiles.png");
	textures.Load(Texture::BallistaShot, "Assets/Textures/BallistaShot.png");
}

void Resources::InitializeFonts()  
{  
   fonts.Load(Font::BasicFont, "Assets/Fonts/arial.ttf");
}
