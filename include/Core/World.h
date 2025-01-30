#pragma once

#include <Gameplay/GameObject.h>
#include <Core/Level.h>
#include <SFML/Graphics/Sprite.hpp>
#include <cstdint>
#include <vector>

class Player;
class EnemyManager;
class CollectableManager;
class CollectableManager;
class Entity;
class Enemy;

namespace sf {
	class RenderWindow;
	class CircleShape;
};



class World
{
public:

	~World();

	bool init(Game::LevelData& levelData);
	void deinit();

	void ChangeLevel(Game::LevelData& newLevelDescriptor);

	void update(uint32_t deltaMilliseconds);
	void render(sf::RenderWindow& window);

	bool levelLoaded() { return m_currentLevel != nullptr; };

	bool m_levelOn = true;
	float m_currentLevelScore{ .0f };
private:

	Player* m_player{ nullptr };
	Entity* m_checkPoint{ nullptr };
	Entity* m_endPoint{ nullptr };
	Level* m_currentLevel{ nullptr };
	CollectableManager* m_fruitsManager{ nullptr };

	EnemyManager* m_enemyManager{ nullptr };


	void checkCollisions();

	CollectableManager* createCollectableManager(int numb);
	EnemyManager* createEnemyManager(int sawNumb, int spikeNumb, int fireNumb);

	void createPlayer(bool& initOk);
	void createEndPoint(bool& initOk);
	void CreateCheckPoint(bool& initOk);
};