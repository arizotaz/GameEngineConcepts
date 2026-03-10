#include <game/level.h>
#include <iostream>
#include <math.h>

Level::Level(const char* name)
{
    this->name = name;
}
Level::~Level()
{

    std::cout << "[Level] Removing Level " << name << std::endl;

    for (int i = layers - 1; i >= 0; --i) {
        std::cout << "[Level] Removing Layer Meta Data " << i << std::endl;
        delete layerData[i];
    }
    std::cout << "[Level] Layer Meta Data List " << std::endl;
    delete[] layerData;
    std::cout << "[Level] Tile List " << std::endl;
    delete[] tiles;
}

void Level::Init(int width, int height, int layers)
{
    this->width = width;
    this->height = height;
    this->layers = layers;

    tiles = new int[width * height * layers];
    for (int i = 0; i < layers; ++i)
        tiles[i] = 0;
    layerData = new Level_Layer*[layers];
}

int Level::GetTile(int index) const
{
    if (index >= width * height * layers)
        return -1;
    if (index < 0)
        return -1;
    return tiles[index];
}
int Level::GetTile(int layer, int x, int y) const
{
    int i = XYIndex(x, y, layer);
    if (i < 0)
        return -1;
    return GetTile(i);
}

bool Level::SetTile(int tileID, int index)
{
    if (index >= width * height * layers)
        return false;
    if (index < 0)
        return false;
    tiles[index] = tileID;
    return tiles[index] == tileID;
}

bool Level::SetTile(int tileID, int layer, int x, int y)
{
    int i = XYIndex(x, y, layer);
    if (!SetTile(tileID, i))
        return false;
    return GetTile(layer, x, y) == tileID;
}

int Level::XYIndex(int x, int y, int layer) const
{
    int w = this->width;
    int h = this->height;

    int rowMultiplier = y * w;

    int tileIndex = rowMultiplier + x;
    int layerOffset = w * h * layer;

    int index = layerOffset + tileIndex;

    if (index >= w * h * layers)
        index = -1;
    return index;
}