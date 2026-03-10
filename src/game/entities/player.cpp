#if defined(use_freeglut)
#include <glad/glad.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h>
#endif

#include <game/entities/player.h>

#include <engine/renderobjects.h>

#include <engine/input.h>
#include <engine/structs.h>
#include <game/deltatime.h>
#include <iostream>
#include <math.h>


#include <game/camera.h>

Player::Player()
    : Entity(0, 0, 1, 1)
{
    this->position_size->X() = 0;
    this->position_size->Y() = 0;
}
void Player::Update()
{
    GEC::Rect<float, float, float, float>* r = this->position_size;

    float speed = GetMainDeltaTime()*5;
    if (GEC::Input::Keyboard::IsSpecialKeyDown(101)) {
        r->Y() += speed;
    }
    if (GEC::Input::Keyboard::IsSpecialKeyDown(103)) {
        r->Y() -= speed;
    }
    if (GEC::Input::Keyboard::IsSpecialKeyDown(100)) {
        r->X() -= speed;
        _dir = -1;
    }
    if (GEC::Input::Keyboard::IsSpecialKeyDown(102)) {
        r->X() += speed;
        _dir = 1;
    }
    if (abs(r->X() - _lastAniX) > 40) {
        _lastAniX = r->X();
        ++_imgX;
        if (_imgX > 2)
            _imgX = 1;
    }

    Camera::GetInstance().Set(position_size->First());
}
void Player::Render()
{
    GEC::Rect<float, float, float, float>* r = this->position_size;
    GEC::Render::SetColor(255);
    GEC::Render::Rect(r->X(), r->Y(), r->W(), r->H());

    GEC::Render::Sprite("game.entities", r->X(), r->Y(), r->W() * _dir, r->H(), GEC::Vector2<int, int>(_imgX, _imgY), 16);
}
Player::~Player()
{
}
void Player::OnSpawn() { }
void Player::OnDeath() { }