#include "Level/LevelDynamicTile.h"
#include "Screens/LevelScreen.h"
#include "Level/LevelMainCharacter.h"

LevelDynamicTile::LevelDynamicTile(LevelScreen* levelScreen) : AnimatedGameObject() {
	m_level = levelScreen->getWorld();
	m_screen = levelScreen;
	m_mainChar = levelScreen->getMainCharacter();
}

bool LevelDynamicTile::isCollidable() const {
	return m_isCollidable;
}

bool LevelDynamicTile::isStrictlyCollidable() const {
	return m_isStrictlyCollidable;
}

bool LevelDynamicTile::isOneWay() const {
	return m_isOneWay;
}

void LevelDynamicTile::setPositionOffset(const sf::Vector2f& offset) {
	m_positionOffset = offset;
}

void LevelDynamicTile::setObjectID(int objectID) {
	m_objectID = objectID;
}

const sf::Vector2f& LevelDynamicTile::getPositionOffset() const {
	return m_positionOffset;
}

void LevelDynamicTile::render(sf::RenderTarget& target) {
	if (m_isFirstRenderIteration) {
		if (!m_isRenderAfterObjects) {
			AnimatedGameObject::render(target);
		}
		m_isFirstRenderIteration = false;
	}
	else {
		if (m_isRenderAfterObjects) {
			AnimatedGameObject::render(target);
		}
		m_isFirstRenderIteration = true;
	}
}

GameObjectType LevelDynamicTile::getConfiguredType() const {
	return _DynamicTile;
}

const Level* LevelDynamicTile::getLevel() const {
	return m_level;
}

void LevelDynamicTile::onHit(LevelMovableGameObject* mob) {
	// nop
}

void LevelDynamicTile::loadResources() {
	g_resourceManager->loadTexture(getSpritePath(), ResourceType::Level);
	g_resourceManager->loadSoundbuffer(getSoundPath(), ResourceType::Level);
}
