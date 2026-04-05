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

#include <engine/structs.h>
#include <engine/ui/button.h>
#include <engine/ui/element.h>
#include <engine/ui/panels.h>
#include <engine/ui/textdisplay.h>
#include <vector>

class Editor_MenuBar;
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

private:
    // Last Window Size
    GEC::Vector2<float,float> lastScreenSize;

    // Pointer to the applications ElementRenderer
    GEC::UI::ElementRenderer* elr;


    Editor_MenuBar* menuBar;

    // List of all available panel locations
    std::vector<EditorPanelSlot*> editorPanels;
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
    GEC::UI::Element** elms;
    int numOfElms = 5;
};

/**
 *
 */
class EditorPanelSlot : public GEC::UI::Elements::MouseInteractor {
public:
    EditorPanelSlot();

    virtual void Update() override;
    virtual void Interact() override;
    virtual void Render() override;

    virtual ~EditorPanelSlot() override;

private:
    bool visible = true;
    std::vector<EditorPanel*> panels;
};


class EditorPanel : public GEC::UI::Elements::MouseInteractor {
    EditorPanel();
    virtual void Update() override;
    virtual void Interact() override;
    virtual void Render() override;

    virtual ~EditorPanel() override;
};

class Editor_Hierarchy : public GEC::UI::Elements::MouseInteractor {
public:
    Editor_Hierarchy();

    virtual void Update() override;
    virtual void Interact() override;
    virtual void Render() override;
};