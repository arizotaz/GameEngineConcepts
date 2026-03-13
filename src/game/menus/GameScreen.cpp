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

#include <engine/texture.h>
#include <game/camera.h>
#include <game/entities/objects.h>
#include <game/entities/player.h>
#include <game/gameprocessor.h>

LevelContainer* lc;

void GameScreen::Open()
{
    lc = new LevelContainer();

    lc->GetEntityManager()->Spawn(new Player(), 5, 5);
    lc->GetEntityManager()->Spawn(new Coin(), 3, 3);

    GEC::TextureEngine::GetInstance().LoadTexture("game.entities", RESOURCES_PATH "entities.png");
};
void GameScreen::Update()
{
    lc->Update();
    lc->Tick();
};
void GameScreen::Render()
{
    Camera& cam = Camera::GetInstance();

    cam.SetScale(cam.ViewPort().Second() / 15.0f);

    cam.SetScreen(
        GEC::Vector2<float, float>(
            glutGet(GLUT_WINDOW_WIDTH),
            glutGet(GLUT_WINDOW_HEIGHT)));
    GEC::Vector3<float, float, float>
        cPos
        = cam.Position();
    float scl = cam.GetScale();

    GEC::Render::SetColor(0, 255, 0,80);
    GEC::Render::Arrow(GEC::Vector3<float, float, float >(0,0,-1), GEC::Vector3<float, float, float >(0, cam.ViewPort().Second()/2-20, -1), 2, 20);
    GEC::Render::SetColor(255, 0, 0,80);
    GEC::Render::Arrow(GEC::Vector3<float, float, float >(0, 0, -1), GEC::Vector3<float, float, float >(cam.ViewPort().First()/2 -20, 0, -1), 2, 20);

    glPushMatrix();
    glScalef(scl, scl, 1);
    glTranslatef(-cPos.First(), -cPos.Second(), -cPos.Third());
    lc->Render();
    glPopMatrix();
};
void GameScreen::Events() { };

void GameScreen::Leave()
{
    delete lc;
};