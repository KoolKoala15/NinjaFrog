#pragma once

#include <vector>
#include <Gameplay/Collectable.h>
#include <tmxlite/Map.hpp>


class CollectableManager
{
public:

	CollectableManager(int32_t startPoolNumber);
	~CollectableManager();

	void init(tmx::TileLayer* tileLayer, const std::uint32_t firstGID);
	void deinit();

	void removeCollectable( Collectable* object);

	void update(float deltaMillilseconds);
	void render(sf::RenderWindow& window);

	const std::vector<Collectable*> getActiveCollectables() { return m_activeCollectables; }

	int getCollectablePercentage();

private:
	Entity::EntityDescriptor* m_descriptor;

	float m_elapsedTime{ .0f };

	std::vector<Collectable*> m_pool;
	std::vector<Collectable*> m_activeCollectables;

	// Textures
	std::unordered_map<std::uint32_t, std::string> m_texturePaths = {
		{0, "../Data/Images/Collectables/Apple.png"},
		{1, "../Data/Images/Collectables/Bananas.png"},
		{2, "../Data/Images/Collectables/Cherries.png"},
		{3, "../Data/Images/Collectables/Kiwi.png"},
		{4, "../Data/Images/Collectables/Melon.png"},
		{5, "../Data/Images/Collectables/Orange.png"},
		{6, "../Data/Images/Collectables/Pineapple.png"},
		{7, "../Data/Images/Collectables/Strawberry.png"}
	};

	// Score by percentage
	std::unordered_map<std::uint32_t, float> m_collectableValues = {
		{0, 10.0f}, {1 , 15.0f}, {2, 20.0f}, {3, 25.0f},
		{4, 30.0f},	{5, 35.0f}, {6, 40.0f}, {7, 45.0f}
	};
	float m_totalCollectableValue = 0.0f;
	float m_collectedValue = 0.0f;

	Collectable* createCollectable();

	void resetCollectable(Collectable* collectable, const sf::Vector2f& position, const std::string& texturePath);
};