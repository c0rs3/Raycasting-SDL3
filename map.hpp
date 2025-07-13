struct Map {
	Map() = default;

	/*
	* @brief takes the ownership of the map data pointer
	*
	* DO NOT DELETE the pointers used in the constructor
	*/
	Map(int** map, unsigned int mapWidth, unsigned int mapHeight)
		: mData(map), mapWidth(mapWidth), mapHeight(mapHeight) {
	}

	~Map() {
		for (size_t i = 0; i < mapWidth; i++)
			delete[] mData[i];
		delete[] mData;
	}

	int** mData;
	unsigned int mapWidth;
	unsigned int mapHeight;
	// int** mFloorLayer;
	// int** mCeilingLayer;
	// int** mWallLayer;
};