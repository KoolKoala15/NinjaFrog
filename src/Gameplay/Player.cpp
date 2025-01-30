#include <Gameplay/Player.h>

#include <Core/AudioManager.h>
#include <Utils/Constants.h>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include <iostream>

bool Player::init(const PlayerDescriptor& playerDescriptor)
{
	m_jumpForce = playerDescriptor.jumpForce;

	return Entity::init(playerDescriptor);
}


void Player::update(float deltaMilliseconds)
{
	if (!m_isGrounded && m_canJump) { m_airJumpTime += deltaMilliseconds; }
	m_lastPosition = m_positionGO;
	
	if (m_speed.y < m_maxSpeed.y && !m_isGrounded) { m_speed.y += gravity * deltaMilliseconds; }

	input();

}



void Player::updatePlayer(float deltaMilliseconds, Level& level)
{	
	if (m_active) {
		update(deltaMilliseconds);
		m_positionGO.y += (m_speed.y * deltaMilliseconds);
		m_collisionShape.setPosition(m_positionGO);
		if (level.checkCollision(getBounds())) {
			m_positionGO.y = m_lastPosition.y;
			m_speed.y = .0f;
			m_canJump = true;
			m_isGrounded = true;
		}
		else { m_isGrounded = false; if (m_airJumpTime >= 100.f) m_canJump = false; }

		m_positionGO.x += (m_direction.x * m_speed.x * deltaMilliseconds);
		m_collisionShape.setPosition(m_positionGO);
		if (level.checkCollision(getBounds())) {
			//m_positionGO.x = m_lastPosition.x;
			//m_positionGO.x -= m_direction.x * m_speed.x;
			while (level.checkCollision(getBounds())) { m_positionGO.x -= m_direction.x * m_speed.x; m_collisionShape.setPosition(m_positionGO); }

			if (m_isGrounded) {
				m_speed.x = .0f;
				m_onWall = false;
			}
			else {		// Wall on air
				m_speed.y = .0f;
				m_canJump = true;
				m_onWall = true;
				m_airJumpTime = 0;
			}
		}
		else { m_onWall = false; }

		
		//std::cout << "player position : " << m_positionGO.x << ", " << m_positionGO.y << std::endl;
	}
	Entity::update(deltaMilliseconds);
}

void Player::input()
{
	// Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_direction.x = -1.0f;
		if (m_speed.x <  m_maxSpeed.x) { m_speed.x += 0.1; }
		m_tiley = 1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_direction.x = 1.0f;
		if (m_speed.x < m_maxSpeed.x) { m_speed.x += 0.1; }
		m_tiley = 1;
	}
	else
	{
		m_direction.x = .0f;
		if (m_speed.x > 0.1) { m_speed.x -= 0.1; }
		else if (m_speed.x < 0.1) { m_speed.x += 0.1; }
		m_tiley = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (m_canJump && !m_onWall) {
			m_canJump = false;
			//m_direction.y = -m_jumpForce;
			m_speed.y -= m_jumpForce;
			AudioManager::getInstance()->playSound("jump");
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		dead();
	}
}

void Player::updateAnimation(bool active)
{
	if (m_direction.x != .0f) { m_sprite.setScale(m_scale.x * m_direction.x, m_scale.y); m_tiley = 1; }
	else { m_tiley = 0; }

	if(m_onWall){
		m_tiley = 3;
	}
	else {
		if (m_speed.y < -0.1f) {
			m_tiley = 2;
			m_tilex = 0;
		}
		else if (m_speed.y > 0.1f) {
			m_tiley = 2;
			m_tilex = 1;
		}
	}
	
	Entity::updateAnimation(m_visible);
}

void Player::dead()
{
	m_active = false;
	spawn();
	
}

void Player::spawn()
{
	m_speed.x = .0f; m_speed.y = .0f;
	m_active = true;
	setPositionGO(m_spawnPoint);
}



void Player::render(sf::RenderWindow& window)
{
	//Entity::render(window);

	if (m_visible) window.draw(m_sprite);
	
#if DEBUG_MODE
	const sf::FloatRect spriteBounds2 = getBounds();
	sf::RectangleShape boundsRect2(sf::Vector2f(spriteBounds2.width, spriteBounds2.height));
	boundsRect2.setPosition(spriteBounds2.left, spriteBounds2.top);
	boundsRect2.setOutlineColor(m_isGrounded ? sf::Color::Green : sf::Color::Red); 
	boundsRect2.setOutlineThickness(2.f);
	boundsRect2.setFillColor(sf::Color::Transparent);
	window.draw(boundsRect2);
#endif
}

void Player::deactivate()
{
	m_active = false;
	m_tiley = 0;
	m_direction.x = 0;
	m_speed = { .0f, .0f };
}

