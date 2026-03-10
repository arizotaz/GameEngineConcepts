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

#include <game/camera.h>

GEC::MenuManager* mm;

void Assignment2::Start()
{
    glutSetWindowTitle("Colton Staiduhar - 811138106");

    mm = new GEC::MenuManager();

    mm->AddMenu(0, new MainMenu());
    mm->AddMenu(1, new GameScreen());

    GEC::TextureEngine::GetInstance().LoadTexture("box", RESOURCES_PATH "container.jpg");
    GEC::TextureEngine::GetInstance().LoadTexture("game.entities", RESOURCES_PATH "entities.png");

    Camera::GetInstance().SetScale(50);
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
    Camera::GetInstance().SetScreen(
        GEC::Vector2<float, float>(
            glutGet(GLUT_WINDOW_WIDTH),
            glutGet(GLUT_WINDOW_HEIGHT)));
    GEC::Vector3<float, float, float>
        cPos
        = Camera::GetInstance().Position();
    float scl = Camera::GetInstance().GetScale();
    glPushMatrix();
    glScalef(scl, scl, 0);
    glTranslatef(-cPos.First(), -cPos.Second(), -cPos.Third());

    mm->Render();

    glPopMatrix();
}
void Assignment2::Exit()
{
    delete mm;
}