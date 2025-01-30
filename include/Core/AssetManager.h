#pragma once

#include <string>
#include <unordered_map>

namespace sf
{
	class Texture;
	class Font;
}

class AssetManager
{
	public:

		static AssetManager* getInstance();

		~AssetManager();


		void clear();

		// Textures
		sf::Texture* loadTexture(const char* assetPath);
		sf::Texture* getTexture(const char* assetPath) const;
		void unloadTexture(const char* assetPath);
		void unloadTexture(const sf::Texture* assetPath);

		// Fonts
		sf::Font* loadFont(const char* assetPath);
		sf::Font* getFont(const char* assetPath) const;
		void unloadFont(const char* assetPath);
		void unloadFont(const sf::Font* font);

	private:

		static AssetManager* s_instance;

		std::unordered_map<std::string, sf::Texture*> m_texturePathToTexture;

		std::unordered_map<std::string, sf::Font*> m_fontPathToFont;
};