#include <game/entities/objects.h>

#include <engine/renderobjects.h>
#include <math.h>

Coin::Coin()
    : Entity(0, 0, .3, .3)
{
    mass = 3.0f;
    force.Move(20, 0);
}
void Coin::Update() {

};
void Coin::Tick()
{
    bool solidGroud = OnSolidGround();

    float lastForce = round(force.Second() * 100.0f) / 100.0f;
    ProcessForce();
    if (force.Second() == 0) {
        force.Move(0, -lastForce * 0.7f);
    }
    if (!solidGroud) {
        friction.Set(0, 0);
    } else {
        friction.Set(10.0f, 0);
    }
    ProcessGravity();
};
void Coin::Render()
{
    GEC::Rect<float, float, float, float>* r = this->position_size;
    GEC::Render::SetColor(255);
    GEC::Render::Sprite("game.entities", r->X(), r->Y(), r->W(), r->H(), GEC::Vector2<int, int>(1, 13), 16);
};
Coin::~Coin() { };
void Coin::OnSpawn() { };
void Coin::OnDeath() { };