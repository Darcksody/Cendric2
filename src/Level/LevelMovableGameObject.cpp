#include "Level/LevelMovableGameObject.h"
#include "SpellManager.h"
#include "Level/Level.h"

LevelMovableGameObject::LevelMovableGameObject(Level* level) : MovableGameObject() {
	m_level = level;
	m_foodAttributes.first = sf::Time::Zero;
	m_foodAttributes.second = ZERO_ATTRIBUTES;
	m_gravity = getConfiguredGravityAcceleration();
}

LevelMovableGameObject::~LevelMovableGameObject() {
	delete m_spellManager;
}

void LevelMovableGameObject::update(const sf::Time& frameTime) {
	if (m_isDead) {
		setAcceleration(sf::Vector2f(0, m_gravity));
	}
	else {
		handleMovementInput();
		handleAttackInput();
	}

	m_spellManager->update(frameTime);
	calculateNextPosition(frameTime, m_nextPosition);
	checkCollisions(m_nextPosition);
	m_level->collideWithDynamicTiles(this, getBoundingBox());
	MovableGameObject::update(frameTime);
	// update time
	m_fightAnimationTime = (m_fightAnimationTime - frameTime) >= sf::Time::Zero ? m_fightAnimationTime - frameTime : sf::Time::Zero;
	updateAnimation(frameTime);
	if (!m_isDead) {
		updateAttributes(frameTime);
	}
	setAccelerationX(0.f);
}

void LevelMovableGameObject::updateAttributes(const sf::Time& frameTime) {
	// update food attributes
	if (m_foodAttributes.first > sf::Time::Zero) {
		m_foodAttributes.first -= frameTime;
		if (m_foodAttributes.first <= sf::Time::Zero) {
			m_foodAttributes.first = sf::Time::Zero;
			m_attributes.removeBean(m_foodAttributes.second);
		}
	}

	// update buff attributes
	for (int i = 0; i < m_buffAttributes.size();/* don't increment here, we remove on the fly */) {
		m_buffAttributes[i].first -= frameTime;
		if (m_buffAttributes[i].first <= sf::Time::Zero) {
			m_attributes.removeBean(m_buffAttributes[i].second);
			m_buffAttributes.erase(m_buffAttributes.begin() + i);
		}
		else {
			i++;
		}
	}

	// health regeneration
	m_timeSinceRegeneration += frameTime;
	if (m_timeSinceRegeneration >= sf::seconds(1)) {
		m_timeSinceRegeneration -= sf::seconds(1);
		m_attributes.currentHealthPoints += m_attributes.healthRegenerationPerS;
		if (m_attributes.currentHealthPoints > m_attributes.maxHealthPoints) {
			m_attributes.currentHealthPoints = m_attributes.maxHealthPoints;
		}
	}

	// update debuff attributes
	for (int i = 0; i < m_dots.size();/* don't increment here, we remove on the fly */) {
		int prevSecond = static_cast<int>(std::floor(m_dots[i].duration.asSeconds()));
		m_dots[i].duration -= frameTime;
		int thisSecond = std::max(-1, static_cast<int>(std::floor(m_dots[i].duration.asSeconds())));
		addDamage(m_dots[i].damage * (prevSecond - thisSecond), m_dots[i].damageType);
		if (m_dots[i].duration <= sf::Time::Zero) {
			m_dots.erase(m_dots.begin() + i);
		}
		else {
			i++;
		}
	}
}

void LevelMovableGameObject::checkCollisions(const sf::Vector2f& nextPosition) {
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	bool isMovingDown = nextPosition.y > getBoundingBox()->top; // the mob is always moving either up or down, because of gravity. There are very, very rare, nearly impossible cases where they just cancel out.
	bool isMovingX = nextPosition.x != getBoundingBox()->left;

	// check for collision on x axis
	if (isMovingX && m_level->collides(nextBoundingBoxX, m_ignoreDynamicTiles)) {
		setAccelerationX(0.0f);
		setVelocityX(0.0f);
	}
	else {
		nextBoundingBoxY.left = nextPosition.x;
	}

	// check for collision on y axis
	bool collidesY = m_level->collides(nextBoundingBoxY, m_ignoreDynamicTiles);
	if (!isMovingDown && collidesY) {
		setAccelerationY(0.0);
		setVelocityY(0.0f);
		// set mob up in case of anti gravity!
		if (getIsUpsideDown()) {
			setPositionY(m_level->getCeiling(nextBoundingBoxY));
			m_isGrounded = true;
		}
	}
	else if (isMovingDown && collidesY) {
		setAccelerationY(0.0f);
		setVelocityY(0.0f);
		// set mob down. in case of normal gravity.
		if (!getIsUpsideDown()) {
			setPositionY(m_level->getGround(nextBoundingBoxY));
			m_isGrounded = true;
		}
	}

	if (std::abs(getVelocity().y) > 0.f)
		m_isGrounded = false;
}

sf::Vector2f LevelMovableGameObject::getConfiguredSpellOffset() const {
	return sf::Vector2f(0, 0);
}

void LevelMovableGameObject::updateAnimation(const sf::Time& frameTime) {
	// calculate new game state and set animation.

	GameObjectState newState = GameObjectState::Idle;
	if (m_isDead) {
		newState = GameObjectState::Dead;
	}
	else if (m_fightAnimationTime > sf::Time::Zero) {
		newState = GameObjectState::Fighting;
	}
	else if (!m_isGrounded) {
		newState = GameObjectState::Jumping;
	}
	else if (std::abs(getVelocity().x) > 20.0f) {
		newState = GameObjectState::Walking;
	}

	// only update animation if we need to
	if (m_state != newState || m_nextIsFacingRight != m_isFacingRight) {
		m_isFacingRight = m_nextIsFacingRight;
		m_state = newState;
		setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	}
}

void LevelMovableGameObject::addAttributes(const sf::Time& duration, const AttributeData& attributes) {
	m_attributes.addBean(attributes);
	m_buffAttributes.push_back(std::pair<sf::Time, AttributeData>(duration, attributes));
}

void LevelMovableGameObject::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	// distinguish damping in the air and at the ground
	float dampingPerSec = (getVelocity().y == 0.0f) ? getConfiguredDampingGroundPersS() : getConfiguredDampingAirPerS();
	// don't damp when there is active acceleration 
	if (getAcceleration().x != 0.0f) dampingPerSec = 0;
	nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds()) * pow(1 - dampingPerSec, frameTime.asSeconds());
	nextVel.y = getVelocity().y + getAcceleration().y * frameTime.asSeconds();
}

void LevelMovableGameObject::addDamage(int damage_, DamageType damageType) {
	int damage = 0;
	switch (damageType) {
	case DamageType::Physical:
		damage = static_cast<int>(damage_ * m_attributes.physicalMultiplier);
		break;
	case DamageType::Ice:
		damage = static_cast<int>(damage_ * m_attributes.iceMultiplier);
		break;
	case DamageType::Fire:
		damage = static_cast<int>(damage_ * m_attributes.fireMultiplier);
		break;
	case DamageType::Shadow:
		damage = static_cast<int>(damage_ * m_attributes.shadowMultiplier);
		break;
	case DamageType::Light:
		damage = static_cast<int>(damage_ * m_attributes.lightMultiplier);
		break;
	default:
		break;
	}

	if (m_isDead || damage <= 0) return;
	m_attributes.currentHealthPoints = std::max(0, std::min(m_attributes.maxHealthPoints, m_attributes.currentHealthPoints - damage));
	if (m_attributes.currentHealthPoints == 0) {
		setDead();
	}
	setSpriteColor(sf::Color::Red, sf::milliseconds(200));
}

void LevelMovableGameObject::addDamageOverTime(const DamageOverTimeData& data) {
	if (m_isDead || data.damageType == DamageType::VOID) return;
	m_dots.push_back(data);
}

void LevelMovableGameObject::addHeal(int heal) {
	if (m_isDead || heal <= 0) return;
	m_attributes.currentHealthPoints = std::max(0, std::min(m_attributes.maxHealthPoints, m_attributes.currentHealthPoints + heal));
	setSpriteColor(sf::Color::Green, sf::milliseconds(200));
}

void LevelMovableGameObject::onHit(Spell* spell) {
	if (m_isDead) {
		return;
	}
	// check for owner
	if (spell->getOwner() == this) {
		return;
	}

	spell->execOnHit(this);
	addDamage(spell->getDamage(), spell->getDamageType());
	if (spell->getDamagePerSecond() > 0.f && spell->getDuration() > sf::Time::Zero) {
		DamageOverTimeData data;
		data.damage = spell->getDamagePerSecond();
		data.damageType = spell->getDamageType();
		data.duration = spell->getDuration();
		addDamageOverTime(data);
	}
}

void LevelMovableGameObject::setDead() { 
	// dispose the spells that this mob is an owner of and that are attached to it
	// that's how magic works, I guess?
	for (auto& go : *m_screen->getObjects(GameObjectType::_Spell)) {
		if (Spell* spell = dynamic_cast<Spell*>(go)) {
			if (spell->getOwner() == this && spell->isAttachedToMob()) {
				spell->setDisposed();
			}
		}
	}
	m_attributes.currentHealthPoints = 0;
	m_isDead = true;
}

void LevelMovableGameObject::setFightAnimationTime() {
	m_fightAnimationTime = getConfiguredFightAnimationTime();
}

Level* LevelMovableGameObject::getLevel() const {
	return m_level;
}

bool LevelMovableGameObject::getIsFacingRight() const {
	return m_isFacingRight;
}

bool LevelMovableGameObject::getIsUpsideDown() const {
	return m_animatedSprite.isFlippedY();
}

bool LevelMovableGameObject::isDead() const {
	return m_isDead;
}

void LevelMovableGameObject::flipGravity() {
	m_isFlippedGravity = !m_isFlippedGravity;
	m_animatedSprite.setFlippedY(m_isFlippedGravity);
}

void LevelMovableGameObject::setGravityScale(float scale) {
	m_gravity = scale * getConfiguredGravityAcceleration();
	m_maxVelocityYDownScale = scale;
}

void LevelMovableGameObject::setMaxXVelocityScale(float scale) {
	m_maxVelocityXScale = scale;
}

void LevelMovableGameObject::setIgnoreDynamicTiles(bool value) {
	m_ignoreDynamicTiles = value;
}

GameObjectState LevelMovableGameObject::getState() const {
	return m_state;
}

float LevelMovableGameObject::getConfiguredWalkAcceleration() const {
	return 1500.0f;
}

float LevelMovableGameObject::getConfiguredGravityAcceleration() const {
	return 1000.f;
}

float LevelMovableGameObject::getGravityAcceleration() const {
	return m_gravity;
}

float LevelMovableGameObject::getConfiguredDampingGroundPersS() const {
	return 1.f;
}

float LevelMovableGameObject::getConfiguredMaxVelocityX() const {
	return m_maxVelocityXScale * getMaxVelocityX();
}

float LevelMovableGameObject::getConfiguredMaxVelocityYDown() const {
	return m_maxVelocityYDownScale * getMaxVelocityYDown();
}

float LevelMovableGameObject::getConfiguredMaxVelocityYUp() const {
	return getMaxVelocityYUp();
}

float LevelMovableGameObject::getConfiguredDampingAirPerS() const {
	return 0.7f;
}

SpellManager* LevelMovableGameObject::getSpellManager() const {
	return m_spellManager;
}

const AttributeData* LevelMovableGameObject::getAttributes() const {
	return &m_attributes;
}

void LevelMovableGameObject::consumeFood(const sf::Time& duration, const AttributeData& attributes) {
	if (m_foodAttributes.first > sf::Time::Zero) {
		// old food attributes have to be removed
		m_attributes.removeBean(m_foodAttributes.second);
	}
	m_foodAttributes = std::pair<sf::Time, AttributeData>(duration, attributes);
	m_attributes.addBean(attributes);
}