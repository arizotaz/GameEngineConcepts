// #############################################################################
// # editor.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       04/01/2026
// # Last Modification:  04/01/2026
// #############################################################################
// # This header define the implementation of the editor, it's elements
// # interaction and controls
// #############################################################################

#include <engine/structs.h>
#include <engine/ui/element.h>
#include <engine/ui/button.h>
#include <engine/ui/panels.h>
#include <engine/ui/textdisplay.h>

class Editor_MenuBar;
class Editor_Hierarchy;

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
    GEC::UI::ElementRenderer* elr;
    
    
    Editor_MenuBar* menuBar;
    Editor_Hierarchy* hierarchy;
};

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

class Editor_Panel : public GEC::UI::Elements::MouseInteractor {
    public:
    Editor_Panel();

    virtual void Update() override;
    virtual void Interact() override;
    virtual void Render() override;
    
};


class Editor_Hierarchy : public GEC::UI::Elements::MouseInteractor {
    public:
    Editor_Hierarchy();

    virtual void Update() override;
    virtual void Interact() override;
    virtual void Render() override;
    
};