#ifndef MAP_HPP
#define MAP_HPP
#include <memory>
#include <string_view>
#include <vector>


class Map {
private:
    std::unique_ptr<uint32_t> mData;
    uint32_t mMapWidth;
    uint32_t mMapHeight;

public:
    Map();

    Map(uint32_t* mapData, uint32_t mapWidth, uint32_t mapHeight)
        : mData(mapData), mMapWidth(mapWidth), mMapHeight(mapHeight) {};

    Map(std::vector<uint32_t>&& mapData, uint32_t mapWidth, uint32_t mapHeight)
        : mData(mapData.data()), mMapWidth(mapWidth), mMapHeight(mapHeight) {};

    ~Map();

    uint32_t getSection(size_t section);

    uint32_t getSection(size_t x, size_t y);

    uint32_t& operator[](size_t section);

    uint32_t getHeight();

    uint32_t getWidth();

    void setSection(uint32_t x, uint32_t y, uint32_t val);

    void storeToFile(std::string_view path);

    void readFromFile(std::string_view path);

    void EditorCLI(uint32_t width, uint32_t height);

    int EditorWindow(uint32_t screenWidth, uint32_t screenHeight);

    void printMap();
};

#endif