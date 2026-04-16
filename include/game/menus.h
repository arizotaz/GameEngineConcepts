// #############################################################################
// # menus.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/10/2026
// # Last Modification:  02/11/2026
// #############################################################################
// # Main Entry point for Cmake project, declare by the int main() function
// #############################################################################

#ifndef MENUS_H
#define MENUS_H 1

#include <engine/structs.h>
#include <engine/ui/button.h>
#include <engine/ui/element.h>
#include <engine/ui/panels.h>
#include <engine/ui/textdisplay.h>
#include <game/levelcontainer.h>

class EngineBootScreen : public GEC::Menu {
public:
    EngineBootScreen(GEC::UI::ElementRenderer* elr) { };
    virtual void Open() override { };
    virtual void Update() override
    {
        this->GetManager()->GoTo(10);
    };
    virtual void Render() override { };
    virtual void Events() override { };
    virtual void Leave() override { };
};

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

    GEC::UI::Elements::TextDisplay* title;
    GEC::UI::Elements::Button* start;
    GEC::UI::Elements::Button* settings;
    GEC::UI::Elements::Button* about;
    GEC::UI::Elements::Button* quit;

    void CreatePage();
};

/** GameScreen Menu Declaration */
class GameScreen : public GEC::Menu {
public:
    GameScreen(GEC::UI::ElementRenderer*, LevelContainer*);
    virtual void Open() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void Events() override;
    virtual void Leave() override;

private:
    LevelContainer* lc;
    GEC::UI::ElementRenderer* elr;
};

class GAME_BlankMenu : public GEC::Menu {
public:
    GAME_BlankMenu();
    virtual void Open() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void Events() override;
    virtual void Leave() override;
};

class GAME_PauseMenu : public GEC::Menu {
public:
    GAME_PauseMenu(GEC::UI::ElementRenderer*, LevelContainer*, GEC::MenuManager*);
    virtual void Open() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void Events() override;
    virtual void Leave() override;

    void CreateElements();
    void CreatePage();

private:
    GEC::MenuManager* globalManager;

    GEC::UI::Elements::Button* backButton;
    GEC::UI::Elements::Button* editButton;

    GEC::UI::ElementRenderer* elr;
    LevelContainer* lc;
    GEC::UI::Elements::Panel* bgPanel;
    GEC::UI::Elements::TextDisplay* pausedText;
};

class WinScreen : public GEC::Menu {
public:
    WinScreen(GEC::UI::ElementRenderer*, int coins, int time);
    virtual void Open() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void Events() override;
    virtual void Leave() override;

    void CreateElements();
    void CreatePage();

private:
    int timeLeft, coins;

    GEC::UI::ElementRenderer* elr;
    GEC::UI::Elements::TextDisplay* tds;
    GEC::UI::Elements::TextDisplay* stats_coins;
    GEC::UI::Elements::TextDisplay* stats_time;

    GEC::UI::Elements::Button* homeButton;
};

class LoseScreen : public GEC::Menu {
public:
    LoseScreen(GEC::UI::ElementRenderer*);
    virtual void Open() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void Events() override;
    virtual void Leave() override;

    void CreateElements();
    void CreatePage();

private:
    GEC::UI::ElementRenderer* elr;
    GEC::UI::Elements::TextDisplay* tds;
    GEC::UI::Elements::Button* homeButton;
};

#endif