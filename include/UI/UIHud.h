#pragma once

#include <UI/UIScreeen.h>

class Game;

class UIHud : public UIScreen
{
public:

	UIHud(UIScreenDescriptor& uis, Game* game);

	~UIHud();

	void init(Game* game) override;
	void render(sf::RenderWindow& window) override;

	void updateScore();

private:
	Game* m_game{ nullptr };
	sf::Text* m_currentLevelText{ nullptr };
	sf::Text* m_currentLevelScore{ nullptr };
};