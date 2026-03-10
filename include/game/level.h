// #############################################################################
// # level.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/09/2025
// # Last Modification:  03/09/2025
// #############################################################################
// # This header file serves as the level data structure.  It also implements
// # saving, loading, and modification of the level.
// #############################################################################

#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H 1

struct Level_Layer {
    bool isSolid = 0;
    int zDepth = 0;
    bool parallax = 0;
};

class Level {
public:
    Level(const char* name);

    void Init(int width, int height, int layers);

    /**
     * Reads in all values of the level file
     */
    void Read(const char* location);

    /**
     * Writes the contents of the level file
     */
    void Write(const char* location);

    /**
     * Returns the tile at the specified index
     */
    int GetTile(int index) const;

    /**
     * Returns the tile and the specified (x,y) position
     */
    int GetTile(int x, int y, int layer) const;

    /**
     * Sets the value of a tile at the specified index
     */
    bool SetTile(int id, int index);

    /**
     * Sets the value of a tile at the specified (x,y) value
     */
    bool SetTile(int id, int x, int y, int layer);

    int Width() const;
    int Height() const;
    int Layers() const;

    ~Level();

private:
    /**
     * Returns the Index in the tile list at the given XY position, returns -1 for null
     */
    int XYIndex(int x, int y, int layer) const;

    const char* name;
    int width, height, layers;
    Level_Layer** layerData;
    int* tiles;
};

#endif