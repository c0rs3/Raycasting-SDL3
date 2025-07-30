#ifndef RAYCASTER_MAP
#define RAYCASTER_MAP

struct Map {
	Map() = default;

	/*
	* @brief takes the ownership of the map data pointer
	*
	* DO NOT DELETE the pointers used in the constructor
	*/
	Map(int** map, unsigned int mapWidth, unsigned int mapHeight)
		: mData(map), mapWidth(mapWidth), mapHeight(mapHeight) {}

	~Map();
	void addMap(const unsigned int& height, const unsigned int& width);
	void printMap();
	int** mData;
	unsigned int mapWidth;
	unsigned int mapHeight;
	/*
	* int** mWallData;
	* int** mGroundData;
	* int** mCeilingData;
	*/
};

#endif