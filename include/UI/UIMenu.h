#pragma once

#include <UI/UIScreeen.h>
#include <SFML/Graphics/Sprite.hpp>
#include <vector>
#include <iostream>

class UIButton;
class Game;

class UIMenu : public UIScreen
{
public:

	struct UIMenuDescriptor : UIScreenDescriptor
	{
		sf::Texture* backgroundTexture{ nullptr };
		sf::Vector2f backgroundScale{ 1.f,1.f };

	};

	UIMenu(UIScreenDescriptor& uis, Game* game);

	virtual ~UIMenu();

	virtual void update(float deltaMilliseconds, sf::RenderWindow& window) override;
	virtual void render(sf::RenderWindow& window) override;
	virtual UIButton* mouseClick(sf::Vector2i& mousePos) override;

	std::vector<UIButton*> m_buttons;
protected:
	sf::Sprite m_backgorundSprite;
	float m_backgroundAnim{ .0f };
	float m_backgroundAnimSpeed{ 1.f };

};