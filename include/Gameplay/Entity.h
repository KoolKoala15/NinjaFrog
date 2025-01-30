#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics.hpp>


class Entity : public GameObject 
{
public: 
	struct EntityDescriptor
	{
		sf::Vector2f position{ 0.0f , 0.0f };
		sf::Texture* texture{ nullptr };
		int tileWidth{ 0 };
		int tileHeight{ 0 };
		int maxTilesX{ 0 };
		float animationSpeed{ .0f };
		sf::Vector2f scale{ 1.0f,1.0f };
		sf::Vector2f speed{ .0f, .0f };
		sf::Vector2f origin{ 0.5f, 0.5f };
		sf::Vector2f collisionShapeSize{ 32.0f, 32.0f };
		sf::Vector2f collisionShapeOrigin{ 0.5f, 0.5f };
		std::vector<int> maxTilePerState;
		bool hasManager = false;
		float rotation = 0;
	};

	virtual ~Entity();

	void unloadTexture();

	bool init(const EntityDescriptor& entityDescriptor);

	sf::FloatRect getBounds() const { return m_collisionShape.getGlobalBounds(); }

	virtual void update(float deltaMilliseconds) override;
	virtual void render(sf::RenderWindow& window) override;

	void activate() { m_active = true;}
	virtual void deactivate() { m_active = false; }
	bool isActive() const { return m_active; }

	void SetVisibility(bool visible) { m_visible = visible; }
	bool isVisible() const { return m_visible; }

	virtual void updateAnimation(bool active);

	int getTileY() { return m_tiley; }
	void setTileY(int new_tiley) { m_tiley = new_tiley; }
	int getTileX() { return m_tilex; }
	void setTileX(int new_tilex) { m_tilex = new_tilex; }
	int getMaxTileX() { return m_maxTileX; }


protected:

	sf::Sprite m_sprite;
	sf::RectangleShape m_collisionShape;

	sf::Vector2f m_speed;
	sf::Vector2f m_direction;

	float m_tileWidth{ .0f };
	float m_tileHeight{ .0f };
	int m_tilex{ 0 };
	int m_tiley{ 0 };
	int m_maxTileX{ 0 };

	float m_elapsedTime{ .0f };
	float m_animationSpeed{ .0f };
	std::vector<int> m_maxTilePerState;

	bool m_active = true;
	bool m_visible = true;
	bool m_hasManager = false;
};