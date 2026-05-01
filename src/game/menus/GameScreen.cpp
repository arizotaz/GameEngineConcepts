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
#include <game/entities/objects.h>
#include <game/entities/player.h>
#include <game/gameprocessor.h>
#include <game/global_states.h>
#include <game/levelcontainer.h>

bool drawAxis = true;

GameScreen::GameScreen(GEC::UI::ElementRenderer* elr, LevelContainer* levelContainer)
{
    this->elr = elr;
    this->lc = levelContainer;
}
void GameScreen::Open()
{

    elr->ClearCycle();
    GEC::MenuManager* mm = lc->GetMenuManager();
    mm->AddMenu(0, new GAME_BlankMenu());
    mm->AddMenu(1, new GAME_PauseMenu(elr, lc, this->GetManager()));

    GEC::AudioEngine::GetInstance().PlaySound("BG_MUSIC");
};
void GameScreen::Update()
{
    lc->GetMenuManager()->Update();
    lc->Update();
    if (!lc->Paused()) {
        lc->Tick();
        if (lc->GameWon() && GEC::AudioEngine::GetInstance().SoundFinished("finish_line")) {
            GEC::AudioEngine::GetInstance().StopSound("finish_line");
            GEC::AudioEngine::GetInstance().StopSound("BG_MUSIC");
            if (this->GetManager()->Exists(2))
                this->GetManager()->RemoveMenu(2);
            this->GetManager()->AddMenu(2, new WinScreen(elr, lc->Coins(), lc->TimeRemaining()));
            this->GetManager()->GoTo(2);
        }
    }
    if (lc->GetPlayer()->Position()->Second() <= -10)
        lc->GetPlayer()->Kill();
    if (lc->GetPlayer()->IsDead())
        GetManager()->GoTo(3);
    if (lc->TimeRemaining() <= 0)
        GetManager()->GoTo(3);

    if (GEC::Input::Keyboard::IsKeyPressed(97))
        drawAxis = !drawAxis;

    if (GEC::AudioEngine::GetInstance().SoundFinished("BG_MUSIC")) {
        GEC::AudioEngine::GetInstance().PlaySound("BG_MUSIC");
    }
};
void GameScreen::Render()
{
    GEC::Camera& cam = GEC::Camera::GetInstance();

    cam.SetScale(cam.ViewPort().Second() / 15.0f);
    GEC::Vector3<float, float, float>
        cPos
        = cam.Position();
    float scl = cam.GetScale();

    if (drawAxis) {
        GEC::Render::SetColor(0, 255, 0, 80);
        GEC::Render::Arrow(GEC::Vector3<float, float, float>(0, 0, -1), GEC::Vector3<float, float, float>(0, cam.ViewPort().Second() / 2 - 20, -1), 2, 20);
        GEC::Render::SetColor(255, 0, 0, 80);
        GEC::Render::Arrow(GEC::Vector3<float, float, float>(0, 0, -1), GEC::Vector3<float, float, float>(cam.ViewPort().First() / 2 - 20, 0, -1), 2, 20);
    }

    glPushMatrix();
    glScalef(scl, scl, 1);
    glTranslatef(-cPos.First(), -cPos.Second(), -cPos.Third());
    lc->Render();
    glPopMatrix();

    GEC::Render::SetColor(255);
    GEC::TextRender::Text t(GetGlobalFont());
    t.Align(0, 2);

    glPushMatrix();
    glTranslatef(-cam.ViewPort().First() / 2, cam.ViewPort().Second() / 2, 0.01);

    t.SetText("FPS: " + std::to_string((int)FPS_AVERAGE()));
    t.Render(0, -1, 0, 18);
    t.SetText("Time: " + std::to_string((int)round(lc->TimeRemaining())));
    t.Render(0, -20, 0, 18);
    t.SetText("Coins: " + std::to_string(lc->Coins()));
    t.Render(0, -40, 0, 18);
    glPopMatrix();

    lc->GetMenuManager()->Render();
};
void GameScreen::Events()
{
    lc->GetMenuManager()->Events();
};

void GameScreen::Leave()
{

    GEC::AudioEngine::GetInstance().StopSound("BG_MUSIC");
    GEC::Camera::GetInstance().SetScale(1);
    delete lc;
};

GAME_PauseMenu::GAME_PauseMenu(GEC::UI::ElementRenderer* elr, LevelContainer* lc, GEC::MenuManager* globalManager)
{
    this->elr = elr;
    this->lc = lc;
    this->globalManager = globalManager;
}
void GAME_PauseMenu::Open()
{
    lc->Pause();
    CreateElements();
    CreatePage();
    GEC::AudioEngine::GetInstance().PauseSound("BG_MUSIC");
};
void GAME_PauseMenu::Update() {

};
void GAME_PauseMenu::Render() {

};
void GAME_PauseMenu::Events()
{
    if (GEC::Input::Keyboard::IsKeyPressed(112) || backButton->Clicked())
        this->GetManager()->GoTo(0);

    if (editButton->Clicked()) {
        globalManager->GoTo(10);
    }
};
void GAME_PauseMenu::Leave()
{
    elr->ClearCycle();
    delete pausedText;
    delete bgPanel;
    delete backButton;

    lc->UnPause();
    GEC::AudioEngine::GetInstance().PlaySound("BG_MUSIC");
};
void GAME_PauseMenu::CreateElements()
{
    bgPanel = new GEC::UI::Elements::Panel(0, 150);
    pausedText = new GEC::UI::Elements::TextDisplay("Game Paused");
    backButton = new GEC::UI::Elements::Button();
    editButton = new GEC::UI::Elements::Button();
}
void GAME_PauseMenu::CreatePage()
{
    elr->ClearCycle();
    bgPanel->Set(0, 0, GEC::Camera::GetInstance().ViewPort().First(), GEC::Camera::GetInstance().ViewPort().Second());

    pausedText->Align(1, 1);
    pausedText->SetFont(GetGlobalFont());
    pausedText->SetColor(255);
    pausedText->Set(0, 30, 100, 28);

    backButton->Set("Back to Game", 0, -30, 200, 28);
    editButton->Set("Open Editor", 0, -60, 200, 28);

    elr->AddElement(bgPanel, 0);
    elr->AddElement(pausedText, 0);
    elr->AddElement(backButton, 0);
#if EDITOR_BUILD
    elr->AddElement(editButton, 0);
#endif
};

GAME_BlankMenu::GAME_BlankMenu() { };
void GAME_BlankMenu::Open() { };
void GAME_BlankMenu::Update() { };
void GAME_BlankMenu::Render() { };
void GAME_BlankMenu::Events()
{
    if (GEC::Input::Keyboard::IsKeyPressed(112))
        this->GetManager()->GoTo(1);
};
void GAME_BlankMenu::Leave() { };