#pragma once

#include <Gameplay/Entity.h>
#include <SFML/Graphics/Shape.hpp>
#include <Core/Level.h>

class Player : public Entity
{
public:
	struct PlayerDescriptor : EntityDescriptor
	{
		float jumpForce{ .0f };
		sf::Vector2f maxSpeed{ 0.f , 0.f };
	};

	bool init(const PlayerDescriptor& playerDescriptor);

	void update(float deltaMillisecond) override;

	void updatePlayer(float deltaMilliseconds, Level& level);
	
	void render(sf::RenderWindow& window) override;

	void input();

	void setSpawnPoint(sf::Vector2f position) {
		m_spawnPoint.x = position.x; m_spawnPoint.y = position.y - m_sprite.getLocalBounds().height;
		std::cout << "origin" << m_sprite.getOrigin().y << std::endl;
	}
	void spawn();

	void dead();

	void deactivate() override;

protected:
	float m_jumpForce{ .0f };
	bool m_isGrounded{ false };
	int m_jumpState{ 0 };

	sf::Vector2f m_maxSpeed{0.5f , 0.5f};
	sf::Vector2f m_lastPosition;
	
	void updateAnimation(bool active) override;
	

	

	bool m_canJump;
	bool m_onWall;
	float m_airJumpTime{ .0f };

	sf::Vector2f m_spawnPoint{ .0f, .0f };
};