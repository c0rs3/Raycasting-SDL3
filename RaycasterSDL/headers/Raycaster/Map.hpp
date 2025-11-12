#pragma once
#ifndef RAYCASTER_MAP
#define RAYCASTER_MAP

#include <Raycaster.hpp>

// TODO: Encapsulate
struct Map {
	~Map() = default;

	/*
	* @brief Takes the ownership of the map data pointer.
	*
	* DO NOT DELETE the pointers used in the constructor.
	*/
	Map(int* map, uint32_t mapWidth, uint32_t mapHeight)
		: mData(map), mapWidth(mapWidth), mapHeight(mapHeight) {}

	void EditorCLI(uint32_t height, uint32_t width);
	int EditorWindow(uint32_t screenWidth, uint32_t screenHeight);
	void printMap();

	std::unique_ptr<int> mData;
	uint32_t mapWidth;
	uint32_t mapHeight;
};

#endif