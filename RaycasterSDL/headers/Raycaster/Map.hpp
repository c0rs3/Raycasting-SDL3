#pragma once

#include <memory>
#include <string_view>
#include <vector>
#include <iostream>

#define BRED    0b11111111'00000000'00000000'11111111
#define BBLUE   0b00000000'11111111'00000000'11111111
#define BGREEN  0b00000000'00000000'11111111'11111111
#define BWHITE  0b11111111'11111111'11111111'11111111
#define BBLACK  0b11111111'11111111'11111111'00000000
#define BORANGE 0b11111111'10110000'00000000'11111111 
#define BPURPLE 0b10011111'00110011'11110000'11111111 
#define BTEAL   0b00110011'11001100'11001100'11111111 
#define BOLIVE  0b10111111'10111111'00110000'11111111 
#define BMAROON 0b10000000'00000000'00110000'11111111 
#define BNAVY   0b00000000'00000000'10000011'11111111 
#define BSILVER 0b11001100'11001100'11001100'11111111 
#define BGRAY   0b10000000'10000000'10000000'11111111 
#define BTRANS  0b00000000'00000000'00000000'00000000

class Map {
private:
	std::vector<int32_t> mData;
public:
	uint32_t mMapWidth;
	uint32_t mMapHeight;
	
	Map(int32_t* mapData, uint32_t mapWidth, uint32_t mapHeight)
		: mData(mapData, mapData + (mapWidth * mapHeight)), mMapWidth(mapWidth),
		mMapHeight(mapHeight) {};

	Map(const std::vector<int32_t>& mapData, uint32_t mapWidth, uint32_t mapHeight)
		: mData(mapData), mMapWidth(mapWidth), mMapHeight(mapHeight) {};

	int32_t& getSection(size_t section);

	int32_t& getSection(size_t x, size_t y);

	int32_t& operator[](size_t section);

	uint32_t getHeight() const;

	uint32_t getWidth() const;

	void setSection(uint32_t x, uint32_t y, int32_t val);

	void setSection(uint32_t section, int32_t val);

	void storeToFile(std::string_view path);

	void readFromFile(std::string_view path);
};

void logMapToConsole(Map& map);

void EditorConsole(Map& map);

int EditorWindow(Map& map, uint32_t screenWidth, uint32_t screenHeight);

