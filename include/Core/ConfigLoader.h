#pragma once

#include <Gameplay/Enemy.h>
#include <Core/Game.h>
#include <UI/UIMenu.h>
#include <string>


class ConfigLoader
{
public:
	static bool loadEntityDescriptor(const std::string& filePath, Entity::EntityDescriptor& descriptor);

	static bool loadGameDescriptior(const std::string& filePath, Game::GameCreateInfo& descriptor);

	static bool loadScreenDescriptor(const std::string& filePath, UIScreen::UIScreenDescriptor& descriptor);

	static bool loadLevelsData(const std::string& filePath, std::unordered_map<int, Game::LevelData>& levelsMap);

	static bool makeSave(Game::SaveData& saveData);

	static bool loadSave(Game::SaveData& saveData);

	static bool loadSounds(const std::string& filePath);
};