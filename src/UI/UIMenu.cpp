#include <UI/UIMenu.h>

#include <Core/AssetManager.h>
#include <Core/Game.h>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <UI/UIButton.h>


UIMenu::UIMenu(UIScreenDescriptor& uis, Game* game)
{
	const UIMenuDescriptor* uiMenuDescriptor = static_cast<UIMenuDescriptor*>(&uis);
	sf::Vector2f windowSize = sf::Vector2f(game->getRenderWindow().getSize());
	
	if (uiMenuDescriptor->backgroundTexture) {
		uiMenuDescriptor->backgroundTexture->setRepeated(true);
		m_backgorundSprite.setTexture(*uiMenuDescriptor->backgroundTexture);
		m_backgorundSprite.setTextureRect(sf::IntRect(0, -64, windowSize.x, windowSize.y + 64));
	}
	m_backgorundSprite.setScale(uiMenuDescriptor->backgroundScale);

}

void UIMenu::update(float deltaMilliseconds, sf::RenderWindow& window)
{
	m_backgroundAnim += m_backgroundAnimSpeed * deltaMilliseconds * 0.01f;
	if (m_backgroundAnim >= 64.f) { m_backgroundAnim = .0f; }
	m_backgorundSprite.setPosition(0, m_backgroundAnim - 64);

	for (const auto& button : m_buttons)
	{
		button->checkHover(sf::Mouse::getPosition(window));
	}
}

void UIMenu::render(sf::RenderWindow& window)
{
	window.draw(m_backgorundSprite);
	for (const auto& button : m_buttons)
	{
		button->render(window);
	}
}

UIButton* UIMenu::mouseClick(sf::Vector2i& mousePos)
{
	for (const auto& button : m_buttons)
	{
		if (button != nullptr) {
			if (button->m_current)
			{
				return button;
			}
		}
	}
	return nullptr;
}


UIMenu::~UIMenu() {
	for (const auto& button : m_buttons)
	{
		delete button;
	}
	m_buttons.clear();
}
