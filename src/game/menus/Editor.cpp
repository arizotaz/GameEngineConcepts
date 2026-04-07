#include <engine/renderobjects.h>
#include <engine/tools.h>
#include <game/gameeditor.h>
#include <game/global_states.h>

#include <engine/web.h>

EditorMenu::EditorMenu(GEC::UI::ElementRenderer* elr)
    : lastScreenSize(0, 0)
{
    this->elr = elr;
}
void EditorMenu::Open()
{
    CreateElements();
    CreatePage();
}
void EditorMenu::Update()
{

    Camera* cam = &Camera::GetInstance();
    GEC::Vector2<float, float> curScreen = cam->ViewPort();

    if (lastScreenSize.First() != curScreen.First() || lastScreenSize.Second() != curScreen.Second()) {
        lastScreenSize = curScreen;
        CreatePage();
    }
}
void EditorMenu::Render() { }
void EditorMenu::Events() { }
void EditorMenu::Leave() { }
void EditorMenu::CreateElements()
{
    menuBar = new Editor_MenuBar();
    editorPanels.push_back(new EditorPanelSlot());
    editorPanels.push_back(new EditorPanelSlot());
    editorPanels.push_back(new EditorPanelSlot());
}
void EditorMenu::CreatePage()
{
    elr->ClearCycle();
    Camera* cam = &Camera::GetInstance();
    GEC::Vector2<float, float> canvas = cam->ViewPort();

    /**
     * Set Panel Locations
     */

    float menuBarHeight = 40;
    float calW = 0;
    float calH = 0;

    menuBar->Set(0, canvas.Second() / 2 - menuBarHeight / 2, canvas.First(), menuBarHeight);

    calW = 200;
    calH = canvas.Second() - menuBarHeight;
    editorPanels[0]->Set(-canvas.First() / 2 + calW / 2, -canvas.Second() / 2 + calH / 2, calW, calH);

    calW = canvas.First() - editorPanels[0]->Width();
    calH = 300;
    editorPanels[1]->Set(-canvas.First() / 2 + editorPanels[0]->Width() + calW / 2, -canvas.Second() / 2 + calH / 2, calW, calH);

    calW = 200;
    calH = canvas.Second() - menuBarHeight - editorPanels[1]->Height();
    editorPanels[2]->Set(canvas.First() / 2 - calW / 2, -canvas.Second() / 2 + editorPanels[1]->Height() + calH / 2, calW, calH);

    /**
     *  Add Elements to draw
     */

    for (int i = 0; i < editorPanels.size(); ++i)
        elr->AddElement(editorPanels[i], 0);
    elr->AddElement(menuBar, 0);
}

Editor_MenuBar::Editor_MenuBar()
{
    GEC::Vector2<std::string, GEC::UI::Elements::ButtonOfButtons*> fileOption("File", new GEC::UI::Elements::ButtonOfButtons());
    fileOption.Second()->AddOption("Exit", CloseCallBack);
    menuButtons.push_back(fileOption);

    GEC::Vector2<std::string, GEC::UI::Elements::ButtonOfButtons*> helpMenu("Help", new GEC::UI::Elements::ButtonOfButtons());
    helpMenu.Second()->AddOption("Controls", CloseCallBack);
    helpMenu.Second()->AddOption("About", CloseCallBack);
    helpMenu.Second()->AddOption("Support", CloseCallBack);
    menuButtons.push_back(helpMenu);
}
Editor_MenuBar::~Editor_MenuBar() { }
void Editor_MenuBar::Update()
{
    GEC::UI::Elements::MouseInteractor::Update();
    float bSize = width / menuButtons.size();

    bSize = GEC::Tools::ClampVar<float>(bSize, 50, 150);

    float xIndex = -width / 2 + bSize / 2 + 5;

    for (int i = 0; i < menuButtons.size(); ++i) {
        GEC::Vector2<std::string, GEC::UI::Elements::ButtonOfButtons*> option = menuButtons[i];
        option.Second()->Set(option.First(), x + xIndex, y, bSize, height - 10);
        option.Second()->Update();
        xIndex += bSize;
    }
}
void Editor_MenuBar::Interact()
{
    for (int i = 0; i < menuButtons.size(); ++i)
        menuButtons[i].Second()->Interact();
    GEC::UI::Elements::MouseInteractor::Interact();
}
void Editor_MenuBar::Render()
{
    GEC::Render::SetColor(120);
    GEC::Render::Rect(x, y, width, height);
    for (int i = 0; i < menuButtons.size(); ++i)
        menuButtons[i].Second()->Render();
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

EditorPanelSlot::EditorPanelSlot() { }
EditorPanelSlot::~EditorPanelSlot() { }
void EditorPanelSlot::Update() { }
void EditorPanelSlot::Interact()
{
    if (visible)
        GEC::UI::Elements::MouseInteractor::Interact();
}
void EditorPanelSlot::Render()
{
    if (!visible)
        return;
    GEC::Render::SetColor((float)0);
    GEC::Render::Rect(x, y, width, height);
    GEC::Render::SetColor(200);
    GEC::Render::Rect(x, y, width - 2, height - 2);
}

EditorPanel::EditorPanel() { }
EditorPanel::~EditorPanel() { }
void EditorPanel::Update() { }
void EditorPanel::Interact() { }
void EditorPanel::Render() { }