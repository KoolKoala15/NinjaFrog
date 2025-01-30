#include <cassert>
#include <Core/ConfigLoader.h>
#include <Core/Game.h>
#include <Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <UI/UILevelCompleted.h>
#include <UI/UILevelSelector.h>
#include <UI/UIManager.h>
#include <UI/UIPauseScreen.h>
#include <UI/UIScreenMainMenu.h>
#include <UI/UIHud.h>

bool Game::init(GameCreateInfo& createInfo)
{
	assert(m_window == nullptr && m_world == nullptr && "Game is already initialized, we are about to leak memory");

	// ##########		RenderWindow		##########
	m_window = new sf::RenderWindow({ createInfo.screenWidth, createInfo.screenHeight }, createInfo.gameTitle);
	m_window->setFramerateLimit(createInfo.frameRateLimit);
	
	// ##########	Load Sounds		##########
	ConfigLoader::loadSounds("../data/Sound/SoundEffectsList.txt");
	

	// ##########	Save Load		##########
	ConfigLoader::loadSave(m_saveData);
	std::cout << "maxLevel save data: " << m_saveData.maxLevel << std::endl;
	for (auto& level : m_saveData.levelScores) {
		std::cout << level.first << " : " << level.second << std::endl;
	}


	// ##########	Levels Load		##########
	if (!ConfigLoader::loadLevelsData("../Data/Config/levels_config.txt", m_levels))
	{std::cerr << "Error loading levels " << std::endl; }

	if (m_saveData.maxLevel != 1) {
		for (int i = 1; i < m_saveData.maxLevel +1; ++i)
		{
			m_levels[i].unlocked = true;
		}
	}
	m_lastLevel = m_levels.size();

	std::cerr << "List of loaded levels: " <<m_levels.size()<< std::endl;
	for (int i = 1; i < m_levels.size() + 1; ++i)
	{
		std::cerr << i << " // Level numb: " << m_levels[i].levelNumber << " // Level path: " << m_levels[i].levelPath << " // unloked: " << m_levels[i].unlocked << " // background" << m_levels[i].backgroundTexturePath << std::endl;
	}


	// ##########	UIManager		##########
	UIManager* uiManager = UIManager::getInstance();
	m_uiManager = uiManager;
	m_uiManager->init(this);

	CreateMenus();

	CreateHuds();

	m_uiManager->setCurrentScreen("MainMenu", true);


	return true;
}

void Game::CreateHuds()
{
	// ------	PauseScreen	------
	UIMenu::UIMenuDescriptor emptyDescriptor{};
	m_uiManager->addScreen("PauseScreen", new UIPauseScreen(emptyDescriptor, this));

	// ------	LevelCompletedScreen	------
	m_uiManager->addScreen("LevelCompleted", new UILevelCompleted(emptyDescriptor, this));

	// ------		Hud		------
	m_uiManager->addScreen("Hud", new UIHud(emptyDescriptor, this));
}

void Game::CreateMenus()
{
	// ------	MainMenu	------
	UIMenu::UIMenuDescriptor menuDescription{};
	if (!ConfigLoader::loadScreenDescriptor("../Data/Config/UIMenu_config.txt", menuDescription)) {
		std::cerr << "Error: Failed to load the menu configuration -" << std::endl;
	}
	m_uiManager->addScreen("MainMenu", new UIScreenMainMenu(menuDescription, this));

	// ------	LevelSelector	------
	UILevelSelector::LevelSelectorDescriptor levelSelectorDescription{};
	if (!ConfigLoader::loadScreenDescriptor("../Data/Config/UILevelSelector_config.txt", levelSelectorDescription)) {
		std::cerr << "Error: Failed to load the level menu configuration -" << std::endl;
	}
	levelSelectorDescription.totalButtons = m_levels.size();
	m_uiManager->addScreen("LevelSelector", new UILevelSelector(levelSelectorDescription, this));
}

void Game::openLevel(int levelNumber)
{
	if (levelNumber == -1) levelNumber = m_levelNumb;
	if(levelNumber == m_lastLevel) m_uiManager->setCurrentScreen("MainMenu", true, true);
	else {
		m_inMainMenu = false;
		setLevelNumber(levelNumber);
		m_uiManager->setCurrentScreen("Hud", false, false);
		std::cout << "Game: Opening level: " << levelNumber << std::endl;
		if (!m_world) {
			m_world = new World();
		}
		if (m_world->levelLoaded())
		{
			m_world->ChangeLevel(m_levels[levelNumber]);
		}
		else {
			m_world->init(m_levels[levelNumber]);
		}
	}
}


void Game::setLevelNumber(int levelNumber)
{
	m_levelNumb = levelNumber;
	if (m_levelNumb > m_saveData.maxLevel) {
		m_saveData.maxLevel = m_levelNumb;
	}
}

void Game::closeGame()
{
	ConfigLoader::makeSave(m_saveData);
	m_window->close();
}


Game::~Game()
{
	// To-Do: make sure m_world is unloaded()
	delete m_uiManager;
	delete m_world;
	delete m_window	;
}

bool Game::isRunning() const 
{ 
	return m_window->isOpen(); 
}

void Game::update(uint32_t deltaMilliseconds)
{
	if (m_world && !m_inMainMenu) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
			if (!m_pauseKeyPresed && m_world->m_levelOn) {
				m_pauseKeyPresed = true;
				togglePause();
			}
		}
		else { m_pauseKeyPresed = false; }

		if(!m_pause) m_world->update(deltaMilliseconds);
	}

	m_uiManager->update(deltaMilliseconds, *m_window);
}
void Game::togglePause()
{
	if (m_pause) {
		m_uiManager->setCurrentScreen("", false);
	}
	else { 
		m_uiManager->setCurrentScreen("PauseScreen", true);
	}
}

void Game::render()
{
	m_window->clear();

	if(m_world && !m_inMainMenu) m_world->render(*m_window);

	m_uiManager->render(*m_window);

	m_window->display();
}


void Game::resetLevel()
{ 
	openLevel(m_levelNumb); 
}

void Game::goToMainMenu()
{
	m_inMainMenu = true;
	deleteWorld();
	m_uiManager->setCurrentScreen("MainMenu", true);
}

void Game::goToLevelMenu()
{
	m_inMainMenu = true;
	deleteWorld();
	m_uiManager->setCurrentScreen("LevelSelector", true);
}

void Game::deleteWorld()
{
	m_world->deinit();
	delete m_world;
	m_world = nullptr;
}

float Game::getCurrentScore()
{
	if (m_world) {
		return m_world->m_currentLevelScore;
	}
	else {
		return 0.0f;
	}
}

void Game::passLevel()
{
	m_saveData.levelScores[m_levelNumb] = getCurrentScore();
	setLevelNumber(m_levelNumb + 1);
	for (auto& level : m_saveData.levelScores) {
		std::cout << level.first << " : " << level.second << std::endl;
	}
}