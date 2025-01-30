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

	sf::Font* m_font;
	UIScreenMainMenu(UIScreenDescriptor& uis, Game* game);

};
