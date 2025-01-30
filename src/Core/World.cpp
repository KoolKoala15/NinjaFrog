#include <Core/AssetManager.h>
#include <Utils/Constants.h>
#include <Core/World.h>
#include <Core/Level.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Core/ConfigLoader.h>
#include <Core/CollectableManager.h>
#include <Gameplay/Player.h>
#include <iostream>
#include <tmxlite/Map.hpp>
#include <Gameplay/Enemy.h>
#include <Core/EnemyManager.h>
#include <UI/UIManager.h>
#include <UI/UIHud.h>


World::~World()
{
	delete m_checkPoint;
	delete m_endPoint;
	delete m_player;
	delete m_enemyManager;
	delete m_fruitsManager;
	delete m_currentLevel;
}

void World::deinit()
{
	if (m_currentLevel) delete m_currentLevel; m_currentLevel = nullptr;

	if (m_player) delete m_player; m_player = nullptr;

	if (m_endPoint) delete m_endPoint; m_endPoint = nullptr;

	if (m_checkPoint) delete m_checkPoint; m_checkPoint = nullptr;

	if (m_fruitsManager) m_fruitsManager->deinit();

	if (m_enemyManager) m_enemyManager->deinit();
}




bool World::init(Game::LevelData& levelData)
{
	bool initOk = true;
	// ##########      Level         ##########
	m_currentLevel = new Level(levelData);

	// ##########	Collectabes - Fruits	##########
	m_fruitsManager = createCollectableManager(20);

	// ##########		 Enemies		##########
	m_enemyManager = createEnemyManager(2, 6, 6);	// Saws, Spiles, Fires
	

	// ##########		Player		##########
	createPlayer(initOk);
	m_player->setSpawnPoint(m_currentLevel->getPlayerStartPosition());
	m_player->spawn();
	// ##########		EndPoint		##########
	createEndPoint(initOk);
	// ##########		CheckPoint		##########
	if (m_currentLevel->hasCheckPoint()) CreateCheckPoint(initOk);

	// ##########		Score		##########
	m_currentLevelScore = 0.0f;

	m_levelOn = true;
	return initOk;
}


CollectableManager* World::createCollectableManager(int numb)
{
	CollectableManager* collectableManager = new CollectableManager(numb);
	collectableManager->init(m_currentLevel->getFruitTileLayer(), m_currentLevel->getFruitFirstGID());
	return collectableManager;	
}

EnemyManager* World::createEnemyManager(int sawNumb, int spikeNumb, int fireNumb)
{
	std::unordered_map<Enemy::EnemyType, int32_t> enemiesStartingPool = {
		   {Enemy::EnemyType::Saw, sawNumb },
		   {Enemy::EnemyType::Spike, spikeNumb },
		   {Enemy::EnemyType::Fire, fireNumb }
	};
	EnemyManager* enemyManager = new EnemyManager(enemiesStartingPool);
	enemyManager->init(m_currentLevel->m_enemyPositions);
	return enemyManager;
}

void World::createPlayer(bool& initOk)
{
	Player::PlayerDescriptor playerDescriptor;
	if (!ConfigLoader::loadEntityDescriptor("../Data/Config/player_config.txt", playerDescriptor)) {
		std::cerr << "Error: Failed to load the player's configuration -" << std::endl;
		initOk = false;
	}
	Player* player = new Player();
	initOk = player->init(playerDescriptor);
	if (!initOk) { std::cerr << "Error: Failed to initialize the player-" << std::endl; delete player; player = nullptr; };
	m_player = player;
}

void World::createEndPoint(bool& initOk)
{
	Entity::EntityDescriptor entityDescriptor;
	if (!ConfigLoader::loadEntityDescriptor("../Data/Config/endPoint_config.txt", entityDescriptor)) {
		std::cerr << "Error: Failed to load the endPoint's configuration -" << std::endl;
		initOk = false;
	}
	Entity* endPoint = new Entity();
	entityDescriptor.position = m_currentLevel->getEndPosition();
#if DEBUG_MODE
	std::cout << "endpPoint pos: " << m_currentLevel->getEndPosition().x << ", " << m_currentLevel->getEndPosition().y << std::endl;
#endif
	initOk = endPoint->init(entityDescriptor);
	m_endPoint = endPoint;
}

void World::CreateCheckPoint(bool& initOk)
{
	Entity::EntityDescriptor entityDescriptor;
	if (!ConfigLoader::loadEntityDescriptor("../Data/Config/checkPoint_config.txt", entityDescriptor)) {
		std::cerr << "Error: Failed to load the checkpoints's configuration -" << std::endl;
		initOk = false;
	}
	Entity* checkPoint = new Entity();
	entityDescriptor.position = m_currentLevel->getCheckPointPosition();
	initOk = checkPoint->init(entityDescriptor);
	m_checkPoint = checkPoint;
}




void World::ChangeLevel(Game::LevelData& newLevelDescriptor)
{
	std::cout << "World :: changing level " << std::endl;
	if (m_currentLevel) {
		delete m_currentLevel;
		m_currentLevel = nullptr;
	}

	m_currentLevel = new Level(newLevelDescriptor);
	m_levelOn = true;
	bool initOk = true;

	// ##########		Player		##########
	if (!m_player) {
		createPlayer(initOk);
	}
	m_player->setSpawnPoint(m_currentLevel->getPlayerStartPosition());
	m_player->spawn();

	// ##########		EndPoint		##########
	if (!m_endPoint) {
		createEndPoint(initOk);
	}
	else {
		m_endPoint->setPositionGO(m_currentLevel->getEndPosition());
	}

	// ##########		CheckPoint		##########
	if (m_currentLevel->hasCheckPoint())
	{
		if (!m_checkPoint) {
			CreateCheckPoint(initOk);
		}
		m_checkPoint->activate();
		m_checkPoint->setPositionGO(m_currentLevel->getCheckPointPosition());
		m_checkPoint->setTileY(0);
		m_checkPoint->setTileX(0);
	}
	else {
		if (m_checkPoint) {
			m_checkPoint->deactivate();
			m_checkPoint->setPositionGO(sf::Vector2f{ .0f,.0f });
		}
	}

	// ##########		Fruits		##########	
	m_fruitsManager->deinit();
	m_fruitsManager->init(m_currentLevel->getFruitTileLayer(), m_currentLevel->getFruitFirstGID());

	// ##########		Enemys		##########	
	m_enemyManager->deinit();
	m_enemyManager->init(m_currentLevel->m_enemyPositions);

	// ##########		Score		##########
	m_currentLevelScore = 0.0f;
}


void World::update(uint32_t deltaMilliseconds)
{
	if (m_currentLevel) {
		m_currentLevel->update(deltaMilliseconds);

		m_fruitsManager->update(deltaMilliseconds);

		if (m_checkPoint) {
			m_checkPoint->update(deltaMilliseconds);
			if (m_checkPoint->getTileY() == 1 && m_checkPoint->getTileX() == m_checkPoint->getMaxTileX()) {
				m_checkPoint->setTileY(2); m_checkPoint->setTileX(0);
			}
		}

		m_player->updatePlayer(deltaMilliseconds, *m_currentLevel);

		m_enemyManager->update(deltaMilliseconds);

		if (m_endPoint->getTileX() != m_endPoint->getMaxTileX())
		{
			m_endPoint->update(deltaMilliseconds);
		}

		checkCollisions();
	}
}


void World::checkCollisions()
{
	if (m_player->isActive()) {

		// Player and Fruits
		for (Collectable* collectable : m_fruitsManager->getActiveCollectables())
		{
			if (collectable->isActive()) {
				if (m_player->getBounds().intersects(collectable->getBounds()))
				{
					m_fruitsManager->removeCollectable(collectable);
					m_currentLevelScore = m_fruitsManager->getCollectablePercentage();
				}
			}
		}

		// Player and Enemies
		for (Enemy* enemy : m_enemyManager->getActiveEnemies())
		{
			if (enemy->isActive()) {
				if (m_player->getBounds().intersects(enemy->getBounds())) {
					m_player->dead();
				}
			}
		}
		
		// Player and Checkpoint
		if (m_checkPoint && m_checkPoint->isActive()) {
			if (m_player->getBounds().intersects(m_checkPoint->getBounds()))
			{
				if (m_checkPoint->getTileY() == 0)
				{
					m_checkPoint->setTileY(1);
					m_checkPoint->setTileX(1);
					m_player->setSpawnPoint(m_currentLevel->getCheckPointPosition());
				}
			}
		}

		// Player and EndPoint
		if (m_player->getBounds().intersects(m_endPoint->getBounds()))
		{
			if (m_player->isActive())
			{
				m_player->deactivate();
				m_endPoint->setTileX(0);
				m_levelOn = false;
				UIManager::getInstance()->setCurrentScreen("LevelCompleted", true);
			}
		}
	}
}





void World::render(sf::RenderWindow& window)
{
	if (m_currentLevel) {
		m_currentLevel->render(window);

		m_fruitsManager->render(window);

		if (m_checkPoint) m_checkPoint->render(window);

		m_endPoint->render(window);

		m_enemyManager->render(window);

		m_player->render(window);
	}
}
