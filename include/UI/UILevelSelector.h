#pragma once

#include <UI/UIMenu.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>

class UIButton;

class UILevelSelector : public UIMenu
{
public:
	struct LevelSelectorDescriptor : public UIMenuDescriptor
	{
		int totalButtons{ 0 };
		int rows{ 0 };
		int cols{ 0 };
		float horizontalEdgesSpacer{ .0f }; //distance between the grid and the sides of the window
		float verticalEdgesSpacer{ .0f };
		float buttonsSpacing{ .0f };
		sf::Texture* buttonTexture;
		int buttonCharSize{ 10 };
		sf::Color buttonTextColor = sf::Color::Magenta;
		sf::Vector2f textPositionAdjustment{ .0f, .0f };
		sf::Vector2f buttonScale{ 1.0f, 1.0f };
		sf::Texture* exitButtonTexture;
		sf::Vector2f exitButtonScale;
		sf::Vector2f exitButtonPosition;
	};
	
	UILevelSelector(UIScreenDescriptor& uis, Game* game);

	~UILevelSelector();

	void render(sf::RenderWindow& window) override;

	void init(Game* game) override;
	void deinit() override;

private:
	sf::Font* m_font{ nullptr };
#if DEBUG_MODE
	std::vector<sf::RectangleShape*> rectangles;
#endif
};
