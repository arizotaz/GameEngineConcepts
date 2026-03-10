// #############################################################################
// # tile.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/09/2025
// # Last Modification:  03/09/2025
// #############################################################################
// # This header file serves as the level data structure.  It also implements
// # saving, loading, and modification of the level.
// #############################################################################

#ifndef GAME_TILE_H
#define GAME_TILE_H 1

#include <game/gameprocessor.h>
#include <engine/renderobjects.h>


class TileList;

class Tile {
public:
    Tile(int id);

    virtual void Render(LevelRenderer*, int x, int y) = 0;

    virtual ~Tile() { };

private:
    const int id;
};

class AirTile : public Tile {
public:
    AirTile()
        : Tile(0)
    {
    }
    void Render(LevelRenderer*, int x, int y) override { };
};

class BrickTile : public Tile {
public:
    BrickTile()
        : Tile(1)
    {
    }
    void Render(LevelRenderer*, int x, int y) override {
        GEC::Render::SetColor(100);
        GEC::Render::Rect(x,y,1,1);
        GEC::Render::SetColor(255);
        GEC::Render::Rect(x,y,0.9,0.9);
     };
};

class TileList {
public:
    static TileList& GetInstance()
    {
        static TileList list;
        return list;
    }

    Tile* GetTile(int index) const
    {
        return GAME_TILE_LIST[index];
    }

    void LoadTiles()
    {
        if (GAME_TILE_LIST != nullptr) {
            throw std::runtime_error("Tile list has already been initialized");
        } else {
            GAME_TILE_LIST = new Tile*[256];
            for (int i = 0; i < 256; ++i) GAME_TILE_LIST[i] = nullptr;
            
            
            new AirTile();
            new BrickTile();
        }
    }

    friend class Tile;

private:
    Tile** GAME_TILE_LIST;
    TileList()
    {
    }
    ~TileList()
    {
        if (GAME_TILE_LIST != nullptr) {
            for (int i = 256 - 1; i >= 0; --i)
                if (GAME_TILE_LIST[i] != nullptr) {
                    std::cout << "[Tiles] Removing tileID: " << i << std::endl;
                    delete GAME_TILE_LIST[i];
                }
            delete[] GAME_TILE_LIST;
        }
    }
};

#endif