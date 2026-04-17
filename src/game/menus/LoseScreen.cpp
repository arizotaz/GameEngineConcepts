#include <engine/ui/element.h>
#include <engine/audio.h>
#include <game/global_states.h>
#include <game/menus.h>

LoseScreen::LoseScreen(GEC::UI::ElementRenderer* elr)
{
    this->elr = elr;
}
void LoseScreen::Open()
{
    GEC::AudioEngine::GetInstance().PlaySound("player.death");
    CreateElements();
    CreatePage();
}
void LoseScreen::Update()
{
}
void LoseScreen::Render()
{
}
void LoseScreen::Events()
{
    if (homeButton->Clicked())
        this->GetManager()->GoTo(10);
}
void LoseScreen::Leave()
{
    delete homeButton;
    delete tds;
}

void LoseScreen::CreateElements()
{
    homeButton = new GEC::UI::Elements::Button();
    tds = new GEC::UI::Elements::TextDisplay("You Died!");
}
void LoseScreen::CreatePage()
{
    elr->ClearCycle();
    homeButton->SetFont(GetGlobalFont());
    homeButton->Set("Main Menu", 0, -30, 400, 50);

    tds->Set(0, 30, 0, 50);
    tds->SetColor(255);
    tds->Align(1, 1);
    tds->SetFont(GetGlobalFont());

    elr->AddElement(homeButton, 0);
    elr->AddElement(tds, 0);
}
