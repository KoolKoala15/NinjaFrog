#include <Core/EnemyManager.h>
#include <Core/ConfigLoader.h>
#include <Gameplay/saw.h>
#include <Gameplay/spike.h>
#include <Gameplay/fire.h>
#include <iostream>
#include <random>
#include <Utils/Constants.h>
#include <vector>


EnemyManager::~EnemyManager()
{
	delete m_descriptor;
	for (auto& pair : m_activeEnemies)
	{
		for (Enemy* enemy : pair.second)
		{
			delete enemy;
		}
	}
	m_activeEnemies.clear();
	for (auto& pair : m_pool)
	{
		for (Enemy* enemy : pair.second)
		{
			delete enemy;
		}
	}
	m_pool.clear();
}


EnemyManager::EnemyManager(std::unordered_map<Enemy::EnemyType, int32_t> enemiesStartingPool)
{
	for (auto it = enemiesStartingPool.begin(); it != enemiesStartingPool.end(); ++it) {
		int count = 0;
		for (int i = 0; i < it->second; ++i) {
			count += 1;
			Enemy* enemy = createEnemy(it->first);
			m_pool[it->first].push_back(enemy);
		}
	}
}



void EnemyManager::init(std::unordered_map<std::string, std::vector<std::pair<sf::Vector2f,float>>>& enemyPositions)
{
	for (const auto [enemyName, positions] : enemyPositions) 
	{
		std::cout << "name: " << enemyName << std::endl;
		Enemy::EnemyType enemyType = Enemy::stringToEnemyType(enemyName);
		if ( enemyType != Enemy::EnemyType::BaseEnemy) 
		{
			size_t enemyNum = (enemyType == Enemy::EnemyType::Saw) ? positions.size() / 2 : positions.size();

			for (size_t i = 0; i < enemyNum; ++i) 
			{
				Enemy* enemy{ nullptr };

				auto it = m_pool.find(enemyType);  
				if (it != m_pool.end() && !it->second.empty()) {  
					enemy = it->second.back();
					it->second.pop_back();
				}
				else {
					enemy = createEnemy(enemyType);
				}

				sf::Vector2f startPosition = positions[i].first;
				float rotation = positions[i].second;

				sf::Vector2f endPosition = (enemyType == Enemy::EnemyType::Saw) ? positions[i + enemyNum].first : sf::Vector2f{ 0.f, 0.f };
				std::cout << "a";
				resetEnemy(enemy, startPosition, rotation, endPosition);
				m_activeEnemies[enemyType].push_back(enemy);
			}
		}
	}
//#if DEBUG_MODE
	std::cout << " ///////////////////// EnemyManager Pools  ////////////////////////" << std::endl;
	std::cout << " __ Active Enemies __  " << std::endl;
	for (auto& pair : m_activeEnemies)
	{
		for (Enemy* enemy : pair.second)
		{
			std::cout << Enemy::enemyTypeToString(enemy->getType()) << " / position :" << enemy->getPositionGO().x << " , " << enemy->getPositionGO().y << std::endl;
		}
	}
	std::cout << " __ Pool Enemies __  " << std::endl;
	for (auto& pair : m_pool)
	{
		for (Enemy* enemy : pair.second)
		{
			std::cout << Enemy::enemyTypeToString(enemy->getType()) << " / position :" << enemy->getPositionGO().x << " , " << enemy->getPositionGO().y << std::endl;
		}
	}
	std::cout << " /////////////////////////////////////////////////////////////////  " << std::endl;
//#endif
}


Enemy* EnemyManager::createEnemy(const Enemy::EnemyType type)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	Enemy* enemy{ nullptr };
	switch (type)
	{
	case Enemy::EnemyType::Saw:
		enemy = new Saw();
		break;
	case Enemy::EnemyType::Spike:
		enemy = new Spike();
		break;
	case Enemy::EnemyType::Fire:
		enemy = new Fire();
		break;
	}

	setDescriptor(type);

	std::uniform_int_distribution<> dist(0, m_descriptor->maxTilesX);
	enemy->init(*m_descriptor);
	enemy->deactivate();
	enemy->setTileX(dist(gen));
	
	return enemy;
}
void EnemyManager::setDescriptor(Enemy::EnemyType enemyType)
{
	if (m_descriptor == nullptr || m_descriptor->type != enemyType)
	{
		delete m_descriptor;
		m_descriptor = new Enemy::EnemyDescriptor;
		if (!ConfigLoader::loadEntityDescriptor(("../Data/config/enemy_" + Enemy::enemyTypeToString(enemyType) + "_config.txt"), *m_descriptor)) {
			std::cerr << "Error loading config for enemy " << Enemy::enemyTypeToString(enemyType) << std::endl;
		}
		else { std::cout << "Enemy " << Enemy::enemyTypeToString(enemyType) << " succesfully load" << std::endl; }
	}
}


void EnemyManager::resetEnemy(Enemy* enemy, sf::Vector2f startPosition, float rotation, sf::Vector2f endPosition )
{
	setDescriptor(enemy->getType());
	Enemy::EnemyDescriptor descriptor = *m_descriptor;
	descriptor.position = startPosition;
	descriptor.rotation = rotation;
	if (m_descriptor->type == Enemy::EnemyType::Saw) descriptor.endPosition = endPosition;

	enemy->init(descriptor);
	enemy->activate();

}

void EnemyManager::deinit()
{
	for (auto& pair : m_activeEnemies)
	{
		for (Enemy* enemy : pair.second) {
			enemy->deactivate();
			m_pool[pair.first].push_back(enemy);
		}
	}

	m_activeEnemies.clear();

	for (auto& pair : m_pool)
	{
		for (Enemy* enemy : pair.second)
		{
			enemy->deactivate();
		}
	}
}



void EnemyManager::update(float deltaMilliseconds)
{
	for (auto& pair : m_activeEnemies) {
		for (Enemy* enemy : pair.second)
		{
			enemy->update(deltaMilliseconds);
		}
	}
}

void EnemyManager::render(sf::RenderWindow& window)
{
	for (auto& pair : m_activeEnemies) {
		for (Enemy* enemy : pair.second)
		{
			if(enemy->isVisible()) enemy->render(window);
		}
	}
}

std::vector<Enemy*> EnemyManager::getActiveEnemies()
{
	std::vector<Enemy*> allActiveEn;
	for (const auto& pair : m_activeEnemies) {
		allActiveEn.insert(allActiveEn.end(), pair.second.begin(), pair.second.end());
	}
	return allActiveEn;
}

