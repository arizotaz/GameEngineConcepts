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
#include <game/entities/objects.h>
#include <engine/texture.h>
#include <game/camera.h>

LevelContainer* lc;

void GameScreen::Open()
{
    lc = new LevelContainer();

    lc->GetEntityManager()->Spawn(new Player(), 5,5);

    GEC::TextureEngine::GetInstance().LoadTexture("game.entities", RESOURCES_PATH "entities.png");
    
};
void GameScreen::Update()
{
    lc->Update();
    lc->Tick();
};
void GameScreen::Render()
{

    GEC::Render::ClearScreen();


    Camera& cam = Camera::GetInstance();
    
    cam.SetScale(cam.ViewPort().Second()/15.0f);

    cam.SetScreen(
        GEC::Vector2<float, float>(
            glutGet(GLUT_WINDOW_WIDTH),
            glutGet(GLUT_WINDOW_HEIGHT)));
    GEC::Vector3<float, float, float>
        cPos
        = cam.Position();
    float scl = cam.GetScale();
    glPushMatrix();
    
    GEC::Render::SetColor(173, 245, 255);
    GEC::Render::Rect(0,0,cam.ViewPort().First(),cam.ViewPort().Second());

    
    glScalef(scl, scl, 0);
    glTranslatef(-cPos.First(), -cPos.Second(), -cPos.Third());
    lc->Render();
    glPopMatrix();

};
void GameScreen::Events() { };

void GameScreen::Leave() {
    delete lc;
};