#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class ParticleComponent;
class LightComponent;

namespace particles {
	class AimedVelocityGenerator;
}

class DivineTorchTile final : public LevelDynamicTile {
public:
	DivineTorchTile(LevelScreen* levelScreen);

	void update(const sf::Time& frameTime) override;
	void setTarget(GameObject* target, bool force = false);

	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::DivineTorch; }

private:
	std::string getSpritePath() const override;
	LightComponent* m_lightComponent;
	particles::AimedVelocityGenerator* m_velGen = nullptr;
	ParticleComponent* m_pc;
	GameObject* m_target = nullptr;

	void loadComponents();
};