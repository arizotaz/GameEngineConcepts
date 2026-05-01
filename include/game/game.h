// #############################################################################
// # game.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/10/2026
// # Last Modification:  02/10/2026
// #############################################################################
// # Main Entry point for Cmake project, declare by the int main() function
// #############################################################################

#ifndef GAME_H
#define GAME_H 1

#include <engine/structs.h>
#include <engine/ui/element.h>

/**
 * Assignment2 Process Object
 */
class Assignment4 : public GEC::Process {

public:
    Assignment4()
        : Process(1)
    {
    }
    /** When Process Starts */
    void Start() override;

    /** Update Loop */
    void Update() override;

    /** Render Loop */
    void Render() override;

    /** Process GUI Events */
    void Events() override;

    /** On Exit */
    void Exit() override;

    void LoadLevel(std::string);

private:
    // MenuManager Pointer
    GEC::MenuManager* mm;

    // Applicaiton Element Renderer Pointer
    GEC::UI::ElementRenderer* elr;
};

class Editor : public GEC::Process {

public:
    Editor()
        : Process(2)
    {
    }
    /** When Process Starts */
    void Start() override;

    /** Update Loop */
    void Update() override;

    /** Render Loop */
    void Render() override;

    /** Process GUI Events */
    void Events() override;

    /** On Exit */
    void Exit() override;

private:
    // MenuManager Pointer
    GEC::MenuManager* mm;

    // Applicaiton Element Renderer Pointer
    GEC::UI::ElementRenderer* elr;
};

#endif