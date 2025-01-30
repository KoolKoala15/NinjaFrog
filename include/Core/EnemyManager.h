#pragma once

#include <vector>
#include <unordered_map>
#include <Gameplay\Enemy.h>

class EnemyManager {

public:
	EnemyManager(std::unordered_map<Enemy::EnemyType, int32_t> enemiesStartingPool);
	~EnemyManager();

	void init(std::unordered_map<std::string, std::vector<std::pair<sf::Vector2f,float>>>& enemyPositions);
	void deinit();

	void update(float deltaMilliseconds);
	void render(sf::RenderWindow& window);
	
	std::vector<Enemy*> getActiveEnemies();

private:
	Enemy::EnemyDescriptor* m_descriptor{ nullptr };
	void setDescriptor(Enemy::EnemyType enemyType);

	std::unordered_map<Enemy::EnemyType, std::vector<Enemy*>> m_pool;
	std::unordered_map<Enemy::EnemyType, std::vector<Enemy*>> m_activeEnemies;

	Enemy* createEnemy(Enemy::EnemyType type);
	void resetEnemy(Enemy* enemy, const sf::Vector2f startPosition, float rotation, const sf::Vector2f endPosition = sf::Vector2f(0,0));
};