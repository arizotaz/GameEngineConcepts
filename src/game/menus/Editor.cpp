#include <game/gameeditor.h>
#include <engine/tools.h>

EditorMenu::EditorMenu(GEC::UI::ElementRenderer* elr)
{
    this->elr = elr;
}

void EditorMenu::Open()
{
    CreateElements();
    CreatePage();
}
void EditorMenu::Update() { }
void EditorMenu::Render() { }
void EditorMenu::Events() { }
void EditorMenu::Leave() { }

void EditorMenu::CreateElements()
{
    menuBar = new Editor_MenuBar();
    hierarchy = new Editor_Hierarchy();
}
void EditorMenu::CreatePage()
{
    Camera* cam = &Camera::GetInstance();
    GEC::Vector2<float, float> canvas = cam->ViewPort();

    float menuBarHeight = 40;
    float hierarchyWidth = 300;

    elr->ClearCycle();

    menuBar->Set(0, canvas.Second() / 2 - menuBarHeight/2, canvas.First(), menuBarHeight);
    hierarchy->Set(-canvas.First() / 2 + hierarchyWidth / 2, 0, hierarchyWidth, canvas.Second());

    // Add the menu bar to the renderer
    elr->AddElement(hierarchy, 0);
    elr->AddElement(menuBar, 0);
}

Editor_MenuBar::Editor_MenuBar()
{
    
}

Editor_MenuBar::~Editor_MenuBar()
{
    
}

void Editor_MenuBar::Update()
{
    GEC::UI::Elements::MouseInteractor::Update();
    float bSize = width/numOfElms;

    bSize = GEC::Tools::ClampVar<float>(bSize,50,150);

    float xIndex = -width/2 + bSize/2+5;
}
void Editor_MenuBar::Interact()
{
    
    GEC::UI::Elements::MouseInteractor::Interact();
}
void Editor_MenuBar::Render()
{
    GEC::Render::SetColor(120);
    GEC::Render::Rect(x, y, width, height);

    
}

Editor_Hierarchy::Editor_Hierarchy()
{
}
void Editor_Hierarchy::Update()
{
    GEC::UI::Elements::MouseInteractor::Update();
}
void Editor_Hierarchy::Interact()
{
    GEC::UI::Elements::MouseInteractor::Interact();
}
void Editor_Hierarchy::Render()
{

    GEC::Render::SetColor(200);
    GEC::Render::Rect(x, y, width, height);
}
