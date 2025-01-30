
#include <cstdio>
#include <Core/Game.h>
#include <Core/ConfigLoader.h>
#include <SFML/System/Clock.hpp>
#include <Utils/Constants.h>

int main()
{
    Game::GameCreateInfo gameCI;
    if (!ConfigLoader::loadGameDescriptior("../Data/Config/game_config.txt", gameCI)) {
        std::cerr << "Error: Failed to load the game's configuration -" << std::endl;
    }
    Game game;
    const bool gameInitialized = game.init(gameCI);

    if (gameInitialized == false)
    {
        printf("Game could not be initialized\n");
        return 0;
    }

    sf::Clock clock;
    bool isWindowFocused = true;
    const float maxDeltaTime = 16.67f; // Limit deltaTime to ~16,67ms (60 FPS)

    while (game.isRunning())
    {
        sf::RenderWindow& window = game.getRenderWindow();
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                game.closeGame();
            }

            if (event.type == sf::Event::LostFocus) {
                isWindowFocused = false;
            }
            else if (event.type == sf::Event::GainedFocus) {
                isWindowFocused = true;
                clock.restart();
            }
        }
        if (!isWindowFocused) {
            continue;
        }

        const sf::Time elapsed = clock.restart();
        float deltaTime = elapsed.asMicroseconds();

        if (deltaTime > maxDeltaTime) {
            deltaTime = maxDeltaTime;
        }

        game.update(deltaTime);
        game.render();
    }
}
