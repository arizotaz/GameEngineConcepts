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
#include <iostream>
#include <math.h>

#include <game/deltatime.h>

#include <game/camera.h>

#include <engine/tools.h>

Player::Player()
    : Entity(0, 0, .8f,.8f)
{
    this->mass = 3;
    this->friction.Set(10.0f,0);
}
void Player::Update()
{
    float dtime = GetMainDeltaTime() / 1000.0f;

    float input_x_axis = 0;
    float input_jump = 0;
    if (GEC::Input::Keyboard::IsSpecialKeyDown(101)) {
        input_jump = 1;
    }
    if (GEC::Input::Keyboard::IsSpecialKeyDown(100)) {
        input_x_axis = -1;
    }
    if (GEC::Input::Keyboard::IsSpecialKeyDown(102)) {
        input_x_axis = 1;
    }

    this->input_x_axis = input_x_axis;
    this->input_jump = input_jump;




    GEC::Vector3<float,float,float> pos = Camera::GetInstance().Position();
    float dist = GEC::Tools::Distance(position_size->X(), position_size->Y(), pos.First(), pos.Second());
		float tdis = 2;
		if (dist > tdis) {
			Camera::GetInstance().MoveTo(position_size->X(), position_size->Y(), (float)pow((dist - tdis),2) * 20 * (dtime/10.0f));
		}
}

void Player::Tick()
{
    float dtime = GetMainDeltaTime() / 1000.0f;

    bool solidGroud = OnSolidGround();

    {
        if (solidGroud) {
            isInAJump = false;
            if (input_jump > 0 && !jumped) {
                jumped = true;
                // forceY = 35; - two tiles
                force.Second() = 50;
                isInAJump = true;
            }
        } else {
            jumped = false;
        }
        float mForce = 280;
        float left = input_left;
        float right = input_right;
        float x_axis = input_x_axis;
        float multiplier = (left + right);
        if (abs(x_axis) > abs(multiplier)) {
            multiplier = x_axis;
        }
        if (abs(multiplier) < .10) {
            multiplier = 0;
        }

        mForce = mForce * multiplier;
        mForce = mForce * dtime;
        force.First() += mForce;
    }

    if (isInAJump)
        if (force.Second() > 0)
            if (input_jump <= 0)
                force.Second() -= 100 * dtime;

    ProcessForce();
    if (force.Second() == 0) {
        jumped = false;
    }
    ProcessGravity();

    _imgX = 0;

    int rx = round(force.First());
    int ry = round(force.Second());
    if (rx > 0) {
        _dir = 1;
    }
    if (rx < 0) {
        _dir = -1;
    }
    if (rx != 0) {
        walk_ani += dtime * abs(force.First() / 2.0f);
        if (walk_ani >= 2) {
            walk_ani = 0;
        }
        _imgX = 8 + 8 * (int)walk_ani;
    }
    if (!solidGroud) {
        _imgX = 24;
    }

    // int used = attack.Process(this);
    // if (used != 0) {
    //     castani = .25f;
    //     dir = used;
    // }
    // if (castani > 0) {
    //     castani -= dtime;
    //     castani = Tools.SetSmallest(castani, 0);
    //     imgX = 32;
    // }

    walk_time += dtime * abs(force.First() / 2.0f);
    if (walk_time >= 1 && _imgX != 24) {
        walk_time = 0;
        // Play Step Audio
    }
}
void Player::Render()
{
    GEC::Rect<float, float, float, float>* r = this->position_size;
    GEC::Render::SetColor(255);
    GEC::Render::Sprite("game.entities", r->X(), r->Y(), r->W() * _dir, r->H(), GEC::Vector2<int, int>(_imgX/8, _imgY/8), 16);
}
Player::~Player()
{
}
void Player::OnSpawn() { }
void Player::OnDeath() { }