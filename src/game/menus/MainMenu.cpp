#include <engine/renderobjects.h>
#include <engine/text.h>
#include <engine/ui/button.h>
#include <engine/ui/element.h>
#include <engine/ui/textdisplay.h>

#include <game/global_states.h>

#include <game/menus.h>

MainMenu::MainMenu(GEC::UI::ElementRenderer* _elr)
{
    elr = _elr;
}

void MainMenu::Open()
{
    title = new GEC::UI::Elements::TextDisplay("Assignment 2!");
    start = new GEC::UI::Elements::Button();
    settings = new GEC::UI::Elements::Button();
    about = new GEC::UI::Elements::Button();
    quit = new GEC::UI::Elements::Button();

    start->SetFont(GetGlobalFont());
    settings->SetFont(GetGlobalFont());
    about->SetFont(GetGlobalFont());
    quit->SetFont(GetGlobalFont());

    CreatePage();
};
void MainMenu::Update() {
    if (GEC::Input::Keyboard::IsKeyPressed(103))
    GetManager()->GoTo(1);
};
void MainMenu::Render()
{

};
void MainMenu::Events()
{
    if (start->Clicked())
        this->GetManager()->GoTo(1);


        if (quit->Clicked())
        CloseCallBack();
};
void MainMenu::Leave()
{
    elr->ClearCycle();
    delete title;
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

    title->SetFont(GetGlobalFont());
    title->Align(1,0);
    title->SetColor(255);
    title->Set(0,bh*2.5+bp*2,0,bh*2);

    elr->AddElement(title, 0);


    start->Set("Play (g)", 0, bh*1.5f+bp*1.5f, bw, bh);
    elr->AddElement(start, 0);
    settings->Set("Settings", 0, bh/2+bp/2, bw, bh);
    elr->AddElement(settings, 0);
    about->Set("About", 0, -(bh/2+bp/2), bw, bh);
    elr->AddElement(about, 0);
    quit->Set("Quit", 0, -(bh*1.5f+bp*1.5f), bw, bh);
    elr->AddElement(quit, 0);
    
}