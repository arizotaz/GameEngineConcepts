#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H 1

#include <engine/game_structs.h>
#include <game/gameprocessor.h>
#include <game/level.h>

class TileRenderer : public GEC::Game::GameObject {
public:
    TileRenderer()
        : GameObject("ocm.arizotaz.tilerenderer")
    {
        this->name = "Untitled Tile Renderer";
        levelData = new Level("EditorLevel");
        levelData->Init(100, 100, 2);
        lr = new LevelRenderer(levelData);
    }
    void Start() { }
    void Update() { }
    void Render()
    {
        lr->Render();
    }
    ~TileRenderer() { }

protected:
    LevelRenderer* lr;
    Level* levelData;
};

#endif