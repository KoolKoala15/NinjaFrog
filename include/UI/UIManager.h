#pragma once

#include <string>
#include <unordered_map>

namespace sf
{
	class RenderWindow;
	class Sprite;
}

class UIScreen;
class Game;

class UIManager
{
public:
	static UIManager* getInstance();

	~UIManager();

	void init(Game* game);
	void addScreen(const std::string& name, UIScreen* screen);
	void setCurrentScreen(const std::string& name, bool pause, bool whitTransition = false);

	void render(sf::RenderWindow& window);
	void update(float deltaMilliseconds, sf::RenderWindow& window);

	UIScreen* getCurrentScreen() { return m_currentUIScreen; }
private:
	UIManager() = default;

	static UIManager* s_instance;

	// Game reference
	Game* m_game{ nullptr };

	// Screens
	std::unordered_map<std::string, UIScreen*> m_screens;
	UIScreen* m_currentUIScreen{ nullptr };

	std::string m_currentScreenName;	
	
	bool m_mousePress = false;


	int m_transition = 0; // -1 out , 0 not transition, 1 in
	sf::Sprite* m_transitionSprite{ nullptr };
	float m_elapsedTime;

	void makeTransitionSprite();
	void startTransition(int mode);
	void updateTransition(float deltaMilliseconds);

};