#include <engine/renderobjects.h>
#include <game/camera.h>
#include <game/gameprocessor.h>
#include <game/tile.h>
#include <engine/texture.h>

LevelContainer::LevelContainer()
{
    // Loads the tile list
    TileList::GetInstance().LoadTiles();

    lp = new LevelProcessor(this);
    lr = new LevelRenderer(this);
    em = new EntityManager(this);
    currentLevel = nullptr;

    currentLevel = new Level("test");
    currentLevel->Init(100, 100, 1);
}

void LevelContainer::Update()
{
    lp->Update();
    em->Update();
}
void LevelContainer::Tick()
{
    lp->Tick();
    em->Tick();
}
void LevelContainer::Render()
{
    lr->Render();
    em->Render();
}

LevelContainer::~LevelContainer()
{
    delete lp;
    delete lr;
    delete em;

    delete currentLevel;
}

Level* LevelContainer::GetLevelData()
{
    return this->currentLevel;
}
LevelProcessor* LevelContainer::GetLevelProcessor()
{
    return this->lp;
}
LevelRenderer* LevelContainer::GetLevelRenderer()
{
    return this->lr;
}
EntityManager* LevelContainer::GetEntityManager()
{
    return this->em;
}

LevelProcessor::LevelProcessor(LevelContainer* lc)
{
    this->levelContainer = lc;
}
LevelProcessor::~LevelProcessor() { }
void LevelProcessor::Update()
{
    Level* l = this->levelContainer->GetLevelData();
    if (!l)
        return;
}
void LevelProcessor::Tick()
{
    Level* l = this->levelContainer->GetLevelData();
    if (!l)
        return;
}

LevelRenderer::LevelRenderer(LevelContainer* lc)
{
    this->levelContainer = lc;

    if (!GEC::TextureEngine::GetInstance().Exists("game.tiles"))
        GEC::TextureEngine::GetInstance().LoadTexture("game.tiles", RESOURCES_PATH "tiles.png");

}
LevelRenderer::~LevelRenderer() { }
void LevelRenderer::Render()
{
    Level* l = this->levelContainer->GetLevelData();
    if (!l)
        return;

    GEC::Vector3<float, float, float> camPos = Camera::GetInstance().Position();
    int cx = round(camPos.First());
    int cy = round(camPos.Second());

    int renderDistance = 10;
    for (int yo = -renderDistance; yo <= renderDistance; ++yo)
        for (int xo = -renderDistance; xo <= renderDistance; ++xo) {
            Level* l = this->levelContainer->GetLevelData();

            int posX = cx + xo;
            int posY = cy + yo;

            if ((posX >= 0 && posX < l->Width()) && posY >= 0 && posY < l->Height()) {
                int tile = l->GetTile(posX, posY, 0);
                if (tile > -1) {

                    Tile* t = TileList::GetInstance().GetTile(tile);
                    if (t == nullptr) {
                        GEC::Render::SetColor(255);
                        GEC::Render::Image("engine::err", posX, posY, 1, 1);
                    } else {
                        t->Render(this, posX, posY);
                    }
                }
            }
        }
}
