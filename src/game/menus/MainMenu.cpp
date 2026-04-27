#include <engine/renderobjects.h>
#include <engine/text.h>
#include <engine/ui/elements.h>

#include <game/global_states.h>

#include <game/menus.h>

MainMenu::MainMenu(GEC::UI::ElementRenderer* _elr)
{
    elr = _elr;
}

void MainMenu::Open()
{
    title = new GEC::UI::Elements::TextDisplay("Assignment 3!");
    start = new GEC::UI::Elements::Button();
    editor = new GEC::UI::Elements::Button();
    quit = new GEC::UI::Elements::Button();

    CreatePage();
};
void MainMenu::Update()
{
    if (GEC::Input::Keyboard::IsKeyPressed(103))
        GetManager()->GoTo(20);
};
void MainMenu::Render() {

};
void MainMenu::Events()
{
    if (start->Clicked())
        this->GetManager()->GoTo(20);
if (editor->Clicked())
        this->GetManager()->GoTo(10);
    if (quit->Clicked())
        CloseCallBack();
};
void MainMenu::Leave()
{
    elr->ClearCycle();
    delete title;
    delete start;
    delete editor;
    delete quit;
};

void MainMenu::CreatePage()
{
    float bw = 400;
    float bh = 40;
    float bp = 10;

    elr->ClearCycle();

    title->SetFont(GetGlobalFont());
    title->Align(1, 0);
    title->SetColor(255);
    title->Set(0, bh * 2.5 + bp * 2, 0, bh * 2);

    elr->AddElement(title, 0);

    if (GetManager()->Exists(20)) {
        start->Set("Play (g)", 0, bh * 1.5f + bp * 1.5f, bw, bh);
        elr->AddElement(start, 0);
    }
    editor->Set("Open Editor", 0, bh / 2 + bp / 2, bw, bh);
    elr->AddElement(editor, 0);
    quit->Set("Quit", 0, -(bh * 1.5f + bp * 1.5f), bw, bh);
    elr->AddElement(quit, 0);
}