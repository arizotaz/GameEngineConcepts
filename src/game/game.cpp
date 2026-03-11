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
#include <game/global_states.h>
#include <iostream>

GEC::MenuManager* mm;

void Assignment2::Start()
{
    glutSetWindowTitle("Colton Staiduhar - 811138106");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    mm = new GEC::MenuManager();

    mm->AddMenu(0, new MainMenu());
    mm->AddMenu(1, new GameScreen());

    GEC::TextureEngine::GetInstance().LoadTexture("box", RESOURCES_PATH "container.jpg");
    GEC::TextureEngine::GetInstance().LoadTexture("game.entities", RESOURCES_PATH "entities.png");
}
void Assignment2::Update()
{
    // ESC
    if (GEC::Input::Keyboard::IsKeyDown(27))
        CloseCallBack();

    mm->Update();
    mm->Events();
}
void Assignment2::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mm->Render();

    
    
}
void Assignment2::Exit()
{
    delete mm;
}