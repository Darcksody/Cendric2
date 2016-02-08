#pragma once

#include "global.h"
#include "AnimatedGameObject.h"
#include "GUI/BitmapText.h"
#include "ResourceManager.h"

// A movable game object with physics.  Abstract class
class MovableGameObject : public virtual AnimatedGameObject {
public:
	MovableGameObject();
	virtual ~MovableGameObject() { delete m_debugInfo; }

	void updateFirst(const sf::Time& frameTime) override;
	void update(const sf::Time& frameTime) override;

	void renderAfterForeground(sf::RenderTarget& target) override;
	void setDebugBoundingBox(const sf::Color &debugColor) override;
	
	void calculateNextPosition(const sf::Time& frameTime, sf::Vector2f& nextPos) const; 
	void calculateNextVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const;
	virtual void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const;

	void setAcceleration(const sf::Vector2f& acceleration);
	void setAccelerationX(float accelerationX);
	void setAccelerationY(float accelerationY);
	void addAccelerationX(float accelerationX);
	void addAccelerationY(float accelerationY);
	void setVelocity(const sf::Vector2f& velocity);
	void setVelocityX(float velocityX);
	void setVelocityY(float velocityY);
	// the relative velocity is added independently of all other velocity calculations
	// it is used by the moving tiles
	void setRelativeVelocity(const sf::Vector2f& relVel);
	
	const sf::Vector2f& getVelocity() const;
	const sf::Vector2f& getRelativeVelocity() const;
	const sf::Vector2f& getAcceleration() const;

protected:
	virtual void updateRelativeVelocity(const sf::Time& frameTime);
	virtual float getConfiguredMaxVelocityYUp() const;
	virtual float getConfiguredMaxVelocityYDown() const;
	virtual float getConfiguredMaxVelocityX() const;
	sf::Vector2f m_velocity;
	sf::Vector2f m_relativeVelocity;
	sf::Vector2f m_acceleration;
	void boundVelocity(sf::Vector2f& vel) const;

	// debug info
	BitmapText* m_debugInfo = nullptr;
};