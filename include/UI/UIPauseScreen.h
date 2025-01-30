#pragma once

#include <UI/UIMenu.h>
#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
	class Texture;
}

class UIPauseScreen : public UIMenu
{
public:
	UIPauseScreen(UIScreenDescriptor& uis, Game* game);

	~UIPauseScreen();

	void deinit() override;

	void render(sf::RenderWindow& window) override;

private:
	sf::Texture* m_backgorundTexture;
	sf::Sprite* m_popupWindowSprite;
	sf::Font* m_font;
	sf::Text* m_text;
};