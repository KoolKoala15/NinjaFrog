#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <Core/AssetManager.h>
#include <Core/AudioManager.h>
#include <Core/ConfigLoader.h>
#include <Gameplay/Enemy.h>
#include <Gameplay/Player.h>
#include <UI/UIMenu.h>
#include <UI/UILevelSelector.h>


bool ConfigLoader::loadEntityDescriptor(const std::string& filePath, Entity::EntityDescriptor& descriptor)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Error : Cant' load file " << filePath << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file,line))	//key=value
	{
		std::istringstream iss(line);
		std::string key;
		if (std::getline(iss, key, '=')) {
			std::string value;
			if (std::getline(iss, value))
			{
				if (key == "texture") {
					descriptor.texture = AssetManager::getInstance()->loadTexture(value.c_str());
				}
				else if (key == "position") {
					std::replace(value.begin(), value.end(), ',', ' ');
					std::istringstream posStream(value);
					posStream >> descriptor.position.x >> descriptor.position.y;
				}
				else if (key == "tileWidth") {
					descriptor.tileWidth = std::stof(value);
				}
				else if (key == "tileHeight") {
					descriptor.tileHeight = std::stof(value);
				}
				else if (key == "maxTilesX") {
					descriptor.maxTilesX = std::stof(value);
				}
				else if (key == "animationSpeed") {
					descriptor.animationSpeed = std::stof(value);
				}
				else if (key == "scale") {
					std::replace(value.begin(), value.end(), ',', ' ');
					std::istringstream scaleStream(value);
					scaleStream >> descriptor.scale.x >> descriptor.scale.y;
				}
				else if (key == "origin") {
					std::replace(value.begin(), value.end(), ',', ' ');
					std::istringstream originStream(value);
					originStream >> descriptor.origin.x >> descriptor.origin.y;
				}
				else if (key == "speed") {
					std::replace(value.begin(), value.end(), ',', ' ');
					std::istringstream speedStream(value);
					speedStream >> descriptor.speed.x >> descriptor.speed.y;
				}
				else if (key == "collisionShapeSize") {
					std::replace(value.begin(), value.end(), ',', ' ');
					std::istringstream collisionShapeSize(value);
					collisionShapeSize >> descriptor.collisionShapeSize.x >> descriptor.collisionShapeSize.y;
				}
				else if (key == "collisionShapeOrigin") {
					std::replace(value.begin(), value.end(), ',', ' ');
					std::istringstream collisionShapeOrigin(value);
					collisionShapeOrigin >> descriptor.collisionShapeOrigin.x >> descriptor.collisionShapeOrigin.y;
				}
				else if (key == "maxTilePerState") {
					std::replace(value.begin(), value.end(), ',', ' ');
					std::istringstream arrayStream(value);
					int num;
					while (arrayStream >> num) {
						descriptor.maxTilePerState.push_back(num);
						if (arrayStream.peek() == ',') {
							arrayStream.ignore();
						}
					}
				}
				else if (key == "hasManager") {
					descriptor.hasManager = (value == "true" || value == "1" || value == "yes");
				}
				// Only for Player
				else if (key == "maxSpeed") {
					Player::PlayerDescriptor* playerDescriptor = static_cast<Player::PlayerDescriptor*>(&descriptor);
					if (playerDescriptor) {
						std::replace(value.begin(), value.end(), ',', ' ');
						std::istringstream speedStream(value);
						speedStream >> playerDescriptor->maxSpeed.x >> playerDescriptor->maxSpeed.y;
					}
				}
				else if (key == "jumpForce") {
					Player::PlayerDescriptor* playerDescriptor = static_cast<Player::PlayerDescriptor*>(&descriptor);
					if (playerDescriptor) {
						playerDescriptor->jumpForce = std::stof(value);
					}
				}
				// Only for Enemy
				else if (key == "enemyType")
				{
					Enemy::EnemyDescriptor* enemyDescriptor = static_cast<Enemy::EnemyDescriptor*>(&descriptor);
					if (enemyDescriptor)
					{
						enemyDescriptor->type = Enemy::stringToEnemyType(value);
					}
				}
			}
		}
	}

	file.close();
	return true;
}

bool ConfigLoader::loadGameDescriptior(const std::string& filePath, Game::GameCreateInfo& descriptor)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Error : Cant' load file " << filePath << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file, line))	//key=value
	{
		std::istringstream iss(line);
		std::string key;
		if (std::getline(iss, key, '=')) {
			std::string value;
			if (std::getline(iss, value)) {
				if (key == "gameTitle") {
					descriptor.gameTitle = value;
				}
				else if (key == "screenWidth") {
					descriptor.screenWidth = static_cast<uint32_t>(std::stoul(value));
				}
				else if (key == "screenHeight") {
					descriptor.screenHeight = static_cast<uint32_t>(std::stoul(value));
				}
				else if (key == "frameRateLimit") {
					descriptor.frameRateLimit = static_cast<uint32_t>(std::stoul(value));
				}
			}
		}
	}

	file.close();
	return true;
}

bool ConfigLoader::loadScreenDescriptor(const std::string& filePath, UIScreen::UIScreenDescriptor& descriptor)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Error : Cant' load file " << filePath << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file, line))	//key=value
	{
		std::istringstream iss(line);
		std::string key;
		if (std::getline(iss, key, '=')) {
			std::string value;
			if (std::getline(iss, value)) {
				if (key == "fontPath") {
					descriptor.font = AssetManager::getInstance()->loadFont(value.c_str());
				}
				else if (key == "backgroundFilePath") {
					UIMenu::UIMenuDescriptor* uiMenuDescriptor = static_cast<UIMenu::UIMenuDescriptor*>(&descriptor);
					if (uiMenuDescriptor) {
						sf::Texture* texture = new sf::Texture();
						texture->loadFromFile(value.c_str());
						uiMenuDescriptor->backgroundTexture = texture;
					}
				}
				else if (key == "backgroundScale") {
					UIMenu::UIMenuDescriptor* uiMenuDescriptor = static_cast<UIMenu::UIMenuDescriptor*>(&descriptor);
					if (uiMenuDescriptor) {
						std::replace(value.begin(), value.end(), ',', ' ');
						std::istringstream scaleStream(value);
						scaleStream >> uiMenuDescriptor->backgroundScale.x >> uiMenuDescriptor->backgroundScale.y;
					}
				}
				else if (key == "rows") {
					UILevelSelector::LevelSelectorDescriptor* uiLevelSelectorDescriptor = static_cast<UILevelSelector::LevelSelectorDescriptor*> (&descriptor);
					if (uiLevelSelectorDescriptor) {
						uiLevelSelectorDescriptor->rows = std::stof(value);
					}
				}
				else if (key == "cols") {
					UILevelSelector::LevelSelectorDescriptor* uiLevelSelectorDescriptor = static_cast<UILevelSelector::LevelSelectorDescriptor*> (&descriptor);
					if (uiLevelSelectorDescriptor) {
						uiLevelSelectorDescriptor->cols = std::stof(value);
					}
				}
				else if (key == "horizontalEdgesSpacer") {
					UILevelSelector::LevelSelectorDescriptor* uiLevelSelectorDescriptor = static_cast<UILevelSelector::LevelSelectorDescriptor*> (&descriptor);
					if (uiLevelSelectorDescriptor) {
						uiLevelSelectorDescriptor->horizontalEdgesSpacer = std::stof(value);
					}
				}
				else if (key == "verticalEdgesSpacer") {
					UILevelSelector::LevelSelectorDescriptor* uiLevelSelectorDescriptor = static_cast<UILevelSelector::LevelSelectorDescriptor*> (&descriptor);
					if (uiLevelSelectorDescriptor) {
						uiLevelSelectorDescriptor->verticalEdgesSpacer = std::stof(value);
					}
				}
				else if (key == "buttonsSpacing") {
					UILevelSelector::LevelSelectorDescriptor* uiLevelSelectorDescriptor = static_cast<UILevelSelector::LevelSelectorDescriptor*> (&descriptor);
					if (uiLevelSelectorDescriptor) {
						uiLevelSelectorDescriptor->buttonsSpacing = std::stof(value);
					}
				}
				else if (key == "buttonTexturePath") {
					UILevelSelector::LevelSelectorDescriptor* uiLevelSelectorDescriptor = static_cast<UILevelSelector::LevelSelectorDescriptor*> (&descriptor);
					if (uiLevelSelectorDescriptor) {
						uiLevelSelectorDescriptor->buttonTexture = AssetManager::getInstance()->loadTexture(value.c_str());
					}
				}
				else if (key == "buttonCharSize") {
					UILevelSelector::LevelSelectorDescriptor* uiLevelSelectorDescriptor = static_cast<UILevelSelector::LevelSelectorDescriptor*> (&descriptor);
					if (uiLevelSelectorDescriptor) {
						uiLevelSelectorDescriptor->buttonCharSize = std::stof(value);
					}
				}
				else if (key == "buttonTextColor") {
					UILevelSelector::LevelSelectorDescriptor* uiLevelSelectorDescriptor = static_cast<UILevelSelector::LevelSelectorDescriptor*> (&descriptor);
					if (uiLevelSelectorDescriptor) {
						std::istringstream colorStream(value);
						int r, g, b, a = 255;
						char comma;
						colorStream >> r >> comma >> g >> comma >> b;
						if (colorStream.peek() == ',') {
							colorStream >> comma >> a;
						}
						uiLevelSelectorDescriptor->buttonTextColor = sf::Color(r, g, b, a);
					}
				}
				else if (key == "textPositionAdjustment") {
					UILevelSelector::LevelSelectorDescriptor* uiLevelSelectorDescriptor = static_cast<UILevelSelector::LevelSelectorDescriptor*> (&descriptor);
					if (uiLevelSelectorDescriptor) {
						std::replace(value.begin(), value.end(), ',', ' ');
						std::istringstream scaleStream(value);
						scaleStream >> uiLevelSelectorDescriptor->textPositionAdjustment.x >> uiLevelSelectorDescriptor->textPositionAdjustment.y;
					}
				}
				else if (key == "buttonScale") {
					UILevelSelector::LevelSelectorDescriptor* uiLevelSelectorDescriptor = static_cast<UILevelSelector::LevelSelectorDescriptor*> (&descriptor);
					if (uiLevelSelectorDescriptor) {
						std::replace(value.begin(), value.end(), ',', ' ');
						std::istringstream scaleStream(value);
						scaleStream >> uiLevelSelectorDescriptor->buttonScale.x >> uiLevelSelectorDescriptor->buttonScale.y;
					}
				}
				else if (key == "exitButtonTexture") {
					UILevelSelector::LevelSelectorDescriptor* uiLevelSelectorDescriptor = static_cast<UILevelSelector::LevelSelectorDescriptor*> (&descriptor);
					if (uiLevelSelectorDescriptor) {
						uiLevelSelectorDescriptor->exitButtonTexture = AssetManager::getInstance()->loadTexture(value.c_str());
					}
				}
				else if (key == "exitButtonScale") {
					UILevelSelector::LevelSelectorDescriptor* uiLevelSelectorDescriptor = static_cast<UILevelSelector::LevelSelectorDescriptor*> (&descriptor);
					if (uiLevelSelectorDescriptor) {
						std::replace(value.begin(), value.end(), ',', ' ');
						std::istringstream scaleStream(value);
						scaleStream >> uiLevelSelectorDescriptor->exitButtonScale.x >> uiLevelSelectorDescriptor->exitButtonScale.y;
					}
				}
				else if (key == "exitButtonPosition") {
					UILevelSelector::LevelSelectorDescriptor* uiLevelSelectorDescriptor = static_cast<UILevelSelector::LevelSelectorDescriptor*> (&descriptor);
					if (uiLevelSelectorDescriptor) {
						std::replace(value.begin(), value.end(), ',', ' ');
						std::istringstream positionStream(value);
						positionStream >> uiLevelSelectorDescriptor->exitButtonPosition.x >> uiLevelSelectorDescriptor->exitButtonPosition.y;
					}
				}

			}
		}
	}

	file.close();
	return true;
}

bool ConfigLoader::loadLevelsData(const std::string& filePath, std::unordered_map<int, Game::LevelData>& levelsMap)
{
	std::ifstream file (filePath);
	if (!file.is_open()) {
		std::cerr << "Error : Cant' load file " << filePath << std::endl;
		return false;
	}
	std::string line;
	Game::LevelData currentLevel;
	int currenteLevelNumber = -1;

	while (std::getline(file, line))
	{
		if (line.empty()) continue;

		std::istringstream iss(line);
		std::string key, value;

		if (std::getline(iss, key, '=') && std::getline(iss, value)) {
			if (key == "levelNumb") {
				// if there is data on the current currentLevel, save it
				if (currenteLevelNumber != -1) {	
					levelsMap[currenteLevelNumber] = currentLevel;
					currentLevel = Game::LevelData();	// reset for the next level
				}
				currenteLevelNumber = std::stoi(value);
				currentLevel.levelNumber = currenteLevelNumber;
			}
			else if (key == "unloked") {
				currentLevel.unlocked = (value == "true" || value == "1" || value == "yes");
			}
			else if (key == "levelPath") {
				currentLevel.levelPath = value;
			}
			else if (key == "backgroundTexturePath")
			{
				currentLevel.backgroundTexturePath = value;
			}
			else if (key == "backgroundScale")
			{
				std::replace(value.begin(), value.end(), ',', ' ');
				std::istringstream backgroundScale(value);
				backgroundScale >> currentLevel.backgroundScale.x >> currentLevel.backgroundScale.y;
			}
		}
	}

	// Save last level
	if (currenteLevelNumber != -1) {
		currentLevel.levelNumber = currenteLevelNumber;
		levelsMap[currenteLevelNumber] = currentLevel;
	}

	file.close();
	return true;
}




bool ConfigLoader::makeSave(Game::SaveData& saveData)
{
	std::ofstream file("../Data/Saves/gameSave.txt");
	file << "maxLevel=" << saveData.maxLevel << "\n";
	
	for (const auto& [level, score] : saveData.levelScores) {
		file << "level_" << level << "=" << score << "\n";
	}
	file.close();
	return true;
}

bool ConfigLoader::loadSave(Game::SaveData& saveData)
{
	std::ifstream file("../Data/Saves/gameSave.txt");
	if (!file) {
		return false;
	}
	std::string line;
	while (std::getline(file, line))	//key=value
	{
		std::istringstream iss(line);
		std::string key;
		if (std::getline(iss, key, '=')) {
			std::string value;
			if (std::getline(iss, value)) {
				if (key == "maxLevel") {
					saveData.maxLevel = std::stof(value);
				}
				else if (key.rfind("level_", 0) == 0) { // Start whit "level_"
					int level = std::stoi(key.substr(6)); // Extract level numb
					int score = std::stoi(value);
					saveData.levelScores[level] = score;
				}
			}
		}
	}

	file.close();
	return true;
}



bool ConfigLoader::loadSounds(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Error : Cant' load file " << filePath << std::endl;
		return false;
	}
	AudioManager* audioManager = AudioManager::getInstance();
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string soundName, soundFilePath;
		if (std::getline(iss, soundName, '=') && std::getline(iss, soundFilePath)) {
			audioManager->loadSound(soundName, soundFilePath);
		}
	}

	file.close();
	return true;

}