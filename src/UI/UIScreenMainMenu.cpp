#include <UI/UIScreenMainMenu.h>

#include <Core/AssetManager.h>
#include <Core/Game.h>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <UI/UIButton.h>
#include <UI/UIManager.h>
#include <functional>


UIScreenMainMenu::~UIScreenMainMenu()
{
	AssetManager::getInstance()->unloadTexture(m_logo->getTexture());
	delete m_logo;
	delete m_font;
}

UIScreenMainMenu::UIScreenMainMenu(UIScreenDescriptor& uis, Game* game):UIMenu(uis, game)
{
	sf::Vector2f windowSize = sf::Vector2f(game->getRenderWindow().getSize());

	m_font = m_font = AssetManager::getInstance()->loadFont("../Data/Fonts/Levels.ttf");

	// Logo
	m_logo = new sf::Sprite();
	sf::Texture* texture = AssetManager::getInstance()->loadTexture("../Data/Images/Menus/Logo.png");
	m_logo->setTexture(*texture);
	m_logo->setOrigin(m_logo->getGlobalBounds().width / 2, m_logo->getGlobalBounds().height / 2);
	m_logo->setScale(3.5, 3.5);
	m_logo->setPosition((windowSize.x / 2.f), (windowSize.y / 2.f) - 300);

	// Buttons //
	UIButton::ButtonDescriptor buttonDescriptor;
	// Start
	buttonDescriptor.normalPathTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Menus/Play.png");
	buttonDescriptor.position = { (windowSize.x / 2.f), (windowSize.y / 2.f) };
	buttonDescriptor.scale = { 3.0f, 3.0f };
	buttonDescriptor.font = m_font;
	buttonDescriptor.charSize = 30;
	buttonDescriptor.textPositionAdjustment = { .0f, 2.5f };
	buttonDescriptor.textColor = sf::Color::White;
	buttonDescriptor.textBelow = "Start";
	buttonDescriptor.function = [game]() {
		game->openLevel(game->m_saveData.maxLevel);
		};
	UIButton* playButton = new UIButton(buttonDescriptor);
	m_buttons.push_back(playButton);


	// Levesl
	buttonDescriptor.normalPathTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Menus/Levels.png");
	buttonDescriptor.position = { (windowSize.x / 2.f) - 200, (windowSize.y / 2.f) + 10};
	buttonDescriptor.textBelow = "Levels";
	buttonDescriptor.function = []() {
		UIManager::getInstance()->setCurrentScreen("LevelSelector", true, true);
		};
	UIButton* levelsButton = new UIButton(buttonDescriptor);
	m_buttons.push_back(levelsButton);


	//Exit
	buttonDescriptor.normalPathTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Menus/Close.png");
	buttonDescriptor.position = { (windowSize.x / 2.f) + 200, (windowSize.y / 2.f) + 10 };
	buttonDescriptor.textBelow = "Exit";
	buttonDescriptor.function = [game]() {
		game->closeGame();
		};
	UIButton* exitButton = new UIButton(buttonDescriptor);
	m_buttons.push_back(exitButton);

}

void UIScreenMainMenu::render(sf::RenderWindow& window)
{
	UIMenu::render(window);
	window.draw(*m_logo);
}