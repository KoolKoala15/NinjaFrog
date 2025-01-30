#pragma once

#include <SFML/Graphics.hpp>

class UIButton;
class Game;

class UIScreen 
{

public:
	struct UIScreenDescriptor
	{
		sf::Font* font{ nullptr };
	};

	virtual ~UIScreen() = default;

	virtual void init(Game* game) {};
	virtual void deinit() {};
	virtual void update(float deltaMilliseconds, sf::RenderWindow& window) {};
	virtual void render(sf::RenderWindow& window)= 0;
	virtual UIButton* mouseClick(sf::Vector2i& mousePos) {return nullptr;}
};