#pragma once
#include <unordered_map>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio.hpp>

template <typename ResourceType, typename ResourceIdentifier>
class ResourceManager
{
public:
	void Load(const ResourceIdentifier& id, const std::string& filename)
	{
		auto ptr = std::make_unique<ResourceType>();
		if (!ptr->loadFromFile(filename))
		{
			throw std::runtime_error("Failed to load file");
		}

		resourceMap.emplace(id, std::move(ptr));
	}

	ResourceType& Get(const ResourceIdentifier& id) const
	{
		return *resourceMap.at(id);
	}
private:
	std::unordered_map<ResourceIdentifier, std::unique_ptr<ResourceType>> resourceMap;
};

template<typename ResourceIdentifier>
class ResourceManager<sf::Font, ResourceIdentifier>
{
public:
	void Load(const ResourceIdentifier& id, const std::string& filename)
	{
		auto ptr = std::make_unique<sf::Font>();
		if (!ptr->openFromFile(filename))
		{
			throw std::runtime_error("Failed to load file");
		}

		FontMap.emplace(id, std::move(ptr));
	}

	sf::Font& Get(const ResourceIdentifier& id) const
	{
		return *FontMap.at(id);
	}
private:
	std::unordered_map<ResourceIdentifier, std::unique_ptr<sf::Font>> FontMap;
};

template<typename ResourceIdentifier>
class ResourceManager<sf::Music, ResourceIdentifier>
{
public:
	void Load(const ResourceIdentifier& id, const std::string& filename)
	{
		auto ptr = std::make_unique<sf::Music>();
		if (!ptr->openFromFile(filename))
		{
			throw std::runtime_error("Failed to load file");
		}

		MusicMap.emplace(id, std::move(ptr));
	}

	sf::Music& Get(const ResourceIdentifier& id) const
	{
		return *MusicMap.at(id);
	}
private:
	std::unordered_map<ResourceIdentifier, std::unique_ptr<sf::Music>> MusicMap;
};
