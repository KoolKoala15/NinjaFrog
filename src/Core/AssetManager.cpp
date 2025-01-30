#include <Core/AssetManager.h>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

AssetManager* AssetManager::s_instance{ nullptr };

AssetManager* AssetManager::getInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new AssetManager();
	}
	return s_instance;
}

AssetManager::~AssetManager()
{
	clear();
}

void AssetManager::clear()
{
	for (auto it : m_texturePathToTexture)
	{
		delete it.second;
	}
	m_texturePathToTexture.clear();
	for (auto it : m_fontPathToFont)
	{
		delete it.second;
	}
	m_fontPathToFont.clear();
}

sf::Texture* AssetManager::loadTexture(const char* assetPath)
{
	auto it = m_texturePathToTexture.find(assetPath);
	if (it != m_texturePathToTexture.end())
	{
		return it->second; // Already loaded, reuse
	}
	else
	{
		sf::Texture* newTexture = new sf::Texture();
		const bool loadOk = newTexture->loadFromFile(assetPath);
		if (!loadOk)
		{
			delete newTexture;
			return nullptr;
		}
		m_texturePathToTexture[assetPath] = newTexture;
		return newTexture; // just created a new one
	}
}

sf::Texture* AssetManager::getTexture(const char* assetPath) const
{
	const auto it = m_texturePathToTexture.find(assetPath);
	return (it != m_texturePathToTexture.end()) ? it->second : nullptr;
}

void AssetManager::unloadTexture(const char* assetPath)
{
	auto it = m_texturePathToTexture.find(assetPath);
	if (it != m_texturePathToTexture.end())
	{
		delete it->second;
		m_texturePathToTexture.erase(it);
	}
}
void AssetManager::unloadTexture(const sf::Texture* texture)
{
	if (texture == nullptr) return;

	auto it = std::find_if(m_texturePathToTexture.begin(), m_texturePathToTexture.end(),
		[texture](const std::pair<std::string, sf::Texture*>& entry) {
			return entry.second == texture;
		});
	if (it != m_texturePathToTexture.end())
	{
		delete it->second;
		m_texturePathToTexture.erase(it);
	}
}




sf::Font* AssetManager::loadFont(const char* assetPath)
{
	auto it = m_fontPathToFont.find(assetPath);
	if (it != m_fontPathToFont.end())
	{
		return it->second; 
	}
	else
	{
		sf::Font* newFont = new sf::Font();
		const bool loadOk = newFont->loadFromFile(assetPath);
		if (!loadOk)
		{
			delete newFont;
			return nullptr;
		}
		m_fontPathToFont[assetPath] = newFont;
		return newFont; 
	}
}

sf::Font* AssetManager::getFont(const char* assetPath) const
{
	const auto it = m_fontPathToFont.find(assetPath);
	return (it != m_fontPathToFont.end()) ? it->second : nullptr;
}

void AssetManager::unloadFont(const char* assetPath)
{
	auto it = m_fontPathToFont.find(assetPath);
	if (it != m_fontPathToFont.end())
	{
		delete it->second;
		m_fontPathToFont.erase(it);
	}
}

void AssetManager::unloadFont(const sf::Font* font)
{
	if (font == nullptr) return;

	auto it = std::find_if(m_fontPathToFont.begin(), m_fontPathToFont.end(),
		[font](const std::pair<std::string, sf::Font*>& entry) {
			return entry.second == font;
		});
	if (it != m_fontPathToFont.end())
	{
		delete it->second;
		m_fontPathToFont.erase(it);
	}
}