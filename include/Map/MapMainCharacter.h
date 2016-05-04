#pragma once

#include "global.h"
#include "Map/MapMovableGameObject.h"
#include "MainCharacter.h"
#include "Map.h"
#include "InputController.h"
#include "Screens/Screen.h"
#include "CharacterCore.h"
#include "GUI/DialogueWindow.h"

class MapScreen;

// Cendric on the map
class MapMainCharacter : public virtual MapMovableGameObject, public virtual MainCharacter {
public:
	MapMainCharacter(Map* level);
	~MapMainCharacter();

	void load();
	void update(const sf::Time& frameTime) override;
	void checkCollisions(const sf::Vector2f& nextPosition);
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;

	void updateFirst(const sf::Time& frameTime) override { MapMovableGameObject::updateFirst(frameTime); }
	void render(sf::RenderTarget& target) override { MapMovableGameObject::render(target); }
	void renderAfterForeground(sf::RenderTarget& target) override { MapMovableGameObject::renderAfterForeground(target); }
	void setDebugBoundingBox(const sf::Color& color) override { MapMovableGameObject::setDebugBoundingBox(color); }
	void setState(GameObjectState state) override { MapMovableGameObject::setState(state); }
	void setPosition(const sf::Vector2f& pos) override { MapMovableGameObject::setPosition(pos); }

	void setCharacterCore(CharacterCore* core);

	float getConfiguredMaxVelocityYUp() const override;
	float getConfiguredMaxVelocityYDown() const override;
	float getConfiguredMaxVelocityX() const override;
	GameObjectType getConfiguredType() const override;

private:
	const float WALK_ACCELERATION = 1500.0f;
	// handle input and calculate the next position
	void handleInput();
	GameObjectState m_state;
	CharacterCore* m_core;
};