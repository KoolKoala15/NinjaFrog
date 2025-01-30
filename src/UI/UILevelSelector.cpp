#include <UI/UILevelSelector.h>

#include <Core/AssetManager.h>
#include <Core/Game.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <UI/UIButton.h>
#include <UI/UIManager.h>
#include <UI/UIMenu.h>
#include <iostream>

UILevelSelector::~UILevelSelector() 
{
	deinit();
#if DEBUG_MODE
	for (auto rect : rectangles) {
		delete rect;
	};
#endif
	UIMenu::~UIMenu();
}


UILevelSelector::UILevelSelector(UIScreenDescriptor& uis, Game* game) : UIMenu(uis, game)
{
	const LevelSelectorDescriptor* lsd = static_cast<LevelSelectorDescriptor*>(&uis);

	// ##########	BackButton	##########
	UIButton::ButtonDescriptor buttonDescriptor;
	buttonDescriptor.normalPathTexture = lsd->exitButtonTexture;
	buttonDescriptor.scale = lsd->exitButtonScale;
	buttonDescriptor.position = lsd->exitButtonPosition;
	buttonDescriptor.function = []() {
		UIManager::getInstance()->setCurrentScreen("MainMenu", true, true);
		};
	UIButton* button = new UIButton(buttonDescriptor);
	m_buttons.push_back(button);

	// ##########	Font	##########
	m_font = lsd->font;
	if (!m_font) std::cerr << "Error loading levels font " << lsd->font << std::endl;


	// ##########	Level buttons	##########
	// Level button grid
	buttonDescriptor = {};
	buttonDescriptor.normalPathTexture = lsd->buttonTexture;
	buttonDescriptor.scale = lsd->buttonScale;
	// Set text
	buttonDescriptor.font = m_font;
	buttonDescriptor.charSize = lsd->buttonCharSize;
	buttonDescriptor.textColor = lsd->buttonTextColor;
	buttonDescriptor.textPositionAdjustment = lsd->textPositionAdjustment;


	// Calculate usable area(space for buttons)
	float usableWidth = game->getRenderWindow().getSize().x - (2 * lsd->horizontalEdgesSpacer);
	float usableHeight = game->getRenderWindow().getSize().y - (2 * lsd->verticalEdgesSpacer);

	// Calculate button size based on the minimum of available width and height
	float buttonSize = std::min(usableWidth / lsd->cols, usableHeight / lsd->rows);

	// Calculate displacement for grid to center it in the window
	float gridWidth = lsd->cols * buttonSize;
	float gridHeight = lsd->rows * buttonSize;
	float offsetX = (game->getRenderWindow().getSize().x - gridWidth) / 2.0f;
	float offsetY = (game->getRenderWindow().getSize().y - gridHeight) / 2.0f;
	//float offsetX = lsd->horizontalEdgesSpacer + (window.getSize().x - gridWidth) / 2.0f;
	//float offsetY = lsd->verticalEdgesSpacer + (window.getSize().y - gridHeight) / 2.0f;

	// Create buttons
	int currentButton = 1;
	for (int row = 0; row < lsd->rows; ++row) {
		for (int col = 0; col < lsd->cols; ++col) {
			if (currentButton > lsd->totalButtons) break;
#if DEBUG_MODE
			sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(buttonSize, buttonSize));

			rect->setPosition(
				offsetX + col * buttonSize,
				offsetY + row * buttonSize
			);
			rect->setFillColor(sf::Color::Transparent);
			rect->setOutlineColor(sf::Color::Yellow);
			rect->setOutlineThickness(2.f);
			rectangles.push_back(rect);
#endif
			// Calculate the position of the center of each cell
			buttonDescriptor.position = sf::Vector2f(
				offsetX + col * buttonSize + buttonSize / 2.0f,
				offsetY + row * buttonSize + buttonSize / 2.0f
			);

			buttonDescriptor.buttonNumber = currentButton;
			if (currentButton < 10) {
				buttonDescriptor.words = "0" + std::to_string(currentButton);
			}
			else {
				buttonDescriptor.words = std::to_string(currentButton);
			}

			buttonDescriptor.textBelow = std::to_string(game->m_saveData.levelScores[currentButton]) + "%";
			buttonDescriptor.charSizeTextBelow = 25;
			buttonDescriptor.textBelowColor = sf::Color::Black;
			buttonDescriptor.textBelowPositionAdjustment = { .0f, 3.f };

			UIButton* button = new UIButton(buttonDescriptor);

			button->m_function = [buttonNumber = buttonDescriptor.buttonNumber, game]() {
				game->openLevel(buttonNumber);
				};
			button->setLock(game->isLevelLocked(currentButton));

			button->m_textBelowAllwaysSaw = true;

			m_buttons.push_back(button);

			currentButton += 1;
		}
	}
}

void UILevelSelector::init(Game* game)
{
	for (UIButton* button : m_buttons)
	{
		if (button->hasTextBelow() && button->m_buttonNumb != -1)
		{
			button->setTextBelow(std::to_string(game->m_saveData.levelScores[button->m_buttonNumb]) + "%");
		}
	}
}

void UILevelSelector::deinit()
{
	AssetManager::getInstance()->unloadTexture(m_backgorundSprite.getTexture());
}

void UILevelSelector::render(sf::RenderWindow& window)
{
	UIMenu::render(window);
#if DEBUG_MODE
	for (auto& rect : rectangles) {
		window.draw(*rect);
	}
#endif
}


