#pragma once
#ifndef RAYCASTER_MAP
#define RAYCASTER_MAP

#include "raycaster.h"

struct Map {
	Map() = default;
	~Map() = default;

	/*
	* @brief Takes the ownership of the map data pointer.
	*
	* DO NOT DELETE the pointers used in the constructor.
	*/
	Map(int* map, unsigned int mapWidth, unsigned int mapHeight)
		: mData(map), mapWidth(mapWidth), mapHeight(mapHeight) {}

	void mapEditorTerminal(const unsigned int& height, const unsigned int& width);
	int mapEditorUI(unsigned int screenWidth, unsigned int screenHeight);
	void printMap();

	std::shared_ptr<int> mData;
	unsigned int mapWidth;
	unsigned int mapHeight;
};

#endif