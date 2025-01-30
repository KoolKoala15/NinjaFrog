#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>


class UIButton {
public:
    struct ButtonDescriptor
    {
        sf::Texture* normalPathTexture{ nullptr };
        sf::Texture* clickedPathTexture{ nullptr };
        std::string words;
        sf::Font* font{ nullptr };
        int charSize{ 0 };
        sf::Color textColor = sf::Color::Magenta;
        sf::Vector2f textPositionAdjustment{ .0f, .0f };
        sf::Vector2f position{ .0f, .0f };
        sf::Vector2f scale{ 1.0f, 1.0f };
        std::function<void()> function;
        int buttonNumber{ -1 };
        std::string textBelow;
        int charSizeTextBelow{ 0 };
        sf::Color textBelowColor = sf::Color::Magenta;
        sf::Vector2f textBelowPositionAdjustment{ .0f, .0f };
    };


    UIButton(const ButtonDescriptor& buttonDescriptor);
    ~UIButton();

    void render(sf::RenderWindow& window);

    bool checkHover(sf::Vector2i& mousePos);
    void setState(bool);
    bool getCurrent() { return m_current; }
    sf::Sprite* getSprite() { return m_currentSpr; }

    void setLock(bool lock);

    bool hasTextBelow() { return m_textBelow!=nullptr; }
    void setTextBelow(std::string text) { m_textBelow->setString(text); }

    bool m_current = false;
    std::function<void()> m_function;
    int m_buttonNumb = -1;
    bool m_textBelowAllwaysSaw = false;

private:
    sf::Sprite m_normalSpr;
    sf::Sprite m_clickedSpr;
    sf::Sprite* m_currentSpr;
    std::unique_ptr<sf::Text> m_text{ nullptr };
    bool m_hasClickedSprite = true;
    bool m_locked = false;

    std::unique_ptr<sf::Text> m_textBelow{ nullptr };
};