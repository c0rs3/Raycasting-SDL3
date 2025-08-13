#ifndef RAYCASTER_MAP
#define RAYCASTER_MAP

struct Map {
	Map() = default;

	/*
	* @brief Takes the ownership of the map data pointer.
	*
	* DO NOT DELETE the pointers used in the constructor.
	*/
	Map(int* map, unsigned int mapWidth, unsigned int mapHeight)
		: mData(map), mapWidth(mapWidth), mapHeight(mapHeight) {}

	~Map();
	void addMap(const unsigned int& height, const unsigned int& width);
	int UI(unsigned int screenWidth, unsigned int screenHeight);
	void printMap();
	int* mData;
	unsigned int mapWidth;
	unsigned int mapHeight;
	/*
	* int** mWallData;
	* int** mGroundData;
	* int** mCeilingData;
	*/
};

#endif