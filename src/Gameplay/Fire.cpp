#include <Gameplay/Fire.h>


bool Fire::init(const EnemyDescriptor& enemyDescriptor)
{
	m_time = 0;
	return Enemy::init(enemyDescriptor);
	
}

void Fire::update(float deltaMilliseconds)
{
	m_time += deltaMilliseconds * 0.001;

	if (m_time >= m_changeInterval)
	{
		changeState();
		m_time = 0;
	}

	Enemy::update(deltaMilliseconds);
}


void Fire::changeState()
{
	switch (m_currentState)
	{
		case 0:	// Off -> Heat
			m_currentState = 1;
			m_changeInterval = m_heatTime;
			m_tiley = 1;
			deactivate();
			break;
		case 1: // Heat -> Fire
			m_currentState = 2;
			m_changeInterval = m_fireTime;
			m_tiley = 2;
			activate();
			break;
		case 2: // Fire -> Off
			m_currentState = 0;
			m_changeInterval = m_offTime ;
			m_tiley = 0;
			deactivate();
			break;
	}
	m_tilex = 0;
}
