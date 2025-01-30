#pragma once

#include <UI/UIMenu.h>
#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
	class Texture;
}

class UILevelCompleted : public UIMenu
{
public:
	UILevelCompleted(UIScreenDescriptor& uis, Game* game);

	~UILevelCompleted() override;

	void deinit() override;

	void render(sf::RenderWindow& window) override;

private:
	sf::Texture* m_backgorundTexture;
	sf::Sprite* m_popupWindowSprite;
	sf::Font* m_font;
	sf::Text* m_text;
	sf::Text* m_scoreText;
};