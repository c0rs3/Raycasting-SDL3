#pragma once
#ifndef RAYCASTER_MAP
#define RAYCASTER_MAP

#include <Raycaster.hpp>

struct Map {
	~Map() = default;

	/*
	* @brief Takes the ownership of the map data pointer.
	*
	* DO NOT DELETE the pointers used in the constructor.
	*/
	Map(int* map, unsigned int mapWidth, unsigned int mapHeight)
		: mData(map), mapWidth(mapWidth), mapHeight(mapHeight) {}

	void EditorCLI(const unsigned int& height, const unsigned int& width);
	int EditorWindow(unsigned int screenWidth, unsigned int screenHeight);
	void printMap();

	std::unique_ptr<int> mData;
	uint32_t mapWidth;
	uint32_t mapHeight;
};

#endif