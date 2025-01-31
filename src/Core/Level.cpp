#include <Core/Level.h>
#include <Core/AssetManager.h>
#include <Utils/Constants.h>
#include <Render/SFMLOrthogonalLayer.h>
#include <tmxlite/Map.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/Object.hpp>
#include <tmxlite/Layer.hpp>

#include <bitset>


Level::~Level() {

	AssetManager::getInstance()->unloadTexture(m_backgroundSprite.getTexture());
	
	m_collisionShapes.clear();
}

Level::Level(Game::LevelData& levelDescriptor)
{

	// ##########		Map			##########
	m_map = new tmx::Map();

	m_map->load(levelDescriptor.levelPath);

	// ##########		Level		##########
	int levelId = makeLayers();

	m_levelLayer = std::make_unique<MapLayer>(*m_map, levelId);
		
	const auto& m_collisionLayer = m_layersMap["CollisionsLayer"]->getLayerAs<tmx::ObjectGroup>();
	for (const auto& object : m_collisionLayer.getObjects()) {
		auto& objectRect = object.getAABB();
		auto rectShape = std::make_unique<sf::RectangleShape>();
		rectShape->setPosition(objectRect.left, objectRect.top);
		rectShape->setSize(sf::Vector2f(objectRect.width, objectRect.height));
		m_collisionShapes.push_back(std::move(rectShape));
	}

	// ##########		Fruits		##########
	m_fruitFistGUI = setFruitFirstGID();

	// ##########		Enemy Positions		##########
	CreateEnemyPositions(m_layersMap["Enemys"]->getLayerAs<tmx::TileLayer>());

	m_fruitTileLayer = &m_layersMap["Fruits"]->getLayerAs<tmx::TileLayer>();

	// ##########		Positions		########## (Player start, CheckPoint & EndPosition)
	makePositions();

#if DEBUG_MODE
	std::cout << "player start pos: " << m_startPlayerPosition.x << ", " << m_startPlayerPosition.y << std::endl;
	std::cout << "checkpoint pos: " << m_checkPointPosition.x << ", " << m_checkPointPosition.y << std::endl;
	std::cout << "end pos: " << m_checkPointPosition.x << ", " << m_checkPointPosition.y << std::endl;
#endif

	// ##########		BackGround		##########
	sf::Texture* bgTexture = AssetManager::getInstance()->loadTexture(levelDescriptor.backgroundTexturePath.c_str());
	bgTexture->setRepeated(true);
	m_backgroundSprite.setTexture(*bgTexture);
	m_backgroundSprite.setScale(levelDescriptor.backgroundScale);
	m_backgroundSprite.setTextureRect(sf::IntRect(0, -64 * m_backgroundSprite.getScale().y, m_map->getBounds().width, m_map->getBounds().height));



	// Clear map 
	delete m_map;
	m_map = nullptr;
}


int Level::makeLayers()
{
	int levelID = -1;
	const auto& layers = m_map->getLayers();
	for (int i = 0; i< layers.size(); ++i)
	{
		switch (layers[i]->getType())
		{
		case tmx::Layer::Type::Tile:
			m_layersMap[layers[i]->getName()] = std::unique_ptr<tmx::TileLayer>(new tmx::TileLayer(dynamic_cast<const tmx::TileLayer&>(*layers[i])));
			if (layers[i]->getName() == "Level") levelID = i;
			break;
		case tmx::Layer::Type::Object:
			m_layersMap[layers[i]->getName()] = std::unique_ptr<tmx::ObjectGroup>(new tmx::ObjectGroup(dynamic_cast<const tmx::ObjectGroup&>(*layers[i])));
			break;
		default:
			std::cout << "Unknow type layer: " << layers[i]->getName();
			break;
		}
	}
#if DEBUG_MODE
	for (const auto& layer : m_layersMap) {
		std::cout << "Load layer: " << layer.first<< " (type: " << static_cast<int>(layer.second->getType()) << ")\n";
	}
#endif
	return levelID;
}

void Level::makePositions()
{
	const auto& objectGroup = m_layersMap["Positions"]->getLayerAs<tmx::ObjectGroup>();
	for (const auto& object : objectGroup.getObjects())
	{
		if (object.getName() == "PlayerStart") {
			m_startPlayerPosition.x = object.getPosition().x;	m_startPlayerPosition.y = object.getPosition().y;
		}
		if (object.getName() == "CheckPoint")
		{
			m_checkPointPosition.x = object.getPosition().x;	m_checkPointPosition.y = object.getPosition().y;
		}
		if (object.getName() == "End")
		{
			m_endPosition.x = object.getPosition().x;	m_endPosition.y = object.getPosition().y;
		}
	}
}


void Level::CreateEnemyPositions(const tmx::TileLayer& tileLayer)
{
	uint32_t tilesetFirstGID = 0;
	const tmx::Tileset* tileset = nullptr;
	const std::vector<tmx::Tileset>& tilesets = m_map->getTilesets();
	for (const auto& ts : tilesets) {
		if (ts.getName() == "Enemys") {
			tilesetFirstGID = ts.getFirstGID();
			tileset = &ts;
			break;
		}
	}
	const auto& tiles = tileLayer.getTiles();
	tmx::Vector2u mapSize = tileLayer.getSize();
		
	for (int i = 0; i < tiles.size(); ++i)
	{
		int x = i % mapSize.x;
		int y = i / mapSize.x;

		int tileID = tiles[i].ID;
		if (tileID != 0)
		{
			sf::Vector2f position = {(float) x * m_map->getTileSize().x, (float) y * m_map->getTileSize().y };

			std::string enemyTypeName;
			int enemyTypeNumb = tileID - tilesetFirstGID;
			switch (enemyTypeNumb)
			{
			case 0:
				enemyTypeName = "Fire";
				break;
			case 1:
				enemyTypeName = "Spike";
				break;
			default:
				std::cout << "unknow enemy type -> " << enemyTypeNumb << std::endl;
				break;
			}

			float rotation = 0;
			m_enemyPositions[enemyTypeName].emplace_back(position, rotation);						
		}
	}

	const auto& objectGroup = m_layersMap["SawPositions"]->getLayerAs<tmx::ObjectGroup>();
	for (const auto& object : objectGroup.getObjects())
	{
		sf::Vector2f position(object.getPosition().x, object.getPosition().y);
		m_enemyPositions["Saw"].emplace_back(position, 0.f);
	}

#if DEBUG_MODE
	for each (const auto& enemysposition in m_enemyPositions)
	{
		std::cout << "Enemys type: " << enemysposition.first << " : ";
		for (int i = 0; i < enemysposition.second.size(); ++i)
		{
			std::cout << enemysposition.second[i].first.x << " , " << enemysposition.second[i].first.y << " / ";
		}
		std::cout << " // " << std::endl;
	}
#endif 
}



bool Level::checkCollision(const sf::FloatRect& shapeToCheck) const {
	for (const auto& shape : m_collisionShapes)
	{
		if (shape->getGlobalBounds().intersects(shapeToCheck)) {
			return true;
		}
	}
	return false;
}

const std::uint32_t Level::setFruitFirstGID()
{
	const std::vector<tmx::Tileset>& tilesets = m_map->getTilesets();
	for (const auto& tileset : tilesets) {
		if (tileset.getName() == "fruits") {
			return tileset.getFirstGID();
			break;
		}
	}
	return 0;
}





void Level::update(float deltaMilliseconds)
{
	if(m_levelLayer)m_levelLayer->update(sf::milliseconds(deltaMilliseconds));
	m_backgroundAnim += 2.f * deltaMilliseconds * 0.01f;
	if(m_backgroundAnim >= 0 ) m_backgroundAnim -= 64.f * m_backgroundSprite.getScale().y;
	m_backgroundSprite.setPosition(0, m_backgroundAnim);
}


void Level::render(sf::RenderWindow& window) 
{
	window.draw(m_backgroundSprite);
	window.draw(*m_levelLayer);
};

