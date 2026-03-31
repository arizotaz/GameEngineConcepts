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
#include <engine/audio.h>
#include <engine/renderobjects.h>
#include <engine/texture.h>
#include <engine/ui/element.h>
#include <game/global_states.h>
#include <game/tile.h>
#include <iostream>

GEC::MenuManager* mm;
GEC::UI::ElementRenderer* elr;

void Assignment2::Start()
{
    GEC::Input::Keyboard::PrintKeysToConsole(true);

    glutSetWindowTitle("Colton Staiduhar - 811138106");

    TileList::GetInstance().LoadTiles();

    mm = new GEC::MenuManager();
    elr = new GEC::UI::ElementRenderer();

    mm->AddMenu(0, new MainMenu(elr));
    mm->AddMenu(1, new GameScreen(elr));
    mm->AddMenu(2, new WinScreen(elr,0,0));
    mm->AddMenu(3, new LoseScreen(elr));

    GEC::TextureEngine::GetInstance().LoadTexture("box", RESOURCES_PATH "container.jpg");
    GEC::TextureEngine::GetInstance().LoadTexture("game.entities", RESOURCES_PATH "entities.png");
    GEC::TextureEngine::GetInstance().LoadTexture("game.finish.base", RESOURCES_PATH "Finish_Line_Base.png");
    GEC::TextureEngine::GetInstance().LoadTexture("game.finish.flag", RESOURCES_PATH "Finish_Line_Flag.png");

    GEC::AudioEngine::GetInstance().LoadSound("player.step", RESOURCES_PATH "audio/step.mp3");
    GEC::AudioEngine::GetInstance().LoadSound("player.jump", RESOURCES_PATH "audio/flap.mp3");
    GEC::AudioEngine::GetInstance().LoadSound("player.death", RESOURCES_PATH "audio/die.mp3");

    GEC::AudioEngine::GetInstance().LoadSound("finish_line", RESOURCES_PATH "audio/totally_not_happy_wheels.mp3");
    GEC::AudioEngine::GetInstance().LoadSound("BG_MUSIC", RESOURCES_PATH "audio/Portal Radio music uncompressed and HQ.mp3");
}
void Assignment2::Update()
{

    GEC::UI::ElementRegistry::GetInstance().Reset();

    elr->Update();
    mm->Update();
    elr->Interact();

    if (GEC::Input::Keyboard::IsKeyPressed(27))
        CloseCallBack();
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