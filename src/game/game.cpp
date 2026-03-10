#if defined(use_freeglut)
#include <glad/glad.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h>
#endif

#include <engine/structs.h>

#include <game/game.h>
#include <game/menus.h>

#include <engine/input.h>
#include <engine/renderobjects.h>
#include <engine/texture.h>
#include <iostream>
#include <game/global_states.h>

bool render = true;
int clearScreen = 0;

GEC::MenuManager* mm;

void Assignment1::Start()
{
    glutSetWindowTitle("Colton Staiduhar - 811138106");

    mm = new GEC::MenuManager();

    mm->AddMenu(0, new MainMenu());
    mm->AddMenu(1, new GameScreen());

    GEC::TextureEngine::GetInstance().LoadTexture("box", RESOURCES_PATH "container.jpg");
    GEC::TextureEngine::GetInstance().LoadTexture("game.entities", RESOURCES_PATH "entities.png");
}
void Assignment1::Update()
{
    // ESC
    if (GEC::Input::Keyboard::IsKeyDown(27))
        CloseCallBack();
    if (GEC::Input::Keyboard::IsKeyPressed('c')) {
        render = false;
        clearScreen = 4; // Screen the screen 4 times so we know both buffers are empty
    }
    if (GEC::Input::Keyboard::IsKeyPressed('m'))
        render = true;
        
    mm->Update();
    mm->Events();
}
void Assignment1::Render()
{
    // This system is to make sure the screen clears both buffers
    if (clearScreen > 0) {
        --clearScreen;
        GEC::Render::ClearScreen();
    }
    
    if (render)
        mm->Render();
}
void Assignment1::Exit()
{
    delete mm;
}