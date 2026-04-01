// #############################################################################
// # deltatime.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/17/2026
// # Last Modification:  02/17/2026
// #############################################################################
// # DeltaTime Table - a list of static DeltaTime object to be accessed
// #                   anywhere in the program
// #############################################################################

#ifndef GAME_DELTATIME_TABLE
#define GAME_DELTATIME_TABLE 1

#include <engine/structs.h>

static GEC::DeltaTime* MainLoopDeltaTime = nullptr;

/** Returns the DeltaTime of the MainLoopDeltaTime Object */
float GetMainDeltaTime();
/** Runs the .Update() function of the MainLoopDeltaTime Object */
void UpdateMainDeltaTime();

#endif