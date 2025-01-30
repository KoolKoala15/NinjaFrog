#pragma once

#include <Gameplay/Entity.h>
#include <unordered_map>

class Enemy : public Entity
{
public:
	enum class EnemyType
	{
		BaseEnemy,
		Saw,
		Spike,
		Fire
	};

	struct EnemyDescriptor : EntityDescriptor
	{
		sf::Vector2f endPosition;
		EnemyType type = EnemyType::BaseEnemy;
	};
	
	virtual bool init(const EnemyDescriptor& enemyDescriptor);

	// Type
	static std::unordered_map<std::string, Enemy::EnemyType> typeMap;
	static Enemy::EnemyType stringToEnemyType(const std::string& typeStr);
	static std::string enemyTypeToString(Enemy::EnemyType type);


	EnemyType getType() const { return m_type; }

protected:

	EnemyType m_type{ EnemyType::BaseEnemy };

	sf::Vector2f m_startPosition;
	sf::Vector2f m_endPosition;
	float m_progress{ .0f };

};


