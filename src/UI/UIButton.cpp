#include <UI/UIButton.h>

#include <iostream>
#include <Core/AssetManager.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <vector>

UIButton::UIButton(const ButtonDescriptor& bd)
{
	// Normal
	sf::Texture* buttonTexture =bd.normalPathTexture;
	m_normalSpr.setTexture(*buttonTexture);
	sf::FloatRect* bounds = &(m_normalSpr.getLocalBounds());
	m_normalSpr.setOrigin(bounds->width * 0.5f, bounds->height * 0.5f);
	m_normalSpr.setPosition(bd.position);
	m_normalSpr.setScale(bd.scale);
	// CLicked
	m_hasClickedSprite = (bd.clickedPathTexture != nullptr);
	if (m_hasClickedSprite) {
		buttonTexture = bd.clickedPathTexture;
	}
	m_clickedSpr.setTexture(*buttonTexture);
	m_clickedSpr.setOrigin(bounds->width * 0.5f, bounds->height * 0.5f);
	m_clickedSpr.setPosition(bd.position);
	m_clickedSpr.setScale(bd.scale.x + 0.1f, bd.scale.y + 0.1f);
	m_currentSpr = &m_normalSpr;
	m_current = false;
	if (bd.font != nullptr && bd.words != "") {
		m_text = std::make_unique<sf::Text>(sf::String(bd.words), *bd.font, bd.charSize);
		m_text->setFillColor(bd.textColor);
		sf::FloatRect textBounds = m_text->getGlobalBounds();
		m_text->setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
		sf::Vector2f newPosition = {
			bd.position.x + textBounds.width * bd.textPositionAdjustment.x,
			bd.position.y + textBounds.height * bd.textPositionAdjustment.y
		};
		m_text->setPosition(newPosition);
	}

	if (bd.font != nullptr && bd.textBelow != "") {
		int charSize = bd.charSizeTextBelow != 0 ? bd.charSizeTextBelow : bd.charSize;
		sf::Color color = bd.textBelowColor != sf::Color::Magenta ? bd.textBelowColor : bd.textColor;
		m_textBelow = std::make_unique<sf::Text>(sf::String(bd.textBelow), *bd.font, charSize );
		m_textBelow->setFillColor(color);
		sf::FloatRect textBounds = m_textBelow->getGlobalBounds();
		m_textBelow->setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
		sf::Vector2f posAdjustment = bd.textBelowPositionAdjustment != sf::Vector2f{} ?
			bd.textBelowPositionAdjustment : bd.textPositionAdjustment;

		sf::Vector2f newPosition = {
			bd.position.x + textBounds.width * posAdjustment.x,
			bd.position.y + textBounds.height * posAdjustment.y
		};
		m_textBelow->setPosition(newPosition);
	}


	m_function = bd.function;
	m_buttonNumb = bd.buttonNumber;
}

UIButton::~UIButton()
{
	AssetManager* instance = AssetManager::getInstance();
	instance->unloadTexture(m_normalSpr.getTexture());
	instance->unloadTexture(m_clickedSpr.getTexture());
}

bool UIButton::checkHover(sf::Vector2i& mousePos) 
{
	bool hover = m_currentSpr->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
	setState(hover);
	return hover;
}

void UIButton::render(sf::RenderWindow& window)
{
	window.draw(*m_currentSpr);
	if (m_text) {
		window.draw(*m_text);
	}
	if (m_textBelowAllwaysSaw) {
		window.draw(*m_textBelow);
	}
	else if (m_current && m_textBelow) {
		window.draw(*m_textBelow);
	}

#if DEBUG_MODE
		const sf::FloatRect spriteBounds = m_text->getGlobalBounds();
		sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
		boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
		boundsRect.setOutlineColor(sf::Color::Red);
		boundsRect.setOutlineThickness(2.f);
		boundsRect.setFillColor(sf::Color::Transparent);
		window.draw(boundsRect);
		sf::CircleShape point;
		point.setRadius(4);
		point.setFillColor(sf::Color::Blue);
		point.setPosition(m_text->getPosition());
		window.draw(point);

		const sf::FloatRect spriteBounds2 = m_currentSpr->getGlobalBounds();
		sf::RectangleShape boundsRect2(sf::Vector2f(spriteBounds2.width, spriteBounds2.height));
		boundsRect2.setPosition(spriteBounds2.left, spriteBounds2.top);
		boundsRect2.setOutlineColor(sf::Color::Green);
		boundsRect2.setOutlineThickness(2.f);
		boundsRect2.setFillColor(sf::Color::Transparent);
		window.draw(boundsRect2);
		sf::CircleShape point2;
		point2.setRadius(2);
		point2.setFillColor(sf::Color::Red);
		point2.setPosition(m_currentSpr->getPosition());
		window.draw(point2);
#endif
	
}


void UIButton::setState(bool which) {
	if (!m_locked) {
		m_current = which;
		if (m_current) {
			m_currentSpr = &m_clickedSpr;
			if (m_text) m_text->setScale(1.1, 1.1);
			return;
		}
		m_currentSpr = &m_normalSpr;
		if (m_text) m_text->setScale(1.0, 1.0);
	}
}

void UIButton::setLock(bool lock)
{
	if (lock) { 
		m_locked = true;
		m_normalSpr.setColor(sf::Color(155, 155, 155, 255)); 
		m_textBelow->setCharacterSize(0);
	}
	else { 
		m_locked = false; 
		m_normalSpr.setColor(sf::Color(255, 255, 255, 255)); 
	}
}
