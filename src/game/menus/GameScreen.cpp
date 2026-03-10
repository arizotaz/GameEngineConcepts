#if defined(use_freeglut)
#include <glad/glad.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h>
#endif

#include <engine/audio.h>
#include <engine/input.h>
#include <engine/renderobjects.h>
#include <engine/structs.h>
#include <game/menus.h>

#include <iostream>
#include <vector>

#include <game/gameprocessor.h>
#include <game/entities/player.h>
#include <engine/texture.h>


LevelContainer* lc;

void GameScreen::Open()
{
    lc = new LevelContainer();

    GEC::TextureEngine::GetInstance().LoadTexture("game.entities", RESOURCES_PATH "entities.png");
    
};
void GameScreen::Update()
{
    lc->Update();
};
void GameScreen::Render()
{
    GEC::Render::ClearScreen();
    lc->Render();
};
void GameScreen::Events() { };

void GameScreen::Leave() {
    delete lc;
};