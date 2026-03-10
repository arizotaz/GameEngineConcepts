#include <engine/renderobjects.h>

#include <game/menus.h>

#include <game/player.h>

void MainMenu::Open() {
    this->GetManager()->GoTo(1);
};
void MainMenu::Update() {};
void MainMenu::Render() {
    GEC::Render::SetColor(255,0,0);
    GEC::Render::Rect(0,0,100,200);
};
void MainMenu::Events() {};
void MainMenu::Leave() {};