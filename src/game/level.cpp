#include <engine/serialization.h>
#include <game/level.h>
#include <iostream>
#include <math.h>

Level::Level(const char* name)
{
    this->name = name;
}
Level::Level(const Level& other)
{
    // Copy simple values
    width = other.width;
    height = other.height;
    layers = other.layers;

    // Deep copy name
    if (other.name) {
        size_t len = strlen(other.name) + 1;
        char* newName = new char[len];
        strcpy(newName, other.name);
        name = newName;
    } else {
        name = nullptr;
    }

    // Deep copy tiles
    int size = width * height * layers;
    tiles = new int[size];
    for (int i = 0; i < size; ++i) {
        tiles[i] = other.tiles[i];
    }

    // Deep copy layer data
    layerData = new Level_Layer*[layers];
    for (int i = 0; i < layers; ++i) {
        layerData[i] = new Level_Layer(*other.layerData[i]);
    }
}
Level& Level::operator=(const Level& other)
{
    if (this == &other)
        return *this;

    // Clean up existing memory
    delete[] tiles;

    for (int i = 0; i < layers; ++i)
        delete layerData[i];
    delete[] layerData;

    delete[] name;

    // Copy values
    width = other.width;
    height = other.height;
    layers = other.layers;

    // Copy name
    if (other.name) {
        size_t len = strlen(other.name) + 1;
        char* newName = new char[len];
        strcpy(newName, other.name);
        name = newName;
    } else {
        name = nullptr;
    }

    // Copy tiles
    int size = width * height * layers;
    tiles = new int[size];
    for (int i = 0; i < size; ++i) {
        tiles[i] = other.tiles[i];
    }

    // Copy layer data
    layerData = new Level_Layer*[layers];
    for (int i = 0; i < layers; ++i) {
        layerData[i] = new Level_Layer(*other.layerData[i]);
    }

    return *this;
}
Level* Level::Clone() const
{
    return new Level(*this);
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

    int s = width * height * layers;
    tiles = new int[s];
    for (int i = 0; i < s; ++i)
        tiles[i] = 0;
    layerData = new Level_Layer*[layers];

    for (int i = 0; i < layers; ++i)
        layerData[i] = new Level_Layer();

    layerData[0]->collidable = false;
    layerData[0]->zDepth = -4.0f;
    layerData[1]->zDepth = 1.0f;

    for (int x = 0; x < width; ++x) {
        SetTile(1, x, 0, 1);
        SetTile(1, x, 1, 1);
    }

    for (int x = 6; x < width; ++x) {
        for (int y = 0; y < 8; ++y) {
            SetTile(3, x, y, 0);
        }
    }

    SetTile(2, 8, 5, 1);
    SetTile(2, 9, 5, 1);
    SetTile(1, 10, 4, 1);
    SetTile(1, 10, 5, 1);
    SetTile(1, 11, 4, 1);
    SetTile(1, 11, 5, 1);
    SetTile(2, 12, 4, 1);
    SetTile(2, 13, 4, 1);
    SetTile(2, 14, 4, 1);
    SetTile(2, 15, 4, 1);
    SetTile(1, 16, 4, 1);

    for (int i = 2; i < 8; ++i) {
        SetTile(1, 20, i, 1);
        SetTile(1, 21, i, 1);
    }
}

int Level::GetTile(int index) const
{
    if (index >= width * height * layers)
        return -1;
    if (index < 0)
        return -1;
    return tiles[index];
}
int Level::GetTile(int x, int y, int layer) const
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

bool Level::SetTile(int tileID, int x, int y, int layer)
{
    int i = XYIndex(x, y, layer);
    if (!SetTile(tileID, i))
        return false;
    return GetTile(x, y, layer) == tileID;
}

int Level::XYIndex(int x, int y, int layer) const
{
    int w = this->width;
    int h = this->height;

    if (x < 0 || x >= w)
        return -1;
    if (y < 0 || y >= h)
        return -1;

    int rowMultiplier = y * w;
    int tileIndex = rowMultiplier + x;
    int layerOffset = w * h * layer;

    int index = layerOffset + tileIndex;

    if (index >= w * h * layers)
        index = -1;
    return index;
}

int Level::Width() const
{
    return width;
}
int Level::Height() const
{
    return height;
}
int Level::Layers() const
{
    return layers;
}

Level_Layer* Level::GetLayerData(int i)
{
    return this->layerData[i];
}

void Level::Serialize(std::ostream& out) const
{
    // Write level properties
    GEC::Serial::WriteString(out, this->name);
    GEC::Serial::Write(out, width);
    GEC::Serial::Write(out, height);
    GEC::Serial::Write(out, layers);

    // Write tile data
    int size = width * height * layers;
    out.write(reinterpret_cast<char*>(tiles), sizeof(int) * size);

    // Write layers
    for (int i = 0; i < layers; i++) {
        GEC::Serial::Write(out, layerData[i]->zDepth);
        GEC::Serial::Write(out, layerData[i]->collidable);
    }
}
void Level::Deserialize(std::istream& in)
{
    std::string nameStr;
    GEC::Serial::ReadString(in, nameStr);
    name = nameStr.c_str();


    GEC::Serial::Read(in, width);
    GEC::Serial::Read(in, height);
    GEC::Serial::Read(in, layers);

    int size = width * height * layers;
    tiles = new int[size];

    in.read(reinterpret_cast<char*>(tiles), sizeof(int) * size);

    layerData = new Level_Layer*[layers];
    for (int i = 0; i < layers; i++) {
        layerData[i] = new Level_Layer();

        GEC::Serial::Read(in, layerData[i]->zDepth);
        GEC::Serial::Read(in, layerData[i]->collidable);
    }
}