#include <UI/UIPauseScreen.h>

#include <Core/AssetManager.h>
#include <Core/Game.h>
#include <UI/UIButton.h>
#include <UI/UIManager.h>

UIPauseScreen::~UIPauseScreen() 
{
	deinit();
	delete m_text;
	delete m_backgorundTexture;
	delete m_popupWindowSprite;
}


UIPauseScreen::UIPauseScreen(UIScreenDescriptor& uis, Game* game) :UIMenu(uis, game)
{
	sf::Vector2f windowSize = sf::Vector2f(game->getRenderWindow().getSize());

	// BackGround
	m_backgorundTexture = new sf::Texture();
	m_backgorundTexture->setRepeated(true);
	m_backgorundSprite.setTexture(*m_backgorundTexture);
	m_backgorundSprite.setColor(sf::Color(255, 255, 255, 155));
	m_backgorundSprite.setTextureRect(sf::IntRect(0, 0, windowSize.x, windowSize.y));
	m_backgroundAnimSpeed = .0f;

	// Window
	sf::Texture* popupTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Menus/pauseWindow.png");
	m_popupWindowSprite = new sf::Sprite();
	m_popupWindowSprite->setTexture(*popupTexture);
	m_popupWindowSprite->setScale(0.5f, 0.5f);
	m_popupWindowSprite->setOrigin(m_popupWindowSprite->getLocalBounds().width / 2, m_popupWindowSprite->getLocalBounds().height / 2);
	m_popupWindowSprite->setPosition(windowSize.x / 2, windowSize.y / 2);
	
	// Text
	m_font = AssetManager::getInstance()->loadFont("../Data/Fonts/Levels.ttf");
	m_text = new sf::Text("Pause", *m_font, 100);
	m_text->setFillColor(sf::Color::White);
	m_text->setOrigin(m_text->getGlobalBounds().width / 2.f, m_text->getGlobalBounds().height / 2.f);
	m_text->setPosition(windowSize.x / 2, windowSize.y / 2 - 120);


	// Buttons //
	UIButton::ButtonDescriptor buttonDescriptor;
	// Continue
	buttonDescriptor.normalPathTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Menus/Play.png");
	buttonDescriptor.position = {
		(windowSize.x / 2.f), 
		(windowSize.y / 2.f) + 110 
	};
	buttonDescriptor.scale = { 5.0f, 5.0f };
	buttonDescriptor.font = m_font;
	buttonDescriptor.charSize = 30;
	buttonDescriptor.textPositionAdjustment = { .0f, 2.5f };
	buttonDescriptor.textColor = sf::Color::White;
	buttonDescriptor.textBelow = "Continue";
	buttonDescriptor.function = [game]() {
		UIManager::getInstance()->setCurrentScreen("", false);
		};
	UIButton* continueButton = new UIButton(buttonDescriptor);
	m_buttons.push_back(continueButton);

	// Main Menu
	buttonDescriptor.normalPathTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Menus/Previous.png");
	buttonDescriptor.position = { 
		(windowSize.x / 2.f) - 200,
		(windowSize.y / 2.f) + 110
	};
	buttonDescriptor.textBelow = "Main Menu";
	buttonDescriptor.function = [game]() {
		game->goToMainMenu();
		};
	UIButton* mainMenuButton = new UIButton(buttonDescriptor);
	m_buttons.push_back(mainMenuButton);

	// Reset Level
	buttonDescriptor.normalPathTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Menus/Restart.png");
	buttonDescriptor.position = { 
		(windowSize.x / 2.f) + 200,
		(windowSize.y / 2.f) + 110
	};
	buttonDescriptor.textBelow = "Reset Level";
	buttonDescriptor.function = [game]() {
		game->resetLevel();
		};
	UIButton* resetButton = new UIButton(buttonDescriptor);
	m_buttons.push_back(resetButton);
}



void UIPauseScreen::render(sf::RenderWindow& window)
{
	window.draw(m_backgorundSprite);
	window.draw(*m_popupWindowSprite);
	window.draw(*m_text);
	for (const auto& button : m_buttons)
	{
		button->render(window);
	}
}

void UIPauseScreen::deinit()
{
	AssetManager::getInstance()->unloadTexture(m_popupWindowSprite->getTexture());
	UIMenu::deinit();
}