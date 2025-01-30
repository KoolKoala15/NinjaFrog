#include <UI/UIHud.h>

#include <Core/AssetManager.h>
#include <Core/Game.h>
#include <iomanip>
#include <sstream>

UIHud::~UIHud()
{
	delete m_currentLevelText;
	delete m_currentLevelScore;
}



UIHud::UIHud(UIScreenDescriptor& uis, Game* game)
{
	// CurrentLevelText
	sf::Font* font = AssetManager::getInstance()->loadFont("../Data/Fonts/Levels.ttf");
	m_currentLevelText = new sf::Text("Level: 00",*font, 20);
	m_currentLevelText->setColor(sf::Color::White);
	m_currentLevelText->setOrigin(0, 0);
	m_currentLevelText->setPosition(40, 10);

	m_currentLevelScore = new sf::Text("Score: 100 %", *font, 20);
	m_currentLevelScore->setColor(sf::Color::White);
	m_currentLevelScore->setOrigin(m_currentLevelText->getGlobalBounds().width, 0);
	m_currentLevelScore->setPosition(game->getRenderWindow().getSize().x-80, 10);

	m_game = game;
}

void UIHud::init(Game* game)
{
	std::ostringstream ss;
	ss << "Level: " << std::setw(2) << std::setfill('0') << game->getLevelNumb();
	m_currentLevelText->setString(sf::String(ss.str()));
}

void UIHud::updateScore()
{
	float percentage = m_game->getCurrentScore();
	std::ostringstream ss;
	ss << "Score: " << std::setw(3) << std::setfill(' ') << percentage << " %";
	m_currentLevelScore->setString(sf::String(ss.str()));
}



void UIHud::render(sf::RenderWindow& window)
{
	updateScore();
	window.draw(*m_currentLevelText);
	window.draw(*m_currentLevelScore);
}