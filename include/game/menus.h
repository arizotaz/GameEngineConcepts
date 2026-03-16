// #############################################################################
// # menus.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/10/2025
// # Last Modification:  02/11/2025
// #############################################################################
// # Main Entry point for Cmake project, declare by the int main() function
// #############################################################################

#ifndef MENUS_H
#define MENUS_H 1

#include <engine/structs.h>
#include <engine/ui/element.h>
#include <engine/ui/button.h>

/** Main Menu Declaration */
class MainMenu : public GEC::Menu {
public:
    MainMenu(GEC::UI::ElementRenderer*);
    virtual void Open() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void Events() override;
    virtual void Leave() override;

private:
    GEC::UI::ElementRenderer* elr;

    GEC::UI::Elements::Button* start;
    GEC::UI::Elements::Button* settings;
    GEC::UI::Elements::Button* about;
    GEC::UI::Elements::Button* quit;

    void CreatePage();
};

/** GameScreen Menu Declaration */
class GameScreen : public GEC::Menu {
public:
    GameScreen(GEC::UI::ElementRenderer*);
    virtual void Open() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void Events() override;
    virtual void Leave() override;

private:
    GEC::UI::ElementRenderer* elr;
};

#endif