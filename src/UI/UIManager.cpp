#include <UI/UIManager.h>

#include <Core/AssetManager.h>
#include <Core/Game.h>
#include <Core/AudioManager.h>
#include <iostream>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Mouse.hpp>
#include <UI/UIScreeen.h>
#include <UI/UIButton.h>

UIManager* UIManager::s_instance = nullptr;


UIManager* UIManager::getInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new UIManager();
	}
	return s_instance;
}

UIManager::~UIManager()
{
	for (auto& pair : m_screens)
	{
		delete pair.second;
	}
	delete m_transitionSprite;
}

void UIManager::init(Game* game)
{
	if (!game)
	{
		throw std::runtime_error("UIManager - Game cannot be null!");
	}
	m_game = game;
	makeTransitionSprite();
}


void UIManager::addScreen(const std::string& name, UIScreen* screen)
{
	if (m_screens.find(name) != m_screens.end())
	{
		throw std::runtime_error("Screen with the name \"" + name + "\" already exists.");
	}
	screen->init(m_game);
	m_screens[name] = screen;
}

void UIManager::setCurrentScreen(const std::string& name, bool pause, bool whitTransition)
{
	m_currentScreenName = name;
	m_game->setPause(pause);
	if (!whitTransition) {
		if (name != "") {
			auto it = m_screens.find(name);
			if (it != m_screens.end())
			{
				std::cout << "Set screen " << name << std::endl;
				m_currentUIScreen = it->second;
				m_currentUIScreen->init(m_game);
			}
			else
			{
				throw std::runtime_error("Screen with the name \"" + name + "\" not found.");
			}
		}
		else { m_currentUIScreen = nullptr; }

	}
	else { startTransition(1); }
}


void UIManager::render(sf::RenderWindow& window)
{
	if (m_currentUIScreen != nullptr)
	{
		m_currentUIScreen->render(window);
	}
	if (m_transition!= 0)
	{
		window.draw(*m_transitionSprite);
	}
}


void UIManager::update(float deltaMilliseconds, sf::RenderWindow& window)
{
	if (m_currentUIScreen != nullptr)
	{
		m_currentUIScreen->update(deltaMilliseconds, window);
		// Mouse Button
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (!m_mousePress) {
				m_mousePress = true;
				UIButton* buttonClicked = m_currentUIScreen->mouseClick(sf::Mouse::getPosition());
				if (buttonClicked) buttonClicked->m_function(); AudioManager::getInstance()->playSound("buttonClick");
			}
		}
		else { m_mousePress = false; }
	}
	if(m_transition!= 0) updateTransition(deltaMilliseconds);
}


void UIManager::makeTransitionSprite()
{
	sf::Texture* transitionTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Menus/Transition.png");
	m_transitionSprite = new sf::Sprite();
	m_transitionSprite->setTexture(*transitionTexture);
	sf::Vector2f size = sf::Vector2f(m_game->getRenderWindow().getSize());
	m_transitionSprite->setPosition(size.x / 2, size.y / 2);
	size = { m_transitionSprite->getLocalBounds().width, m_transitionSprite->getLocalBounds().height };
	m_transitionSprite->setOrigin(size.x / 2, size.y / 2);
}

void UIManager::startTransition(int mode)
{
	m_transition = mode;
	m_elapsedTime = 0;
	if (m_transition < 0) // out
	{
		m_transitionSprite->setScale(16 * 3, 9 * 3);
	}
	else if (m_transition > 0) // in
	{
		m_transitionSprite->setScale(0, 0);
	}
}

void UIManager::updateTransition(float deltaMilliseconds)
{
	m_elapsedTime += deltaMilliseconds;

	float t = std::min(m_elapsedTime / 500.0f, 1.0f);

	float easeValue = t < 0.5 ? 2 * t * t : 1 - std::pow(-2 * t + 2, 2) / 2;

	float currentScale = m_transition > 0 ? easeValue * 3 : 3 - easeValue*3;

	m_transitionSprite->setScale(16 * currentScale, 9 * currentScale);

	// When ends
	if (t == 1.0f) {
		if (m_transition == 1) {
			startTransition(- 1);	// play inverse transition
			setCurrentScreen(m_currentScreenName, m_game->getPause());
		}
		else {
			m_transition = 0;
		}
	}
}