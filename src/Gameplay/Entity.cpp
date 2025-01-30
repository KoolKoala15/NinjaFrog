#include <Core/AssetManager.h>
#include <Gameplay/Entity.h>
#include <Utils/Constants.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

bool Entity::init(const EntityDescriptor& ed)
{
	m_positionGO = ed.position;
	m_tileWidth = ed.tileWidth;
	m_tileHeight = ed.tileHeight;

	m_scale = ed.scale;

	if(ed.texture)m_sprite.setTexture(*ed.texture);

	m_sprite.setRotation(ed.rotation);
	m_sprite.setOrigin(
		ed.origin.x * ed.tileWidth,
		ed.origin.y * ed.tileHeight
	);
	m_sprite.setPosition(m_positionGO);
	m_sprite.setScale(m_scale);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_tileWidth, m_tileHeight));
	
	m_maxTileX = ed.maxTilesX;
	m_animationSpeed = ed.animationSpeed;
	//m_animationSpeed = entityDescriptor.animationSpeed * (1/1000.f);

	m_speed = ed.speed;

	m_collisionShape = sf::RectangleShape(ed.collisionShapeSize);
	m_collisionShape.setRotation(ed.rotation);
	m_collisionShape.setOrigin(
		ed.collisionShapeOrigin.x * ed.collisionShapeSize.x,
		ed.collisionShapeOrigin.y * ed.collisionShapeSize.y
	);
	m_collisionShape.setPosition(m_positionGO);
	m_collisionShape.setScale(m_scale);

	m_maxTilePerState = ed.maxTilePerState;

	return true;
}

void Entity::update(float deltaMilliseconds)
{
	setPositionGO(m_positionGO);
	m_sprite.setPosition(m_positionGO);
	m_collisionShape.setPosition(m_positionGO);

	m_elapsedTime += deltaMilliseconds * millisecondsToSeconds;
	if (m_elapsedTime >= m_animationSpeed) {
		m_elapsedTime -= m_animationSpeed;
		updateAnimation(m_visible);
	}

}

void Entity::updateAnimation(bool active)
{
	if (active) {
		if (!m_maxTilePerState.empty())m_maxTileX = m_maxTilePerState[m_tiley];
		m_sprite.setTextureRect(sf::IntRect(m_tilex * m_tileWidth, m_tiley * m_tileHeight, m_tileWidth, m_tileHeight));
		m_tilex = (m_tilex + 1) % (m_maxTileX + 1);
	}
}

void Entity::render(sf::RenderWindow& window)
{
	if(m_visible) window.draw(m_sprite);

#if DEBUG_MODE
	const sf::FloatRect spriteBounds2 = m_sprite.getGlobalBounds();
	sf::RectangleShape boundsRect2(sf::Vector2f(spriteBounds2.width, spriteBounds2.height));
	boundsRect2.setPosition(spriteBounds2.left, spriteBounds2.top);
	boundsRect2.setOutlineColor(sf::Color::Green);
	boundsRect2.setOutlineThickness(2.f);
	boundsRect2.setFillColor(sf::Color::Transparent);
	window.draw(boundsRect2);

	const sf::FloatRect spriteBounds = getBounds();
	sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
	boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
	boundsRect.setOutlineColor(m_active ? sf::Color::Red : sf::Color::Black);
	boundsRect.setOutlineThickness(2.f);
	boundsRect.setFillColor(sf::Color::Transparent);
	window.draw(boundsRect);
#endif
}

Entity::~Entity()
{
	unloadTexture();
}

void Entity::unloadTexture()
{
	AssetManager::getInstance()->unloadTexture(m_sprite.getTexture());
}
