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

#include <game/player.h>

#include <iostream>
#include <vector>

Player* p;

bool axes = true;
bool swapColors = false;
float scale = 1;

std::string current_song = "";

int songIndex = 0;
std::vector<std::string> songs {
    std::string("portal"),
    std::string("crazy_la_paint"),
    std::string("dirty")
};

bool music_pause = false;
void NextSong()
{
    GEC::AudioEngine::GetInstance().StopSound(current_song.c_str());

    ++songIndex;
    if (songIndex >= songs.size())
        songIndex = 0;

    current_song = songs[songIndex];

    if (!GEC::AudioEngine::GetInstance().PlaySound(current_song.c_str())) {
        NextSong();
    }
}

void GameScreen::Open()
{
    p = new Player(0, 0);

    // Used to print keys to console
    // GEC::Input::Keyboard::PrintKeysToConsole(true);

    GEC::AudioEngine::GetInstance().LoadSound("crazy_la_paint", RESOURCES_PATH "audio/CrazyLaPaint.mp3");
    GEC::AudioEngine::GetInstance().LoadSound("dirty", RESOURCES_PATH "audio/grandson - Dirty [OFFICIAL VIDEO].mp3");
    GEC::AudioEngine::GetInstance().LoadSound("portal", RESOURCES_PATH "audio/Portal Radio music uncompressed and HQ.mp3");
    current_song = songs[0];

    GEC::AudioEngine::GetInstance().PlaySound(current_song.c_str());
};
void GameScreen::Update()
{
    if (GEC::Input::Keyboard::IsKeyPressed('p')) {
        music_pause = !music_pause;
        if (music_pause) {
            GEC::AudioEngine::GetInstance().PauseSound(current_song.c_str());
        } else {
            GEC::AudioEngine::GetInstance().PlaySound(current_song.c_str());
        }
    }
    if (GEC::AudioEngine::GetInstance().SoundFinished(current_song.c_str())) {
        NextSong();
    }

    if (GEC::Input::Mouse::GetInstance().LeftPressed())
        swapColors = !swapColors;
    if (GEC::Input::Mouse::GetInstance().RightPressed())
        NextSong();

        if (GEC::Input::Keyboard::IsKeyPressed('a')) axes = !axes;

    p->Update();
};
void GameScreen::Render()
{
    // Clear the screen
    GEC::Render::ClearScreen();

    // Draw Graph Arrows
    if (axes) {
        GEC::Render::SetColor(0, 100, 0);
    GEC::Render::Arrow(GEC::Vector2<float, float>(0, 0), GEC::Vector2<float, float>(0, 300), 1, 10);
    GEC::Render::SetColor(100, 0, 0);
    GEC::Render::Arrow(GEC::Vector2<float, float>(0, 0), GEC::Vector2<float, float>(400, 0), 1, 10);
    }

    scale += GEC::Input::Mouse::GetInstance().ScrollY() / 50;

    glPushMatrix();
    glScalef(scale, scale, scale);

    // Static Objects
    if (!swapColors)
        GEC::Render::SetColor(0, 255, 0);
    else
        GEC::Render::SetColor(255, 0, 0);
    GEC::Render::Triangle(-200, 0,
        GEC::Vector2<float, float>(-50, -50),
        GEC::Vector2<float, float>(0, 50),
        GEC::Vector2<float, float>(50, -50), 0);

    if (!swapColors)
        GEC::Render::SetColor(255, 0, 0);
    else
        GEC::Render::SetColor(0, 255, 0);
    GEC::Render::Rect(200, 0, 100, 100);

    // GEC::Render::SetColor(255, 255, 255);
    // GEC::Render::Image("box", 0, 100, 100, 100);

    // Render Main Player
    p->Render();

    glPopMatrix();
};
void GameScreen::Events() { };

void GameScreen::Leave() { };