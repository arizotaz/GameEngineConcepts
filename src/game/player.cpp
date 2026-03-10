#if defined(use_freeglut)
#include <glad/glad.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h>
#endif

#include <game/player.h>

#include <engine/renderobjects.h>

#include <engine/input.h>
#include <engine/structs.h>
#include <game/deltatime.h>
#include <iostream>
#include <math.h>

Player::Player(int x, int y)
{
    _x = x;
    _y = y;
    _w = 100;
    _h = 100;


}
void Player::Update()
{

    float speed = GetMainDeltaTime()*500;
    if (GEC::Input::Keyboard::IsSpecialKeyDown(101)) {
        _y += speed;
    }
    if (GEC::Input::Keyboard::IsSpecialKeyDown(103)) {
        _y -= speed;
    }
    if (GEC::Input::Keyboard::IsSpecialKeyDown(100)) {
        _x -= speed;
        _dir = -1;
    }
    if (GEC::Input::Keyboard::IsSpecialKeyDown(102)) {
        _x += speed;
        _dir = 1;
    }
    if (abs(_x - _lastAniX) > 40) {
        _lastAniX = _x;
        ++_imgX;
        if (_imgX > 2) _imgX = 1;
    }
}
void Player::Render()
{
    GEC::Render::SetColor(255);
    GEC::Render::Sprite("game.entities",_x,_y,_w*_dir,_h,GEC::Vector2<int,int>(_imgX,_imgY), 16);
}
Player::~Player()
{
}