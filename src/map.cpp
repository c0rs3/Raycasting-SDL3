#include "map.h"
#include "raycasterutil.h"

Map::~Map() {
    for (size_t i = 0; i < mapWidth; i++)
        delete[] mData[i];
    delete[] mData;
}

void Map::addMap(const unsigned int& height, const unsigned int& width) {
    mapWidth = width;
    mapHeight = height;
    if (mData == nullptr) {
        mData = new int* [mapWidth];
        for (unsigned int i = 0; i < mapWidth; ++i)
            mData[i] = new int[mapHeight];
    }

    printMap();
    unsigned int inputVal;
    for (unsigned int i = 0; i < mapWidth; i++) {
        for (unsigned int k = 0; k < mapHeight; k++) {
            std::cin >> inputVal;
            mData[i][k] = inputVal;
            system("clear");
            printMap();
        }
    }
}

void Map::printMap() {
    for (unsigned int i = 0; i < mapWidth; i++) {
        std::cout << "{";
        for (unsigned int k = 0; k < mapHeight; k++) {
            std::cout << mData[i][k] << ((k == mapHeight - 1) ? "" : ", ");
        }
        std::cout << ((i == mapHeight - 1) ? "}" : "},") << std::endl;
    }
}
