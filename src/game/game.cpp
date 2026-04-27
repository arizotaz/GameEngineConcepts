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

#include <engine/audio.h>
#include <engine/input.h>
#include <engine/renderobjects.h>
#include <engine/texture.h>
#include <engine/ui/element.h>
#include <game/global_states.h>
#include <game/tile.h>
#include <iostream>

#include <game/gameeditor.h>

// MenuManager Pointer
GEC::MenuManager* mm;

// Applicaiton Element Renderer Pointer
GEC::UI::ElementRenderer* elr;

/** Start function of the Main Process */
void Assignment2::Start()
{

    // Set the Window Title
    glutSetWindowTitle("Colton Staiduhar - 811138106");

    // Call the load tiles
    TileList::GetInstance().LoadTiles();

    // Create the main game components
    mm = new GEC::MenuManager();
    elr = new GEC::UI::ElementRenderer();

    // Create the main menus of the application
    mm->AddMenu(0, new EngineBootScreen(elr));
    mm->AddMenu(3, new LoseScreen(elr));
    mm->AddMenu(5, new MainMenu(elr));
    mm->AddMenu(10, new EditorMenu(elr));
    // mm->AddMenu(10, new GameScreen(elr));

    // Load Game Textures
    GEC::TextureEngine::GetInstance().LoadTexture("box", RESOURCES_PATH "container.jpg");
    GEC::TextureEngine::GetInstance().LoadTexture("game.entities", RESOURCES_PATH "entities.png");
    GEC::TextureEngine::GetInstance().LoadTexture("game.finish.base", RESOURCES_PATH "Finish_Line_Base.png");
    GEC::TextureEngine::GetInstance().LoadTexture("game.finish.flag", RESOURCES_PATH "Finish_Line_Flag.png");

    // Load Game Audio
    GEC::AudioEngine::GetInstance().LoadSound("player.step", RESOURCES_PATH "audio/step.mp3");
    GEC::AudioEngine::GetInstance().LoadSound("player.jump", RESOURCES_PATH "audio/flap.mp3");
    GEC::AudioEngine::GetInstance().LoadSound("player.death", RESOURCES_PATH "audio/die.mp3");
    GEC::AudioEngine::GetInstance().LoadSound("finish_line", RESOURCES_PATH "audio/totally_not_happy_wheels.mp3");
    GEC::AudioEngine::GetInstance().LoadSound("BG_MUSIC", RESOURCES_PATH "audio/Portal Radio music uncompressed and HQ.mp3");
}

/** Main Processing loop of the process */
void Assignment2::Update()
{
    // Update Elements in the ElementRenderer
    elr->Update();

    // Update the MenuManager
    mm->Update();

    // Run the interact scripts of Elements in the ElementRenderer
    elr->Interact();

    // If the "esc" key is pressed end the main loop
    if (GEC::Input::Keyboard::IsKeyPressed(27))
        CloseCallBack();
}

/** Main Renderering Loop of the process */
void Assignment2::Render()
{
    // Update the camera with the window's viewport information
    GEC::Camera::GetInstance().SetScreen(
        GEC::Vector2<float, float>(
            glutGet(GLUT_WINDOW_WIDTH),
            glutGet(GLUT_WINDOW_HEIGHT)));

    // Clear the screen buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Run the menu manager
    mm->Render();

    // Render all elements in the Element Manager
    elr->Render();
}

/** Main Events loop of the Current Process */
void Assignment2::Events()
{
    // Run the event loop of the Menu Manager
    mm->Events();

    // Reset the Element Click Limit
    GEC::UI::ElementRegistry::GetInstance().Reset();
}
/** Called when the process exits */
void Assignment2::Exit()
{
    delete elr;
    delete mm;
}