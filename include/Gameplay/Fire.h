#pragma once

#include <Gameplay/Enemy.h>

class Fire : public Enemy
{
public:
	bool init(const EnemyDescriptor& enemyDescriptor) override;
	void update(float deltaMilliseconds) override;

private:
	int m_currentState{ 2 };		// 0 Off, 1 heat, 2 Fire
	float m_changeInterval{ .0f }; 

	const float m_offTime = 2.f;
	const float m_heatTime = 1.f;
	const float m_fireTime = 1.f;

	float m_time;

	void changeState();
};