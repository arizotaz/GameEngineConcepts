#if defined(use_freeglut)
#include <glad/glad.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h>
#endif

#include <engine/structs.h>
#include <engine/ui/element.h>

#include <game/game.h>
#include <game/menus.h>

#include <engine/input.h>
#include <engine/renderobjects.h>
#include <engine/texture.h>
#include <game/global_states.h>
#include <iostream>
#include <engine/ui/element.h>

GEC::MenuManager* mm;
GEC::UI::ElementRenderer* elr;

void Assignment2::Start()
{
    glutSetWindowTitle("Colton Staiduhar - 811138106");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    mm = new GEC::MenuManager();
    elr = new GEC::UI::ElementRenderer();

    mm->AddMenu(0, new MainMenu(elr));
    mm->AddMenu(1, new GameScreen(elr));

    GEC::TextureEngine::GetInstance().LoadTexture("box", RESOURCES_PATH "container.jpg");
    GEC::TextureEngine::GetInstance().LoadTexture("game.entities", RESOURCES_PATH "entities.png");
}
void Assignment2::Update()
{

    GEC::UI::ElementRegistry::GetInstance().Reset();
    // ESC
    if (GEC::Input::Keyboard::IsKeyDown(27))
        CloseCallBack();

    elr->Update();
    mm->Update();
    elr->Interact();
}
void Assignment2::Render()
{
    Camera::GetInstance().SetScreen(
        GEC::Vector2<float, float>(
            glutGet(GLUT_WINDOW_WIDTH),
            glutGet(GLUT_WINDOW_HEIGHT)));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mm->Render();
    elr->Render();
}
void Assignment2::Events()
{
    mm->Events();
}
void Assignment2::Exit()
{
    delete elr;
    delete mm;
}