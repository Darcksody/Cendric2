#include "SpellCreators/RaiseTheDeadSpellCreator.h"
#include "Screens/LevelScreen.h"

RaiseTheDeadSpellCreator::RaiseTheDeadSpellCreator(const SpellData& spellData, LevelMovableGameObject* owner) : SpellCreator(spellData, owner) {
}

void RaiseTheDeadSpellCreator::execExecuteSpell(const sf::Vector2f& target) {
	SpellData spellData = m_spellData;
	updateDamageAndHeal(spellData);

	RaiseTheDeadSpell* newSpell = new RaiseTheDeadSpell();
	newSpell->load(spellData, m_owner, target);
	m_screen->addObject(newSpell);
}

int RaiseTheDeadSpellCreator::getStrengthModifierValue() const {
	return m_spellData.strength;
}

std::string RaiseTheDeadSpellCreator::getStrengthModifierName() const {
	return "Strength";
}

void RaiseTheDeadSpellCreator::addRangeModifier(int level) {
	SpellCreator::addRangeModifier(level);
	m_spellData.activeDuration = sf::seconds(m_spellData.range / m_spellData.speed);
}

void RaiseTheDeadSpellCreator::addStrengthModifier(int level) {
	m_spellData.strength += level;
}

void RaiseTheDeadSpellCreator::addDurationModifier(int level) {
	m_spellData.duration += static_cast<float>(level) * m_spellData.durationModifierAddition;
}