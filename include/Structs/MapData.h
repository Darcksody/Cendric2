#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Structs/TMXData.h"
#include "Structs/MapExitData.h"
#include "Structs/NPCData.h"
#include "Structs/MapDynamicTileData.h"

struct MapData : TMXData {
	std::vector<std::pair<MapDynamicTileID, std::vector<int>>> dynamicTileLayers;
	std::vector<MapDynamicTileData> dynamicTiles;
	
	std::vector<MapExitData> mapExits;
	std::vector<NPCData> npcs;
};