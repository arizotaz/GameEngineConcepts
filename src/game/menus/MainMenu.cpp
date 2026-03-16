#include <engine/renderobjects.h>
#include <engine/ui/button.h>
#include <engine/ui/element.h>

#include <game/global_states.h>

#include <game/menus.h>

MainMenu::MainMenu(GEC::UI::ElementRenderer* _elr)
{
    elr = _elr;
}

void MainMenu::Open()
{
    start = new GEC::UI::Elements::Button();
    settings = new GEC::UI::Elements::Button();
    about = new GEC::UI::Elements::Button();
    quit = new GEC::UI::Elements::Button();

    CreatePage();
};
void MainMenu::Update() { };
void MainMenu::Render() { };
void MainMenu::Events() { 
    if (start->Clicked()) {
        this->GetManager()->GoTo(1);
    }
};
void MainMenu::Leave()
{
    delete start;
    delete settings;
    delete about;
    delete quit;
};

void MainMenu::CreatePage()
{
    float bw = 400;
    float bh = 40;
    float bp = 10;

    elr->ClearCycle();
    start->Set("Test",0, 0, bw, bh);
    elr->AddElement(start, 0);
}