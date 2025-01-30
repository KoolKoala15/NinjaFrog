#pragma once

#include <iostream>
#include <Core/Game.h>
#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <Gameplay/Collectable.h>
#include <unordered_map>

namespace tmx
{
	class Map;
	class TileLayer;
	class Layer;
}
class MapLayer;
class ObjectLayer;




class Level
{
public:

	Level(Game::LevelData& levelDescriptor);

	~Level();

	int makeLayers();
	void makePositions();

	void update(float deltaMilliseconds);
	void render(sf::RenderWindow& window);

	bool checkCollision(const sf::FloatRect& shapeToCheck) const;

	tmx::TileLayer* getFruitTileLayer() { return m_fruitTileLayer; }

	const std::uint32_t getFruitFirstGID();

	sf::Vector2f getPlayerStartPosition() { return m_startPlayerPosition; }
	bool hasCheckPoint() { return m_checkPointPosition != sf::Vector2f{ .0f,.0f }; }
	sf::Vector2f getCheckPointPosition() { return m_checkPointPosition; }
	sf::Vector2f getEndPosition() { return m_endPosition; }

	//std::unordered_map<std::string, std::vector<sf::Vector2f>> m_enemyPositions;
	std::unordered_map<std::string, std::vector<std::pair<sf::Vector2f, float>>> m_enemyPositions;
	std::unordered_map<std::string, std::unique_ptr<tmx::Layer>> m_layersMap;

protected:
	sf::Sprite m_backgroundSprite;
	float m_backgroundAnim{ -64.f };

	std::unique_ptr<tmx::Map> m_map{ nullptr };


	std::unique_ptr<MapLayer> m_levelLayer{ nullptr };

	std::vector<std::unique_ptr<sf::Shape>> m_collisionShapes;

	tmx::TileLayer* m_fruitTileLayer{ nullptr };
	std::vector<Collectable*> m_fruits;
	
	sf::Vector2f m_startPlayerPosition{ .0f, .0f };
	sf::Vector2f m_checkPointPosition{ .0f, .0f };
	sf::Vector2f m_endPosition{ .0f, .0f };

	void CreateEnemyPositions(const tmx::TileLayer& tileLayer);
};