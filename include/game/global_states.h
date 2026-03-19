#ifndef GAME_GLOBAL_STATES
#define GAME_GLOBAL_STATES 1

#include <engine/text.h>

static GEC::TextRender::Font* globalFont = nullptr;
GEC::TextRender::Font* GetGlobalFont();

// GameRunning Flag
static bool gameRunning = true;

// Return the GameRunning value
bool GameRunning();

// On Window Close Callback
void CloseCallBack();


float FPS();
float FPS_AVERAGE();


#endif