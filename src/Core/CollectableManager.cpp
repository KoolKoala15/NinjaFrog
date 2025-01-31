#include <Core/CollectableManager.h>

#include <random>
#include <Core/AssetManager.h>
#include <Core/AudioManager.h>
#include <Core/ConfigLoader.h>
#include <tmxlite/TileLayer.hpp>
#include <Utils/Constants.h>

CollectableManager::~CollectableManager()
{
	for (Collectable* collectable : m_pool)
	{
		delete collectable;	//Each entity unload his texture in destructor
	}
	m_pool.clear();
}

CollectableManager::CollectableManager(int32_t startPoolNumber)
{
	m_descriptor = new Entity::EntityDescriptor();
	if (!ConfigLoader::loadEntityDescriptor("../Data/config/collectables_config.txt", *m_descriptor))
	{
		std::cerr << "Error loading config for fruit " << std::endl;
	}
	for (int i = 0; i < startPoolNumber; ++i)
	{
		Collectable* collectable = createCollectable();
		m_pool.push_back(collectable);
	}
}

Collectable* CollectableManager::createCollectable()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, m_descriptor->maxTilesX);

	Collectable* collectable = new Collectable();

	collectable->init(*m_descriptor);
	collectable->deactivate();
	collectable->setTileX(dist(gen));

	return collectable;
}


void CollectableManager::init(tmx::TileLayer* tileLayer, const std::uint32_t firstGID)
{
	m_totalCollectableValue = 0.0f;
	m_collectedValue = 0.0f;
	if (!tileLayer) { std::cerr << "error: tileLayer pointer is null " << std::endl; return; }

	const std::vector<tmx::TileLayer::Tile>& tiles = tileLayer->getTiles();
	tmx::Vector2u mapSize = tileLayer->getSize();

	for (int i = 0; i < tiles.size(); ++i)
	{
		int x = i % mapSize.x;
		int y = i / mapSize.x;

		int tileID = tiles[i].ID;
		if (tileID != 0)
		{
			Collectable* collectable = nullptr;
			if (m_pool.empty())
			{
				collectable = createCollectable();
			}
			else 
			{
				collectable = m_pool.back();
				m_pool.pop_back();
			}

			sf::Vector2f position = { 
				static_cast<float>(x * m_descriptor->tileHeight + m_descriptor->tileHeight * m_descriptor->origin.x), 
				static_cast<float>(y * m_descriptor->tileWidth + m_descriptor->tileWidth * m_descriptor->origin.y)
			};

			std::string texturePath = m_texturePaths[tileID - firstGID];

			collectable->m_type = tileID - firstGID;

			resetCollectable(collectable, position, texturePath);
			m_activeCollectables.push_back(collectable);

			m_totalCollectableValue += m_collectableValues[tileID - firstGID];
		}
	}


}

void CollectableManager::resetCollectable(Collectable* collectable, const sf::Vector2f& position, const std::string& texturePath)
{
	Entity::EntityDescriptor descriptor = *m_descriptor;
	descriptor.position = position;
	descriptor.texture = AssetManager::getInstance()->loadTexture(texturePath.c_str());
	collectable->init(descriptor);
	collectable->activate();
}



void CollectableManager::removeCollectable( Collectable* collectable)
{
	auto it = std::find(m_activeCollectables.begin(), m_activeCollectables.end(), collectable);
	if (it != m_activeCollectables.end())
	{
		m_collectedValue += m_collectableValues[collectable->m_type];
		collectable->deactivate();
		m_pool.push_back(*it);
		m_activeCollectables.erase(it);
		AudioManager::getInstance()->playSound("eat");
	}
}

int CollectableManager::getCollectablePercentage()
{
	if (m_totalCollectableValue == 0.0f) return 100.0f; // Don't divide by 0

	return (m_collectedValue / m_totalCollectableValue) * 100.0f;
}


void CollectableManager::deinit()
{
	for (Collectable* colllectable : m_activeCollectables)
	{
		colllectable->deactivate();
		m_pool.push_back(colllectable);
	}

	m_activeCollectables.clear();

	for (Collectable* collectable : m_pool)
	{
		collectable->deactivate();
		collectable->unloadTexture();
	}
}


void CollectableManager::update(float deltaMillilseconds)
{
	for (Collectable* collectable : m_activeCollectables)
	{
		if (collectable->isActive())
		{
			collectable->update(deltaMillilseconds);
		}
	}
	m_elapsedTime += deltaMillilseconds * millisecondsToSeconds; // elapsedTime = deltaTime
	if (m_elapsedTime >= m_descriptor->animationSpeed) {
		m_elapsedTime -= m_descriptor->animationSpeed;
		for (Collectable* collectable : m_activeCollectables)
		{
			if (collectable->isActive())
			{
				collectable->updateAnimation(true);
			}
		}
	}
}

void CollectableManager::render(sf::RenderWindow& window)
{
	for (Collectable* collectable : m_activeCollectables)
	{
		if (collectable->isActive())
		{
			collectable->render(window);
		}
	}
}



