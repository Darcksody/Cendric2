#include "Spells/ChopSpell.h"

ChopSpell::ChopSpell() : Spell()
{
}

void ChopSpell::load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target, float divergenceAngle)
{
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	Spell::load(bean, mob, target, 0);
}

const sf::Vector2f ChopSpell::getConfiguredPositionOffset() const
{
	return sf::Vector2f(10.f, 0.f);
}

bool ChopSpell::getConfiguredIsAttachedToMob() const
{
	return true;
}

bool ChopSpell::getConfiguredRotateSprite() const
{
	return false;
}