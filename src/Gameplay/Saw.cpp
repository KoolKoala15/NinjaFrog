#include <Gameplay/Saw.h>
#include <iostream>


void Saw::setters(sf::Vector2f& endPosition)
{
	m_startPosition = m_positionGO;
	m_endPosition = endPosition;
}

void Saw::update(float deltaMilliseconds)
{
	m_progress += m_speed.x * deltaMilliseconds / 1000.f;


	if (m_goingToEnd) {
		setPositionGO(lerp(m_startPosition, m_endPosition, m_progress));
		if (m_sprite.getPosition() == m_endPosition) {
			m_goingToEnd = false;
			m_progress = 0.f;
		}
	}
	else {
		setPositionGO(lerp(m_endPosition, m_startPosition, m_progress));
		if (m_sprite.getPosition() == m_startPosition) {
			m_goingToEnd = true;
			m_progress = 0.f;
		}
	}

	Entity::update(deltaMilliseconds);
}