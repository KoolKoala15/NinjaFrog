#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf
{
	class RenderWindow;
}

class GameObject
{
	public:

		GameObject() = default;
		GameObject(float x, float y) : m_positionGO{ x,y } {}
		virtual ~GameObject() = default; // IMPORTANT! If we don't define it as virtual, we will leak memory

		const sf::Vector2f& getPositionGO() const { return m_positionGO; }
		sf::Vector2f& getPositionGO() { return m_positionGO; }
		void setPositionGO(const sf::Vector2f& newPosition) { m_positionGO = newPosition; }

		const sf::Vector2f& getScale() const { return m_scale; }
		sf::Vector2f& getScale() { return m_scale; }
		void setScale(const sf::Vector2f& newScale) { m_scale = newScale; }

		const float getRotation() const { return m_rotation; }
		float getRotation() { return m_rotation; }
		void setRotatio(float newRotation) { m_rotation = newRotation; }

		virtual void update(float delta) = 0;
		virtual void render(sf::RenderWindow& window) = 0;

	protected:

		sf::Vector2f m_positionGO{ .0f, .0f };
		sf::Vector2f m_scale{ 1.0f, 1.0f };
		float m_rotation{ 0.f };
};