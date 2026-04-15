#include <engine/renderobjects.h>
#include <engine/tools.h>
#include <engine/web.h>
#include <game/gameeditor.h>
#include <game/global_states.h>

EditorMenu::EditorMenu(GEC::UI::ElementRenderer* elr)
    : lastScreenSize(0, 0)
{
    this->elr = elr;
    activeScene = new GEC::Game::Scene();
}
void EditorMenu::Open()
{
    CreateElements();
    CreatePage();
    activeScene->Start();
}
void EditorMenu::Update()
{

    Camera* cam = &Camera::GetInstance();
    GEC::Vector2<float, float> curScreen = cam->ViewPort();

    if (lastScreenSize.First() != curScreen.First() || lastScreenSize.Second() != curScreen.Second()) {
        lastScreenSize = curScreen;
        CreatePage();
    }

    activeScene->Update();
}
void EditorMenu::Render() { 
    activeScene->Render();
}
void EditorMenu::Events() { }
void EditorMenu::Leave() {

 }
void EditorMenu::CreateElements()
{
    menuBar = new Editor_MenuBar();
    editorPanels.push_back(new EditorPanelSlot(this,0));
    editorPanels.push_back(new EditorPanelSlot(this,1));
    editorPanels.push_back(new EditorPanelSlot(this,2));

    editorPanels[0]->panels.push_back(new Editor_Hierarchy());
    editorPanels[1]->panels.push_back(new Editor_Assets());
    editorPanels[2]->panels.push_back(new Editor_Properties());
}
void EditorMenu::CreatePage()
{
    elr->ClearCycle();
    Camera* cam = &Camera::GetInstance();
    GEC::Vector2<float, float> canvas = cam->ViewPort();

    /**
     * Set Panel Locations
     */

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
    helpMenu.Second()->AddOption("Support", []() {
        OpenWebURL("https://arizotaz.com/contact/kent/gameengineconcepts");
    });
    menuButtons.push_back(helpMenu);
}
Editor_MenuBar::~Editor_MenuBar() { }
void Editor_MenuBar::Update()
{
    GEC::UI::Elements::MouseInteractor::Update();
    float bSize = width / menuButtons.size();

    bSize = GEC::Tools::ClampVar<float>(bSize, 50, 150);

    float xIndex = -width / 2 + bSize / 2;

    for (int i = 0; i < menuButtons.size(); ++i) {
        GEC::Vector2<std::string, GEC::UI::Elements::ButtonOfButtons*> option = menuButtons[i];
        option.Second()->Set(option.First(), x + xIndex, y, bSize, height);
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

EditorPanelSlot::EditorPanelSlot(EditorMenu* editor, int slotID) {
    this->editor = editor;
    this-> slotID = slotID;
 }
EditorPanelSlot::~EditorPanelSlot() { }
void EditorPanelSlot::Update()
{

    if (tabButtons.size() != panels.size()) {
        for (int i = 0; i < tabButtons.size(); ++i)
            delete tabButtons[i];
        tabButtons.clear();
        for (int i = 0; i < panels.size(); ++i)
            tabButtons.push_back(new GEC::UI::Elements::Button());
    }

    float bWidth = GEC::Tools::ClampVar<float>((width-tabHeight*2) / tabButtons.size(), 0, 300);
    for (int i = 0; i < tabButtons.size(); ++i) {
        tabButtons[i]->Set(panels[i]->GetName(), x - width / 2 + bWidth / 2 + bWidth * i, y + height / 2 - tabHeight / 2, bWidth, tabHeight);
        tabButtons[i]->Update();
        if (i == selectedIndex) tabButtons[i]->SetButtonColor(255,255,255);
        else tabButtons[i]->SetButtonColor(200,200,200);
    }

    visible = panels.size() > 0;
    if (visible) {
        while (selectedIndex >= panels.size()) {
            selectedIndex--;
        }

        EditorPanel* p = panels[selectedIndex];
        int nHeight = height - tabHeight;
        p->Set(x, y - height / 2 + nHeight / 2, width, nHeight);
        p->Update();
    }

    left.Set("<",x+width/2-tabHeight/2-tabHeight,y+height/2-tabHeight/2,tabHeight,tabHeight);
    right.Set(">",x+width/2-tabHeight/2,y+height/2-tabHeight/2,tabHeight,tabHeight);

    left.Update();
    left.Update();
}
void EditorPanelSlot::Interact()
{
    if (visible) {
        left.Interact();
        right.Interact();
        for (int i = 0; i < tabButtons.size(); ++i) {
            tabButtons[i]->Interact();
            if (tabButtons[i]->Clicked()) {
                selectedIndex = i;
            }
        }


        std::cout << panels.size() << "\n";
        int p_size = panels.size();
        while (selectedIndex >= p_size) {
            --selectedIndex;
        }
        if (selectedIndex > -1)
            panels[selectedIndex]->Interact();
        GEC::UI::Elements::MouseInteractor::Interact();


        // Move the selected panel to the left
        if (left.Clicked()) {

            // Get current panel
            int i = this->selectedIndex;

            // Get new panel slot
            int nP = slotID-1;
            if (nP < 0) nP = editor->editorPanels.size()-1;
            EditorPanelSlot* nPanel = editor->editorPanels[nP];

            // Change panels
            EditorPanel* panel = panels[i];
            panels.erase(panels.begin() + i);
            nPanel->panels.push_back(panel);
            
        }

        // Move the selected panel to the right
        if (right.Clicked()) {

            // Get current panel
            int i = this->selectedIndex;

            // Get new panel slot
            int nP = slotID+1;
            if (nP >= editor->editorPanels.size()) nP = 0;
            EditorPanelSlot* nPanel = editor->editorPanels[nP];

            // Change panels
            EditorPanel* panel = panels[i];
            panels.erase(panels.begin() + i);
            nPanel->panels.push_back(panel);
        }
    }

    if (panels.size() <= 0) visible = false;
    else selectedIndex = GEC::Tools::ClampVar<int>(selectedIndex,0,panels.size()-1);
}
void EditorPanelSlot::Render()
{
    if (!visible)
        return;
    GEC::Render::SetColor((float)0);
    GEC::Render::Rect(x, y, width, height);
    GEC::Render::SetColor(200);
    GEC::Render::Rect(x, y, width - 2, height - 2);

    if (panels.size() > 0) {
        panels[selectedIndex]->Render();
    }

    for (int i = 0; i < tabButtons.size(); ++i) {
        tabButtons[i]->Render();
    }

    left.Render();
    right.Render();
}

Editor_Hierarchy::Editor_Hierarchy()
{
    panelName = "Hierarchy";
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
    GEC::Render::SetColor(255,0,0);
    GEC::Render::Rect(x, y, width, height);
}
Editor_Hierarchy::~Editor_Hierarchy()
{
}

Editor_Assets::Editor_Assets()
{
    panelName = "Assets";
}
void Editor_Assets::Update()
{
    GEC::UI::Elements::MouseInteractor::Update();
}
void Editor_Assets::Interact()
{
    GEC::UI::Elements::MouseInteractor::Interact();
}
void Editor_Assets::Render()
{
    GEC::Render::SetColor(0,255,0);
    GEC::Render::Rect(x, y, width, height);
}
Editor_Assets::~Editor_Assets()
{
}

Editor_Properties::Editor_Properties()
{
    panelName = "Properties";
}
void Editor_Properties::Update()
{
    GEC::UI::Elements::MouseInteractor::Update();
}
void Editor_Properties::Interact()
{
    GEC::UI::Elements::MouseInteractor::Interact();
}
void Editor_Properties::Render()
{
    GEC::Render::SetColor(0,0,255);
    GEC::Render::Rect(x, y, width, height);
}
Editor_Properties::~Editor_Properties()
{
}

