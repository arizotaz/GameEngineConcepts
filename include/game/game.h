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

/**
 * Assignment2 Process Object
 */
class Assignment2 : public GEC::Process {

public:
    Assignment2() : Process(1) {}
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
};

#endif