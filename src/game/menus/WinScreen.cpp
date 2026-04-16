#include <engine/ui/element.h>
#include <game/global_states.h>
#include <game/menus.h>

WinScreen::WinScreen(GEC::UI::ElementRenderer* elr, int coins, int timeRemaining)
{
    this->elr = elr;
    this->coins = coins;
    this->timeLeft = timeRemaining;
}
void WinScreen::Open()
{
    CreateElements();
    CreatePage();
}
void WinScreen::Update()
{
}
void WinScreen::Render()
{
}
void WinScreen::Events()
{
    if (homeButton->Clicked()) {
        GetManager()->GoTo(10);
    }
}
void WinScreen::Leave()
{
    delete homeButton;
    delete stats_coins;
    delete stats_time;
}

void WinScreen::CreateElements()
{

    homeButton = new GEC::UI::Elements::Button();
    tds = new GEC::UI::Elements::TextDisplay("Victory!");
    stats_coins = new GEC::UI::Elements::TextDisplay("Coins Collected: " + std::to_string(coins));
    stats_time = new GEC::UI::Elements::TextDisplay("Time Left: " + std::to_string(timeLeft) + "s");
}
void WinScreen::CreatePage()
{
    elr->ClearCycle();
    homeButton->SetFont(GetGlobalFont());
    homeButton->Set("Main Menu", 0, -80, 400, 50);

    tds->Set(0, 50, 0, 50);
    tds->SetColor(255);
    tds->Align(1, 1);
    tds->SetFont(GetGlobalFont());

    stats_coins->Set(0, 0, 0, 20);
    stats_coins->SetColor(255);
    stats_coins->Align(1, 1);
    stats_coins->SetFont(GetGlobalFont());

    stats_time->Set(0, -30, 0, 20);
    stats_time->SetColor(255);
    stats_time->Align(1, 1);
    stats_time->SetFont(GetGlobalFont());

    elr->AddElement(homeButton, 0);
    elr->AddElement(tds, 0);
    elr->AddElement(stats_coins, 0);
    elr->AddElement(stats_time, 0);
}
