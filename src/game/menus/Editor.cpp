#include <engine/renderobjects.h>
#include <engine/tools.h>
#include <engine/web.h>
#include <game/entities/objects.h>
#include <game/gameeditor.h>
#include <game/global_states.h>
#include <game/game_objects.h>

#include <game/menus.h>

bool drawEditorAxis = true;

EditorMenu::EditorMenu(GEC::UI::ElementRenderer* elr)
    : lastScreenSize(0, 0)
{
    this->elr = elr;
    activeScene = new GEC::Game::Scene();

    activeScene->AddObject(new TileRenderer());
    activeScene->AddObject(new Player(), 1, 2);
    activeScene->AddObject(new Coin(), 6, 5);
    activeScene->AddObject(new Coin(), 7, 8);
    activeScene->AddObject(new Coin(), 8, 8);
    activeScene->AddObject(new Coin(), 9, 8);
    activeScene->AddObject(new Coin(), 10, 8);
    activeScene->AddObject(new Coin(), 11, 8);
    activeScene->AddObject(new Coin(), 12, 8);

    activeScene->AddObject(new FinishLine(), 40, 2);
}
void EditorMenu::Open()
{
    CreateElements();
    CreatePage();
    activeScene->Start();
}
void EditorMenu::Update()
{

    GEC::Camera* cam = &GEC::Camera::GetInstance();
    GEC::Vector2<float, float> curScreen = cam->ViewPort();

    if (lastScreenSize.First() != curScreen.First() || lastScreenSize.Second() != curScreen.Second()) {
        lastScreenSize = curScreen;
        CreatePage();
    }

    activeScene->Update();

    if (true) {
        GEC::Camera& cam = GEC::Camera::GetInstance();
        float mSpeed = GetMainDeltaTime() / 100;
        if (GEC::Input::Keyboard::IsSpecialKeyDown(112))
            mSpeed *= 2;
        // UP
        if (GEC::Input::Keyboard::IsSpecialKeyDown(101))
            cam.Move(0, mSpeed);
        // Down
        if (GEC::Input::Keyboard::IsSpecialKeyDown(103))
            cam.Move(0, -mSpeed);
        // Left
        if (GEC::Input::Keyboard::IsSpecialKeyDown(100))
            cam.Move(-mSpeed, 0);
        // Right
        if (GEC::Input::Keyboard::IsSpecialKeyDown(102))
            cam.Move(mSpeed, 0);
    }

    if (GEC::Input::Keyboard::IsKeyPressed(97))
        drawEditorAxis = !drawEditorAxis;
}
void EditorMenu::Render()
{
    GEC::Camera& cam = GEC::Camera::GetInstance();

    cam.SetScale(cam.ViewPort().Second() / 15.0f);
    GEC::Vector3<float, float, float>
        cPos
        = cam.Position();
    float scl = cam.GetScale();

    glPushMatrix();
    glScalef(scl, scl, 1);
    glTranslatef(-cPos.First(), -cPos.Second(), -cPos.Third());
    activeScene->Render();
    glPopMatrix();

    if (drawEditorAxis) {
        GEC::Render::SetColor(0, 255, 0, 80);
        GEC::Render::Arrow(GEC::Vector3<float, float, float>(0, 0, 1), GEC::Vector3<float, float, float>(0, cam.ViewPort().Second() / 2 - 80, -1), 2, 20);
        GEC::Render::SetColor(255, 0, 0, 80);
        GEC::Render::Arrow(GEC::Vector3<float, float, float>(0, 0, 1), GEC::Vector3<float, float, float>(cam.ViewPort().First() / 2 - 250, 0, -1), 2, 20);
    }
}
void EditorMenu::Events()
{
    if (GEC::Input::Keyboard::IsKeyPressed(103))
        PlayGame();
}
void EditorMenu::Leave()
{
    this->selectedGameObject = nullptr;
}
void EditorMenu::CreateElements()
{
    if (menuBar != nullptr)
        return;

    menuBar = new Editor_MenuBar();
    stateBar = new Editor_StateBar(this);
    editorPanels.push_back(new EditorPanelSlot(this, 0));
    editorPanels.push_back(new EditorPanelSlot(this, 1));
    editorPanels.push_back(new EditorPanelSlot(this, 2));

    editorPanels[0]->panels.push_back(new Editor_Hierarchy());
    editorPanels[0]->panels[0]->SetCurrectSlot(editorPanels[0]);
    editorPanels[1]->panels.push_back(new Editor_Assets());
    editorPanels[1]->panels[0]->SetCurrectSlot(editorPanels[1]);
    editorPanels[2]->panels.push_back(new Editor_Properties());
    editorPanels[2]->panels[0]->SetCurrectSlot(editorPanels[2]);
}
void EditorMenu::CreatePage()
{
    elr->ClearCycle();
    GEC::Camera* cam = &GEC::Camera::GetInstance();
    GEC::Vector2<float, float> canvas = cam->ViewPort();

    /**
     * Set Panel Locations
     */

    float calW = 0;
    float calH = 0;

    menuBar->Set(0, canvas.Second() / 2 - menuBarHeight / 2, canvas.First(), menuBarHeight);
    stateBar->Set(0, canvas.Second() / 2 - menuBarHeight - stateBarHeight / 2, canvas.First(), stateBarHeight);

    calW = 200;
    calH = canvas.Second() - menuBarHeight - stateBarHeight;
    editorPanels[0]->Set(-canvas.First() / 2 + calW / 2, -canvas.Second() / 2 + calH / 2, calW, calH);

    calW = canvas.First() - editorPanels[0]->Width();
    calH = 300;
    editorPanels[1]->Set(-canvas.First() / 2 + editorPanels[0]->Width() + calW / 2, -canvas.Second() / 2 + calH / 2, calW, calH);

    calW = 200;
    calH = canvas.Second() - menuBarHeight - stateBarHeight - editorPanels[1]->Height();
    editorPanels[2]->Set(canvas.First() / 2 - calW / 2, -canvas.Second() / 2 + editorPanels[1]->Height() + calH / 2, calW, calH);

    /**
     *  Add Elements to draw
     */

    for (int i = 0; i < editorPanels.size(); ++i)
        elr->AddElement(editorPanels[i], 0);
    elr->AddElement(stateBar, 0);
    elr->AddElement(menuBar, 0);
}
void EditorMenu::PlayGame()
{
    GEC::MenuManager* mm = this->GetManager();
    mm->RemoveMenu(20);
    LevelContainer* lc = new LevelContainer();

    // Copy Level
    TileRenderer* tr = dynamic_cast<TileRenderer*>(this->activeScene->Objects()[0]);
    lc->SetLevel(tr->LevelData()->Clone());

    std::vector<GEC::Game::GameObject*> obj = this->activeScene->Objects();
    for (auto i : obj) {
        Entity* e = dynamic_cast<Entity*>(i);
        if (e) {
            Entity* en = dynamic_cast<Entity*>(e->Clone());

            if (dynamic_cast<Player*>(en)) {
                Player* p = dynamic_cast<Player*>(en);
                lc->SetPlayer(p);
                lc->GetEntityManager()->Spawn(lc->GetPlayer(), i->Position()->First(), i->Position()->Second());
                continue;
            }

            if (dynamic_cast<FinishLine*>(en)) {
                FinishLine* fl = dynamic_cast<FinishLine*>(en);
                fl->SetLevelContainer(lc);
                lc->GetEntityManager()->Spawn(fl, i->Position()->First(), i->Position()->Second());
                continue;
            }
            lc->GetEntityManager()->Spawn(en, i->Position()->First(), i->Position()->Second());
        }
    }

    mm->AddMenu(20, new GameScreen(elr, lc));

    if (GEC::Input::Keyboard::IsKeyPressed(103))
        mm->GoTo(5);
    else
        mm->GoTo(20);
}
GEC::Game::Scene* EditorMenu::Scene() const { return activeScene; }

EditorPanelSlot::EditorPanelSlot(EditorMenu* editor, int slotID)
{
    this->editor = editor;
    this->slotID = slotID;
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

    float bWidth = GEC::Tools::ClampVar<float>((width - tabHeight * 2) / tabButtons.size(), 0, 300);
    for (int i = 0; i < tabButtons.size(); ++i) {
        tabButtons[i]->Set(panels[i]->GetName(), x - width / 2 + bWidth / 2 + bWidth * i, y + height / 2 - tabHeight / 2, bWidth, tabHeight);
        tabButtons[i]->Update();
        if (i == selectedIndex)
            tabButtons[i]->SetButtonColor(255, 255, 255);
        else
            tabButtons[i]->SetButtonColor(200, 200, 200);
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

    left.Set("<", x + width / 2 - tabHeight / 2 - tabHeight, y + height / 2 - tabHeight / 2, tabHeight, tabHeight);
    right.Set(">", x + width / 2 - tabHeight / 2, y + height / 2 - tabHeight / 2, tabHeight, tabHeight);

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
            int nP = slotID - 1;
            if (nP < 0)
                nP = editor->editorPanels.size() - 1;
            EditorPanelSlot* nPanel = editor->editorPanels[nP];

            // Change panels
            EditorPanel* panel = panels[i];
            panels.erase(panels.begin() + i);
            nPanel->panels.push_back(panel);
            panel->SetCurrectSlot(nPanel);
            nPanel->selectedIndex = nPanel->panels.size() - 1;
        }

        // Move the selected panel to the right
        if (right.Clicked()) {

            // Get current panel
            int i = this->selectedIndex;

            // Get new panel slot
            int nP = slotID + 1;
            if (nP >= editor->editorPanels.size())
                nP = 0;
            EditorPanelSlot* nPanel = editor->editorPanels[nP];

            // Change panels
            EditorPanel* panel = panels[i];
            panels.erase(panels.begin() + i);
            nPanel->panels.push_back(panel);
            panel->SetCurrectSlot(nPanel);
            nPanel->selectedIndex = nPanel->panels.size() - 1;
        }
    }

    if (panels.size() <= 0)
        visible = false;
    else
        selectedIndex = GEC::Tools::ClampVar<int>(selectedIndex, 0, panels.size() - 1);
}
void EditorPanelSlot::Render()
{
    if (!visible)
        return;
    GEC::Render::SetColor((float)0);
    GEC::Render::Rect(x, y, width, height);
    GEC::Render::SetColor(120);
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

    GEC::Game::Scene* scene = this->EditorObject()->Scene();

    std::vector<GEC::Game::GameObject*> objects = scene->Objects();

    if (objects.size() != objectList.size()) {
        for (int i = 0; i < objectList.size(); ++i) {
            delete objectList[i];
        }
        objectList.clear();

        while (objectList.size() < objects.size()) {
            objectList.push_back(new GEC::UI::Elements::Button());
        }
    }

    float yIndex = height / 2;
    float itemSize = 20;
    for (int i = 0; i < objectList.size(); ++i) {
        if (objects[i] != nullptr) {
            yIndex -= itemSize / 2;
            objectList[i]->Set(objects[i]->GetName(), x, y + yIndex, width - 4, itemSize - 2);
            yIndex -= itemSize / 2;
        }
    }
}
void Editor_Hierarchy::Interact()
{
    for (int i = 0; i < objectList.size(); ++i) {
        objectList[i]->Interact();
        if (objectList[i]->Clicked()) {
            EditorObject()->SetSelectedObj(this->EditorObject()->Scene()->Objects()[i]);
        }
    }

    GEC::UI::Elements::MouseInteractor::Interact();
}
void Editor_Hierarchy::Render()
{
    GEC::Render::SetColor(200);
    GEC::Render::Rect(x, y, width, height);
    for (int i = 0; i < objectList.size(); ++i) {
        objectList[i]->Render();
    }
}
Editor_Hierarchy::~Editor_Hierarchy()
{
}

Editor_Assets::Editor_Assets()
{
    panelName = "Assets";
    objs = AssetObjectList::GetInstance().Objects();
    for (int i = 0; i < objs.size(); ++i) {
        assetIcons.push_back(new GEC::UI::Elements::Button());
    }
}
void Editor_Assets::Update()
{
    GEC::UI::Elements::MouseInteractor::Update();

    GEC::Vector2<float, float> iInd(x - width / 2, y + height / 2);

    float iconSize = 40;
    iInd.Move(iconSize / 2 + 2, -iconSize / 2 - 5);

    for (int i = 0; i < assetIcons.size(); ++i) {
        assetIcons[i]->Update();
        assetIcons[i]->Set(iInd.First(), iInd.Second(), iconSize, iconSize);
        iInd.Move(iconSize + 5, 0);
    }
}
void Editor_Assets::Interact()
{
    for (int i = 0; i < assetIcons.size(); ++i) {
        assetIcons[i]->Interact();
        if (assetIcons[i]->Clicked()) {
            GEC::Game::GameObject* gc = objs[i]->Clone();
            gc->Scale()->Set(1, 1);
            EditorObject()->Scene()->AddObject(gc, 0, 0);
        }
    }

    GEC::UI::Elements::MouseInteractor::Interact();
}
void Editor_Assets::Render()
{
    GEC::Render::SetColor(200);
    GEC::Render::Rect(x, y, width, height);

    for (int i = 0; i < assetIcons.size(); ++i) {
        assetIcons[i]->Render();
        objs[i]->Position()->Set(assetIcons[i]->X(), assetIcons[i]->Y());
        objs[i]->Scale()->Set(assetIcons[i]->Width(), assetIcons[i]->Height());

        Entity* e = dynamic_cast<Entity*>(objs[i]);
        if (e) {
            objs[i]->Scale()->Set(assetIcons[i]->Width() * (1 / e->RawSize().First()), assetIcons[i]->Height() * (1 / e->RawSize().Second()));
        }

        objs[i]->Draw();
    }
}
Editor_Assets::~Editor_Assets()
{
}

Editor_Properties::Editor_Properties()
    : objPropBounds(0, 0, 0, 0)
{
    panelName = "Properties";
    for (int i = 0; i < axis; ++i)
        positionInputs.push_back(new GEC::UI::Elements::InputField());
    for (int i = 0; i < axis; ++i)
        rotationInputs.push_back(new GEC::UI::Elements::InputField());
    for (int i = 0; i < axis; ++i)
        scaleInputs.push_back(new GEC::UI::Elements::InputField());
    labels.push_back(new GEC::UI::Elements::TextDisplay("Position"));
    labels.push_back(new GEC::UI::Elements::TextDisplay("Rotation"));
    labels.push_back(new GEC::UI::Elements::TextDisplay("Scale"));
    for (int i = 0; i < axis; ++i)
        labels[i]->Align(0, 0);

    name = new GEC::UI::Elements::InputField();
    removeButton = new GEC::UI::Elements::Button();
}
void Editor_Properties::Update()
{

    GEC::Game::GameObject* cObj = EditorObject()->GetSelectedObj();
    if (EditorObject()->GetSelectedObj() != nullptr && lastGOBJ != cObj) {
        lastGOBJ = cObj;

        name->SetValue(EditorObject()->GetSelectedObj()->GetName());

        positionInputs[0]->SetValue(std::to_string(cObj->Position()->First()));
        positionInputs[1]->SetValue(std::to_string(cObj->Position()->Second()));

        rotationInputs[0]->SetValue(std::to_string(cObj->Rotation()->First()));
        rotationInputs[1]->SetValue(std::to_string(cObj->Rotation()->Second()));

        scaleInputs[0]->SetValue(std::to_string(cObj->Scale()->First()));
        scaleInputs[1]->SetValue(std::to_string(cObj->Scale()->Second()));
    }

    float yIndex = height / 2;
    GEC::UI::Elements::MouseInteractor::Update();

    int iheight = 20;
    int size;

    // Object name
    yIndex -= iheight / 2;
    name->Set(x, y + yIndex, width, iheight);
    yIndex -= iheight / 2;
    if (EditorObject()->GetSelectedObj() != nullptr && !EditorObject()->GetSelectedObj()->Persistent()) {

        yIndex -= iheight / 2;
        removeButton->Set("Delete Object", x, y + yIndex, width, iheight);
        yIndex -= iheight / 2;
    }

    yIndex -= iheight;

    // Position
    labels[0]->Set(x - width / 2 + 2, y + yIndex, width, iheight);
    yIndex -= iheight / 2;
    size = (width - 4) / positionInputs.size();
    for (int i = 0; i < positionInputs.size(); ++i)
        positionInputs[i]->Set(x - width / 2 + 2 + size / 2 + size * i, y + yIndex, size, iheight);
    yIndex -= iheight * 2;

    // Rotation
    labels[1]->Set(x - width / 2 + 2, y + yIndex, width, iheight);
    yIndex -= iheight / 2;
    size = (width - 4) / rotationInputs.size();
    for (int i = 0; i < rotationInputs.size(); ++i)
        rotationInputs[i]->Set(x - width / 2 + 2 + size / 2 + size * i, y + yIndex, size, iheight);
    yIndex -= iheight * 2;

    // Scale
    labels[2]->Set(x - width / 2 + 2, y + yIndex, width, iheight);
    yIndex -= iheight / 2;
    size = (width - 4) / scaleInputs.size();
    for (int i = 0; i < scaleInputs.size(); ++i)
        scaleInputs[i]->Set(x - width / 2 + 2 + size / 2 + size * i, y + yIndex, size, iheight);
    yIndex -= iheight;

    float propH = (yIndex + height / 2) - 2;
    this->objPropBounds.X() = x;
    this->objPropBounds.Y() = y - height / 2 + propH / 2 + 4;
    this->objPropBounds.W() = width - 4;
    this->objPropBounds.H() = propH;

    name->Update();
    removeButton->Update();
    for (auto i : positionInputs)
        i->Update();
    for (auto i : rotationInputs)
        i->Update();
    for (auto i : scaleInputs)
        i->Update();
    for (auto i : labels)
        i->Update();

    if (EditorObject()->GetSelectedObj() != nullptr)
        EditorObject()->GetSelectedObj()->UpdatePropertiesPanel(objPropBounds.X(), objPropBounds.Y(), objPropBounds.W(), objPropBounds.H());
}
void Editor_Properties::Interact()
{

    bool reload = false;
    bool changed;

    if (EditorObject()->GetSelectedObj() == nullptr)
        return;

    try {
        name->Interact();
        if (name->Changed()) {
            reload = true;
            EditorObject()->GetSelectedObj()->SetName(name->GetValue());
        }

        changed = false;
        for (auto i : positionInputs) {
            i->Interact();
            if (i->Changed())
                changed = true;
        }
        if (changed) {
            reload = true;
            EditorObject()->GetSelectedObj()->Position()->Set(std::stof(positionInputs[0]->GetValue()), std::stof(positionInputs[1]->GetValue()));
        }

        changed = false;
        for (auto i : rotationInputs) {
            i->Interact();
            if (i->Changed())
                changed = true;
        }
        if (changed) {
            reload = true;
            EditorObject()->GetSelectedObj()->Rotation()->Set(std::stof(rotationInputs[0]->GetValue()), std::stof(rotationInputs[1]->GetValue()));
        }

        changed = false;
        for (auto i : scaleInputs) {
            i->Interact();
            if (i->Changed())
                changed = true;
        }
        if (changed) {
            reload = true;
            EditorObject()->GetSelectedObj()->Scale()->Set(std::stof(scaleInputs[0]->GetValue()), std::stof(scaleInputs[1]->GetValue()));
        }
    } catch (std::exception e) {
        reload = true;
    }

    EditorObject()->GetSelectedObj()->InteractPropertiesPanel(objPropBounds.X(), objPropBounds.Y(), objPropBounds.W(), objPropBounds.H());

    if (!EditorObject()->GetSelectedObj()->Persistent()) {
        removeButton->Interact();
        if (removeButton->Clicked()) {
            EditorObject()->Scene()->RemoveObject(EditorObject()->GetSelectedObj());
            reload = true;
            EditorObject()->SetSelectedObj(nullptr);
        }
    }

    if (reload)
        this->lastGOBJ = nullptr;

    GEC::UI::Elements::MouseInteractor::Interact();
}
void Editor_Properties::Render()
{
    GEC::Render::SetColor(200);
    GEC::Render::Rect(x, y, width, height);

    if (this->EditorObject()->GetSelectedObj() == nullptr)
        return;

    name->Render();
    if (EditorObject()->GetSelectedObj() != nullptr && !EditorObject()->GetSelectedObj()->Persistent())
        removeButton->Render();
    for (auto i : positionInputs)
        i->Render();
    for (auto i : rotationInputs)
        i->Render();
    for (auto i : scaleInputs)
        i->Render();
    for (auto i : labels)
        i->Render();

    if (EditorObject()->GetSelectedObj() != nullptr)
        EditorObject()->GetSelectedObj()->RenderPropertiesPanel(objPropBounds.X(), objPropBounds.Y(), objPropBounds.W(), objPropBounds.H());
}
Editor_Properties::~Editor_Properties()
{
    CleanUp();
}

void Editor_Properties::CleanUp()
{
    for (auto i : positionInputs)
        delete i;
    for (auto i : rotationInputs)
        delete i;
    for (auto i : scaleInputs)
        delete i;
    for (auto i : labels)
        delete i;

    delete name;
}

Editor_MenuBar::Editor_MenuBar()
{
    GEC::Vector2<std::string, GEC::UI::Elements::ButtonOfButtons*> fileOption("File", new GEC::UI::Elements::ButtonOfButtons());
    fileOption.Second()->AddOption("Exit", CloseCallBack);
    menuButtons.push_back(fileOption);

    GEC::Vector2<std::string, GEC::UI::Elements::ButtonOfButtons*> helpMenu("Help", new GEC::UI::Elements::ButtonOfButtons());
    helpMenu.Second()->AddOption("Controls", []() {
        std::cout << "Game Controls" << std::endl;
        std::cout << "Player Move: Arrow Keys + Spacebar" << std::endl;
        std::cout << "Pause: p" << std::endl;
        std::cout << "Close App: esc" << std::endl;
        std::cout << "Toggle Axises: a" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Editor Controls" << std::endl;
        std::cout << "Camera Move: arrow keys" << std::endl;
        std::cout << "Camera Fast: shift" << std::endl;
        std::cout << "Start from Title: g" << std::endl;
        std::cout << "Toggle Axises: a" << std::endl;
        std::cout << "PS: GLUT is weird, use fn+delete instead of backspace for text inputs" << std::endl;
        std::cout << "" << std::endl;
    });
    helpMenu.Second()->AddOption("About", []() {
        std::cout << "This is a simple game with an editor included.  The game will eventually take on the form of a small indie game called \"MicroMages\", look up the video about it, it's really cool.  However this is just a simple platformer with an editor to modify the tilemap and place entities on the level." << std::endl;
        std::cout << "You can see more about MicroMages here: https://www.youtube.com/watch?v=ZWQ0591PAxM" << std::endl;
    });
    helpMenu.Second()->AddOption("Support", []() {
        OpenWebURL("https://www.youtube.com/watch?v=Y2nEje0JGdQ");
    });
    menuButtons.push_back(helpMenu);
}
Editor_MenuBar::~Editor_MenuBar()
{
    for (int i = 0; i < menuButtons.size(); ++i)
        delete menuButtons[i].Second();
}
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
    GEC::Render::SetColor(255);
    GEC::Render::Rect(x, y, width, height);
    for (int i = 0; i < menuButtons.size(); ++i)
        menuButtons[i].Second()->Render();
}

Editor_StateBar::Editor_StateBar(EditorMenu* editor)
{
    this->editorObj = editor;
    this->playButton = new GEC::UI::Elements::Button();
}
Editor_StateBar::~Editor_StateBar()
{
    delete playButton;
}
void Editor_StateBar::Update()
{
    GEC::UI::Elements::MouseInteractor::Update();
    playButton->Set("Play", x, y, height / 2 * 3, height / 2);
    playButton->Update();
}
void Editor_StateBar::Interact()
{
    playButton->Interact();
    GEC::UI::Elements::MouseInteractor::Interact();

    if (playButton->Clicked())
        editorObj->PlayGame();
}
void Editor_StateBar::Render()
{
    GEC::Render::SetColor(120);
    GEC::Render::Rect(x, y, width, height);
    playButton->Render();
}
