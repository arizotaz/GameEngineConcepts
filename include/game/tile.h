// #############################################################################
// # tile.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/09/2026
// # Last Modification:  03/09/2026
// #############################################################################
// # This header file serves as the level data structure.  It also implements
// # saving, loading, and modification of the level.
// #############################################################################

#ifndef GAME_TILE_H
#define GAME_TILE_H 1

#include <engine/renderobjects.h>
#include <engine/structs.h>
#include <game/gameprocessor.h>
#include <game/levelcontainer.h>
#include <math.h>

class TileList;

class Tile {
public:
    Tile(int id);

    virtual void Render(LevelRenderer*, int x, int y, int z) = 0;

    virtual ~Tile() { };

    bool SolidTop() const { return solidTop; }
    bool Solid() const { return solid; }
    const int ID() const { return id; }

protected:
    bool solid = false;
    bool solidTop = false;

private:
    const int id;
};

class AirTile : public Tile {
public:
    AirTile()
        : Tile(0)
    {
        this->solid = false;
        this->solidTop = false;
    }
    void Render(LevelRenderer*, int x, int y, int z) override { };
};

class BrickTile : public Tile {
public:
    BrickTile()
        : Tile(1)
    {
        this->solid = true;
        this->solidTop = false;
    }
    void Render(LevelRenderer*, int x, int y, int z) override
    {

        int ix = 0;
        int iy = 0;

        if (fmod(((float)x - 1.0f) / 2.0f, 2.0f) >= 1)
            ix += 2;

        if (((x % 2) != 0)) {
            ix += 1;
        }
        if (((y % 2) == 0)) {
            iy += 1;
        }

        GEC::Render::SetColor(255);
        GEC::Render::Sprite("game.tiles", x, y, z, 1, 1, GEC::Vector2<int, int>(ix, iy), 16);
    };
};

class WoodPlatform : public Tile {
public:
    WoodPlatform()
        : Tile(2)
    {
        this->solid = false;
        this->solidTop = true;
    }
    void Render(LevelRenderer* r, int x, int y, int z) override
    {
        Level* level = r->GetLevel();

        int iniImgX = 0;
        int iniImgY = 2;

        int left = 0, right = 0;

        int dir = 1;

        if (level != nullptr) {
            left = level->GetTile(x - 1, y, z);
            right = level->GetTile(x + 1, y, z);
        }

        if ((left == 0 && right != 0) || (left == ID() && right != 0))
            dir = -1;

        int ix = iniImgX;
        int iy = iniImgY;
        if (((x % 2) == 0)) {
            ix += 1;
        }

        if ((left == ID() && right != ID()) || (right == ID() && left != ID())) {
            iy = iniImgY + 1;
            ix = iniImgX;
        }
        if ((left == 0 && right == ID()) || (right == 0 && left == ID())) {
            iy = iniImgY + 1;
            ix = iniImgX + 1;
        }

        GEC::Render::SetColor(255);
        GEC::Render::Sprite("game.tiles", x, y, z, 1 * dir, 1, GEC::Vector2<int, int>(ix, iy), 16);
    };
};

class BackgroundCastleWall : public Tile {
public:
    BackgroundCastleWall()
        : Tile(3)
    {
        this->solid = false;
        this->solidTop = true;
    }
    void Render(LevelRenderer* r, int x, int y, int z) override
    {
        Level* level = r->GetLevel();

        int ix = 0;
        int iy = 16 - 2;
        int secy = x / 2;
        int secx = y / 2;
        if (((x % 2) == 0)) {
            ix += 1;
        }
        if (((y % 2) == 0)) {
            iy += 1;
        }
        if (((secy % 2) == 0)) {
            iy += 2;
        }
        if (((secx % 2) == 0)) {
            ix = 2;
        }

        GEC::Render::SetColor(255);
        GEC::Render::Sprite("game.tiles", x, y, z, 1, 1, GEC::Vector2<int, int>(ix, iy), 16);
    };
};

class CrackedTile : public Tile {
public:
    CrackedTile()
        : Tile(4)
    {
        this->solid = true;
        this->solidTop = false;
    }
    void Render(LevelRenderer*, int x, int y, int z) override
    {

        int ix = 4;
        int iy = 0;

        if (((x % 2) != 0)) {
            ix += 1;
        }
        if (((y % 2) == 0)) {
            iy += 1;
        }

        GEC::Render::SetColor(255);
        GEC::Render::Sprite("game.tiles", x, y, z, 1, 1, GEC::Vector2<int, int>(ix, iy), 16);
    };
};
class ObsidianTile : public Tile {
public:
    ObsidianTile()
        : Tile(5)
    {
        this->solid = true;
        this->solidTop = false;
    }
    void Render(LevelRenderer*, int x, int y, int z) override
    {

        int ix = 6;
        int iy = 0;

        if (fmod(((float)x - 1.0f) / 2.0f, 2.0f) >= 1)
            ix += 2;

        if (((x % 2) != 0)) {
            ix += 1;
        }
        if (((y % 2) == 0)) {
            iy += 1;
        }

        GEC::Render::SetColor(255);
        GEC::Render::Sprite("game.tiles", x, y, z, 1, 1, GEC::Vector2<int, int>(ix, iy), 16);
    };
};
class CrackedBGTile : public Tile {
public:
    CrackedBGTile()
        : Tile(6)
    {
        this->solid = false;
        this->solidTop = true;
    }
    void Render(LevelRenderer* r, int x, int y, int z) override
    {
        int ix = 3;
        int iy = 16-2;

        if (((x % 2) != 0)) {
            ix += 1;
        }
        if (((y % 2) == 0)) {
            iy += 1;
        }

        GEC::Render::SetColor(255);
        GEC::Render::Sprite("game.tiles", x, y, z, 1, 1, GEC::Vector2<int, int>(ix, iy), 16);
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
            for (int i = 0; i < 256; ++i)
                GAME_TILE_LIST[i] = nullptr;

            new AirTile();
            new BrickTile();
            new WoodPlatform();
            new BackgroundCastleWall();
            new CrackedTile();
            new CrackedBGTile();
            new ObsidianTile();
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