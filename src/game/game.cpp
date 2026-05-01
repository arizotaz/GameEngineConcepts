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

#include <game/game_objects.h>

void Assignment4::Start()
{
    // Set the Window Title
    glutSetWindowTitle("Colton Staiduhar - 811138106");

    // Call the load tiles
    TileList::GetInstance().LoadTiles();

    // Create the main game components
    mm = new GEC::MenuManager();
    elr = new GEC::UI::ElementRenderer();

    LevelContainer* lc = new LevelContainer();

    // Create the main menus of the application
    mm->AddMenu(0, new EngineBootScreen(elr, 10));
    mm->AddMenu(3, new LoseScreen(elr));
    mm->AddMenu(5, new MainMenu(elr));
    mm->AddMenu(10, new MainMenu(elr));

    LoadLevel("./game.dat");


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
void Assignment4::Update()
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
void Assignment4::Render()
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
void Assignment4::Events()
{
    // Run the event loop of the Menu Manager
    mm->Events();

    // Reset the Element Click Limit
    GEC::UI::ElementRegistry::GetInstance().Reset();
}
void Assignment4::Exit()
{
    delete elr;
    delete mm;
}
void Assignment4::LoadLevel(std::string sceneLocation) {

    GEC::Game::Scene* activeScene = nullptr;

    activeScene = new GEC::Game::Scene();
    std::ifstream in(sceneLocation, std::ios::binary);
    activeScene->Deserialize(in);
    in.close();

    mm->RemoveMenu(20);
    LevelContainer* lc = new LevelContainer();

    // Copy Level
    TileRenderer* tr = dynamic_cast<TileRenderer*>(activeScene->Objects()[0]);
    lc->SetLevel(tr->LevelData()->Clone());

    std::vector<GEC::Game::GameObject*> obj = activeScene->Objects();
    for (auto i : obj) {
        Entity* e = dynamic_cast<Entity*>(i);
        if (e) {
            Entity* en = dynamic_cast<Entity*>(e->Clone());

            if (dynamic_cast<Player*>(en)) {
                Player* p = dynamic_cast<Player*>(en);
                lc->SetPlayer(p);
                lc->GetEntityManager()->Spawn(lc->GetPlayer(), i->Position()->First(), i->Position()->Second());
                continue;
            }

            if (dynamic_cast<FinishLine*>(en)) {
                FinishLine* fl = dynamic_cast<FinishLine*>(en);
                fl->SetLevelContainer(lc);
                lc->GetEntityManager()->Spawn(fl, i->Position()->First(), i->Position()->Second());
                continue;
            }
            lc->GetEntityManager()->Spawn(en, i->Position()->First(), i->Position()->Second());
        }
    }

    mm->AddMenu(20, new GameScreen(elr, lc));
}

/** Start function of the Main Process */
void Editor::Start()
{

    // Set the Window Title
    glutSetWindowTitle("Colton Staiduhar - 811138106 (Editor)");

    // Call the load tiles
    TileList::GetInstance().LoadTiles();

    // Create the main game components
    mm = new GEC::MenuManager();
    elr = new GEC::UI::ElementRenderer();

    // Create the main menus of the application
    mm->AddMenu(0, new EngineBootScreen(elr, 10));
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
void Editor::Update()
{
    // Update Elements in the ElementRenderer
    elr->Update();

    // Update the MenuManager
    mm->Update();

    // Run the interact scripts of Elements in the ElementRenderer
    elr->Interact();
}

/** Main Renderering Loop of the process */
void Editor::Render()
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
void Editor::Events()
{
    // Run the event loop of the Menu Manager
    mm->Events();

    // Reset the Element Click Limit
    GEC::UI::ElementRegistry::GetInstance().Reset();
}
/** Called when the process exits */
void Editor::Exit()
{
    delete elr;
    delete mm;
}