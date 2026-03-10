#include <game/gameprocessor.h>

LevelContainer::LevelContainer()
{
    lp = new LevelProcessor(this);
    lr = new LevelRenderer(this);
    em = new EntityManager(this);
    currentLevel = nullptr;
}

void LevelContainer::Update() {
    lp->Update();
    em->Update();
}
void LevelContainer::Render() {
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


LevelProcessor::LevelProcessor(LevelContainer* lc) {
    this->levelContainer = lc;
}
LevelProcessor::~LevelProcessor(){}
void LevelProcessor::Update() {
    Level* l = this->levelContainer->GetLevelData();
    if (!l) return;




}

LevelRenderer::LevelRenderer(LevelContainer* lc) {
    this->levelContainer = lc;
    
}
LevelRenderer::~LevelRenderer() {}
void LevelRenderer::Render() {
    Level* l = this->levelContainer->GetLevelData();
    if (!l) return;


    
}
