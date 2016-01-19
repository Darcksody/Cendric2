#include "Level/DynamicTiles/FrozenWaterTile.h"
#include "Spell.h"

#include "Level/DynamicTiles/SimulatedWaterTile.h"

// this tile is special and is not registered
FrozenWaterTile::FrozenWaterTile(SimulatedWaterTile *waterTile, int waterTileIndex) : LevelDynamicTile(nullptr) {
	m_waterTile = waterTile;
	m_waterTileIndex = waterTileIndex;
}

void FrozenWaterTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(m_tileSize.x), static_cast<float>(m_tileSize.y)));
}

void FrozenWaterTile::load(int skinNr) {
	m_isCollidable = true;

	Animation idleAnimation(sf::seconds(0.5f));
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_frozenwater));
	idleAnimation.addFrame(sf::IntRect(BORDER, BORDER, m_tileSize.x, m_tileSize.y));
	//idleAnimation.addFrame(sf::IntRect(BORDER + 1 * (2 * BORDER + m_tileSize.x), BORDER, m_tileSize.x, m_tileSize.y));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

void FrozenWaterTile::onHit(Spell* spell) {
	using std::cout; using std::endl;
	switch (spell->getSpellID()) {
	case SpellID::FireBall:
		spell->setDisposed();
		m_waterTile->melt(m_waterTileIndex);
		setDisposed();
		break;
	default:
		break;
	}
}