#include <Gameplay/Enemy.h>

std::unordered_map<std::string, Enemy::EnemyType> Enemy::typeMap = {
		{"BaseEnemy", Enemy::EnemyType::BaseEnemy},
		{"Saw", Enemy::EnemyType::Saw },
		{"Spike", Enemy::EnemyType::Spike },
		{"Fire", Enemy::EnemyType::Fire },
};

Enemy::EnemyType Enemy::stringToEnemyType(const std::string& typeStr) 
{
	for (const auto& pair : Enemy::typeMap) {
		if (pair.first == typeStr) {
			return pair.second;
		}
	}
	std::cout << "Enmy type not found (" << typeStr << ")" << std::endl;
	return Enemy::EnemyType::BaseEnemy;
}

std::string Enemy::enemyTypeToString(Enemy::EnemyType type) 
{
	for (const auto& pair : Enemy::typeMap) {
		if (pair.second == type) {
			return pair.first;
		}
	}	return"";
}

bool Enemy::init(const EnemyDescriptor& enemyDescriptor)
{
	Entity::init(enemyDescriptor);
	m_startPosition = m_positionGO;
	m_endPosition = enemyDescriptor.endPosition;
	m_type = enemyDescriptor.type;
	return true;
}



