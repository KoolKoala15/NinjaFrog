#pragma once

#include <unordered_map>
#include <cstdint>
#include <string>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	class RenderWindow;
}

class UIManager;
class UIScreen;
class World;

class Game
{
	public:

		struct GameCreateInfo
		{
			std::string gameTitle;
			uint32_t screenWidth;
			uint32_t screenHeight;
			uint32_t frameRateLimit;
		};

		struct LevelData
		{
			bool unlocked;
			std::string levelPath;
			std::string backgroundTexturePath;
			sf::Vector2f backgroundScale{ 1.0f, 1.0f };
			int levelNumber;
		};

		struct  SaveData
		{
			int maxLevel = 1;
			std::unordered_map<int, int> levelScores;
		};

		~Game();

		bool init(GameCreateInfo& createInfo);


		bool isRunning() const;
		void closeGame();

		void update(uint32_t deltaMilliseconds);	
		void render();
		sf::RenderWindow& getRenderWindow() { return *m_window; }


		// UI
		void CreateHuds();
		void CreateMenus();
		void goToMainMenu();
		void goToLevelMenu();

		// Level logic
		void openLevel(int levelNumber);
		void passLevel();
		void resetLevel();
		bool isLevelLocked(int level) { return !m_levels[level].unlocked; }
		int getLevelNumb() { return m_levelNumb; }

		// Pause 
		void setPause(bool pause) { m_pause = pause; }
		bool getPause() { return m_pause; }
		void togglePause();

		// Score
		float getCurrentScore();

		SaveData m_saveData;

	private:
		UIManager* m_uiManager { nullptr };
		sf::RenderWindow* m_window{ nullptr };
		World* m_world{ nullptr };

		std::unordered_map<int, Game::LevelData> m_levels;

		bool m_inMainMenu = true;
		bool m_pause = false;
		bool m_pauseKeyPresed = false;
		
		int m_levelNumb;
		int m_lastLevel;

		void deleteWorld();
		void setLevelNumber(int levelNumber);
};