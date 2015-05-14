#include "Screens/GameScreen.h"

using namespace std;

GameScreen::GameScreen(ResourceID levelID)
{
	m_levelID = levelID;
}

void GameScreen::execOnEnter(const Screen *previousScreen)
{
	LevelMainCharacterLoader loader;
	m_mainChar = loader.loadMainCharacter(this, &m_currentLevel);
	loader.loadEquipment(this);
	if (!(m_currentLevel.load(m_levelID, this)))
	{
		string filename(g_resourceManager->getFilename(m_levelID));
		string errormsg = filename + ": file corrupted!";
		g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
	}
}

void GameScreen::execOnExit(const Screen *nextScreen)
{
	m_currentLevel.dispose();
}

Screen* GameScreen::update(const sf::Time& frameTime)
{
	updateObjects(GameObjectType::_MainCharacter, frameTime);
	updateObjects(GameObjectType::_LevelEquipment, frameTime);
	updateObjects(GameObjectType::_Spell, frameTime);
	updateObjects(GameObjectType::_DynamicTile, frameTime);
	updateObjects(GameObjectType::_LevelItem, frameTime);
	deleteDisposedObjects();
	return this;
}

void GameScreen::render(sf::RenderTarget &renderTarget)
{
	renderTooltipText(renderTarget);
	// parallax, maybe forground + background layers?
	// don't render dynamic tiles, they are rendered in the level.
	m_currentLevel.draw(renderTarget, sf::RenderStates::Default, m_mainChar->getCenter());
	// ASSURE that at this point, the view is the correct game view
	renderObjects(GameObjectType::_LevelItem, renderTarget);
	renderObjects(GameObjectType::_MainCharacter, renderTarget);
	renderObjects(GameObjectType::_LevelEquipment, renderTarget);
	renderObjects(GameObjectType::_Spell, renderTarget);
}