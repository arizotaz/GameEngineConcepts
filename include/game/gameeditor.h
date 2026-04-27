// #############################################################################
// # editor.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       04/01/2026
// # Last Modification:  04/04/2026
// #############################################################################
// # This header define the implementation of the editor, it's elements
// # interaction and controls
// #############################################################################

#include <engine/game_structs.h>
#include <engine/structs.h>
#include <engine/ui/element.h>
#include <engine/ui/elements.h>
#include <game/entities/objects.h>
#include <vector>


class Editor_MenuBar;
class Editor_StateBar;
class Editor_Hierarchy;
class EditorPanelSlot;
class EditorPanel;

/**
 * The Main Class that represents the editor
 * This is designed to just be a menu used by the MenuManager
 */
class EditorMenu : public GEC::Menu {
public:
    EditorMenu(GEC::UI::ElementRenderer*);
    virtual void Open() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void Events() override;
    virtual void Leave() override;

    void CreateElements();
    void CreatePage();

    void PlayGame();

    GEC::Game::Scene* Scene() const;

    void SetSelectedObj(GEC::Game::GameObject* obj)
    {
        this->selectedGameObject = obj;
    }
    GEC::Game::GameObject* GetSelectedObj() const
    {
        return selectedGameObject;
    }

    friend class EditorPanelSlot;

private:
    const float menuBarHeight = 20;
    const float stateBarHeight = 40;

    // Last Window Size
    GEC::Vector2<float, float> lastScreenSize;

    // Pointer to the applications ElementRenderer
    GEC::UI::ElementRenderer* elr;

    Editor_MenuBar* menuBar;
    Editor_StateBar* stateBar;

    // List of all available panel locations
    std::vector<EditorPanelSlot*> editorPanels;

    // The actively selected scene
    GEC::Game::Scene* activeScene = nullptr;

    // The selected game obj
    GEC::Game::GameObject* selectedGameObject = nullptr;
};

/**
 * The Menu bar of the application.  Used to provide specific, and global actions
 */
class Editor_MenuBar : public GEC::UI::Elements::MouseInteractor {
public:
    Editor_MenuBar();

    virtual void Update() override;
    virtual void Interact() override;
    virtual void Render() override;

    virtual ~Editor_MenuBar() override;

private:
    std::vector<GEC::Vector2<std::string, GEC::UI::Elements::ButtonOfButtons*>> menuButtons;
};

/**
 * The Menu bar of the application.  Used to provide specific, and global actions
 */
class Editor_StateBar : public GEC::UI::Elements::MouseInteractor {
public:
    Editor_StateBar(EditorMenu*);

    virtual void Update() override;
    virtual void Interact() override;
    virtual void Render() override;

    virtual ~Editor_StateBar() override;

    private:
    GEC::UI::Elements::Button* playButton = nullptr;
    EditorMenu* editorObj;
};

/**
 *
 */
class EditorPanelSlot : public GEC::UI::Elements::MouseInteractor {
public:
    EditorPanelSlot(EditorMenu* editor, int slotID);

    virtual void Update() override;
    virtual void Interact() override;
    virtual void Render() override;

    virtual ~EditorPanelSlot() override;

    friend class EditorMenu;
    friend class EditorPanel;

private:
    float tabHeight = 15;
    bool visible = true;
    int selectedIndex = 0;
    std::vector<GEC::UI::Elements::Button*> tabButtons;

    GEC::UI::Elements::Button left, right;
    std::vector<EditorPanel*> panels;

    EditorMenu* editor = nullptr;
    int slotID;
};

class EditorPanel : public GEC::UI::Elements::MouseInteractor {
public:
    EditorPanel() { };
    virtual void Update() override = 0;
    virtual void Interact() override = 0;
    virtual void Render() override = 0;

    virtual ~EditorPanel() override { };

    std::string GetName() const
    {
        return panelName;
    }

    void SetCurrectSlot(EditorPanelSlot* slot)
    {
        this->currentEditorSlot = slot;
    }

protected:
    int SlotID() const { return currentEditorSlot->slotID; }
    EditorMenu* EditorObject() const
    {
        return currentEditorSlot->editor;
    }

    std::string panelName;

    EditorPanelSlot* currentEditorSlot;
};

class Editor_Hierarchy : public EditorPanel {
public:
    Editor_Hierarchy();

    virtual void Update() override;
    virtual void Interact() override;
    virtual void Render() override;
    virtual ~Editor_Hierarchy() override;

private:
    std::vector<GEC::UI::Elements::Button*> objectList;
};
class Editor_Assets : public EditorPanel {
public:
    Editor_Assets();

    virtual void Update() override;
    virtual void Interact() override;
    virtual void Render() override;
    virtual ~Editor_Assets() override;

    private:
    std::vector<GEC::Game::GameObject*> objs;
    std::vector<GEC::UI::Elements::Button*> assetIcons;
};
class Editor_Properties : public EditorPanel {
public:
    Editor_Properties();

    virtual void Update() override;
    virtual void Interact() override;
    virtual void Render() override;
    virtual ~Editor_Properties() override;

    void CleanUp();
private:
    GEC::UI::Elements::Button* removeButton;
    const float axis = 2;
    GEC::Game::GameObject* lastGOBJ = nullptr;
    std::vector<GEC::UI::Elements::TextDisplay*> labels;
    GEC::UI::Elements::InputField* name;
    std::vector<GEC::UI::Elements::InputField*> positionInputs;
    std::vector<GEC::UI::Elements::InputField*> rotationInputs;
    std::vector<GEC::UI::Elements::InputField*> scaleInputs;

    GEC::Rect<float,float,float,float> objPropBounds;
};





class AssetObjectList {
    public:
    static AssetObjectList& GetInstance()
    {
        static AssetObjectList aol;
        return aol;
    }
    AssetObjectList(const AssetObjectList&) = delete;
    AssetObjectList& operator=(const AssetObjectList&) = delete;

    std::vector<GEC::Game::GameObject*> Objects() {
        return list;
    }

private:
    std::vector<GEC::Game::GameObject*> list;
    AssetObjectList()
    {
        list.push_back(new Coin());
        list.push_back(new FinishLine());
    }
    ~AssetObjectList() { 
        for (auto i : list)
        delete i;

    }
};