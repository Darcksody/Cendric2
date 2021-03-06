#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "World/MovableGameObject.h"

enum class FallingTileState {
	Idle,
	Falling,
	Waiting,
	Returning
};

class FallingTile final : public LevelDynamicTile, public MovableGameObject {
public:
	FallingTile(LevelScreen* levelScreen);

	void updateFirst(const sf::Time& frameTime) override { MovableGameObject::updateFirst(frameTime); }
	void renderAfterForeground(sf::RenderTarget& target) override { MovableGameObject::renderAfterForeground(target); }
	void render(sf::RenderTarget& target) override { LevelDynamicTile::render(target); }

	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void onHit(LevelMovableGameObject* mob) override;

	GameObjectType getConfiguredType() const override { return LevelDynamicTile::getConfiguredType(); }
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Falling; }

private:
	std::string getSpritePath() const override;
	std::string getSoundPath() const override;
	bool isCurrentlyColliding(); 
	void checkCollisions(const sf::Vector2f& nextPosition);

	const float GRAVITY_ACCELERATION = 1000.f;
	const float RETURN_VELOCITY = 80.f;
	const float AGGRO_DISTANCE = 250.f;
	const sf::Time WAITING_TIME = sf::seconds(2.f);

	float m_initialHeight = 0.f;
	FallingTileState m_tileState;
	sf::Time m_waitingTime = sf::Time::Zero;
};