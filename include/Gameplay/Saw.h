#pragma once

#include <Gameplay/Enemy.h>

class Saw : public Enemy
{
public:
	void update(float deltaMilliseconds) override;

	void setters(sf::Vector2f& endPosition);

private:
	bool m_goingToEnd{ true };

	sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float progress) {
		if (progress > 1.f) { progress = 1.f; }
		else if (progress < 0.f) { progress = 0.f; }
		return start + (end - start) * progress;
	}
};