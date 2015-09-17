#pragma once

#include "global.h"
#include "Logger.h"
#include "tinyxml2/tinyxml2.h"

#include "Structs/MapData.h"

using namespace tinyxml2;

class MapReader
{
public:
	MapReader();
	~MapReader();

	bool readMap(const char* fileName, MapData& data);

private:
	// reads properties name, tile size, map size, tileset, dimming starting @map node
	bool readMapProperties(XMLElement* map, MapData& data) const;
	bool readMapName(XMLElement* _property, MapData& data) const;
	bool readTilesetPath(XMLElement* _property, MapData& data) const;
	bool readDimming(XMLElement* _property, MapData& data) const;

	bool readLayers(XMLElement* map, MapData& data) const;
	bool readBackgroundTileLayer(const std::string& layer, MapData& data) const;
	bool readForegroundTileLayer(const std::string& layer, MapData& data) const;
	bool readCollidableLayer(const std::string& layer, MapData& data) const;

	bool readObjects(XMLElement* map, MapData& data) const;
	bool readMapExits(XMLElement* objects, MapData& data) const;
	bool readNPCs(XMLElement* objects, MapData& data) const;
	bool readLights(XMLElement* objects, MapData& data) const;

	// check map bean for validity before loading the map
	bool checkData(MapData& data) const;

	// update data to prepare it for the map
	void updateData(MapData& data) const;
};