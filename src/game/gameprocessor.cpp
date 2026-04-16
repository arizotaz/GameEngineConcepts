#include <engine/camera.h>
#include <engine/renderobjects.h>
#include <engine/texture.h>
#include <engine/tools.h>
#include <game/deltatime.h>
#include <game/gameprocessor.h>
#include <game/levelcontainer.h>
#include <game/tile.h>

LevelContainer::LevelContainer()
{
    lp = new LevelProcessor(this);
    lr = new LevelRenderer(this);
    em = new EntityManager(this);
    currentLevel = nullptr;
    lc_mm = new GEC::MenuManager();

    timeRemaining = 255;
}

void LevelContainer::SetLevel(Level* level) {
    currentLevel = level;
}

void LevelContainer::Update()
{
    lp->Update();
    em->Update();
    lc_mm->Update();
    lc_mm->Events();
}
void LevelContainer::Tick()
{
    lp->Tick();
    em->Tick();
    if (!GameWon())
        timeRemaining -= GetMainDeltaTime() / 1000.0f;
}
void LevelContainer::Render()
{
    lr->Render();
    em->Render();
    lc_mm->Render();
}

LevelContainer::~LevelContainer()
{
    delete lp;
    delete lr;
    delete em;
    delete currentLevel;
    delete lc_mm;
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
GEC::MenuManager* LevelContainer::GetMenuManager()
{
    return this->lc_mm;
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
LevelRenderer::LevelRenderer(Level* level)
{
    this->level = level;
    if (!GEC::TextureEngine::GetInstance().Exists("game.tiles"))
        GEC::TextureEngine::GetInstance().LoadTexture("game.tiles", RESOURCES_PATH "tiles.png");
}
LevelRenderer::~LevelRenderer() { }
void LevelRenderer::Render()
{
    if (this->levelContainer != nullptr)
        level = this->levelContainer->GetLevelData();
    Level* l = level;
    if (!l)
        return;

    for (int i = 0; i < l->Layers(); ++i) {
        Level_Layer* layerData = l->GetLayerData(i);
        if (layerData->visible)
            DrawLayer(i, layerData->zDepth);
    }
}

void LevelRenderer::DrawLayer(int layer, int z)
{
    if (this->levelContainer != nullptr)
        level = this->levelContainer->GetLevelData();
    Level* l = level;
    if (!l)
        return;

    GEC::Vector3<float, float, float> camPos = Camera::GetInstance().Position();
    int cx = round(camPos.First());
    int cy = round(camPos.Second());

    Camera& cam = Camera::GetInstance();

    int renderDistance = GEC::Tools::ClampVar<float>(cam.ViewPort().First() / cam.GetScale(), 2, 100);
    for (int yo = -renderDistance; yo <= renderDistance; ++yo)
        for (int xo = -renderDistance; xo <= renderDistance; ++xo) {
            int posX = cx + xo;
            int posY = cy + yo;

            if ((posX >= 0 && posX < l->Width()) && posY >= 0 && posY < l->Height()) {
                int tile = l->GetTile(posX, posY, layer);
                if (tile > 0) {
                    Tile* t = TileList::GetInstance().GetTile(tile);
                    if (t == nullptr) {
                        GEC::Render::SetColor(255);
                        GEC::Render::Image("engine::err", posX, posY, 1, 1);
                    } else {
                        t->Render(this, posX, posY, z);
                    }
                }
            }
        }
}

LevelContainer* LevelRenderer::Container() const
{
    return this->levelContainer;
}

Level* LevelRenderer::GetLevel() const
{
    return this->level;
}
