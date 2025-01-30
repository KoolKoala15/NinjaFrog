#pragma once

#include <Gameplay/Enemy.h>

class Spike : public Enemy
{
public:
	bool init(const EnemyDescriptor& enemyDescriptor) override;
	void update(float deltaMilliseconds) override ;
};