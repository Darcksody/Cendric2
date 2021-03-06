#pragma once

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/LeapOfFaithSpell.h"

// a class that creates leap of faith spells
class LeapOfFaithSpellCreator final : public SpellCreator {
public:
	LeapOfFaithSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner);
	void execExecuteSpell(const sf::Vector2f& target) override;
	std::string getStrengthModifierName() const override;
	int getStrengthModifierValue() const override;

private:
	void addStrengthModifier(int level) override;
	void addDurationModifier(int level) override;

	void removeOldSpell();
};