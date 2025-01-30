#pragma once

#include <UI/UIMenu.h>
#include <SFML/Graphics/Sprite.hpp>
#include <unordered_map>
#include <string>
#include <functional>

class UIButton;
class Game;


class UIScreenMainMenu : public UIMenu
{
public:

	UIScreenMainMenu(UIScreenDescriptor& uis, Game* game);

	~UIScreenMainMenu();

	void render(sf::RenderWindow& window) override;

private:
	sf::Sprite* m_logo;
	sf::Font* m_font;

};
