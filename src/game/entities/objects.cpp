#include <game/entities/objects.h>

#include <engine/renderobjects.h>
#include <engine/tools.h>
#include <game/entities/player.h>
#include <math.h>
#include <string.h>

static GEC::Game::GameObjectRegistrar<Coin> regobj_coin("com.arizotaz.gec.entity.coin");
static GEC::Game::GameObjectRegistrar<FinishLine> regobj_finishline("com.arizotaz.gec.entity.finish");

Coin::Coin()
    : Entity("com.arizotaz.gec.entity.coin", 0, 0, .3, .3)
{
    mass = 3.0f;
    force.Move(20, 0);
    this->name = "Coin";
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

    EntityManager* em = this->Manager();
    std::vector<int> list = em->List();
    for (int i = 0; i < list.size(); ++i) {
        Entity* entity = em->Get(list[i]);
        if (strcmp(entity->GetType(), "gec.assign2.player") == 0) {
            if (GEC::Tools::Distance(*(entity->Position()), *(this->Position())) < this->Size().First() + entity->Size().First()) {
                Player* p = dynamic_cast<Player*>(entity);
                p->collectedCoins++;
                this->Kill();
            }
        }
    }
};
void Coin::Render()
{
    GEC::Rect<float, float, float, float> r(position->First(), position->Second(), size.First(), size.Second());
    GEC::Render::SetColor(255);
    GEC::Render::Sprite("game.entities", 0, 0, r.W(), r.H(), GEC::Vector2<int, int>(1, 13), 16);
};
Coin::~Coin() { };
void Coin::OnSpawn() { };
void Coin::OnDeath() { };

FinishLine::FinishLine()
    : Entity("com.arizotaz.gec.entity.finish", 0, 0, 6, 0.5)
{
    this->mass = 3;
    this->name = "FinishLine";
};

void FinishLine::Update() {

};
void FinishLine::Tick()
{
    float dtime = GetMainDeltaTime() / 1000.0f;
    ProcessForce();
    ProcessGravity();

    Player* p = lc->GetPlayer();
    GEC::Vector2<float, float>* pos = p->Position();
    GEC::Vector2<float, float> size = p->Size();

    GEC::Physics::BoxCollider2D player_collider(pos->First(), pos->Second(), size.First(), size.Second());
    GEC::Physics::BoxCollider2D my_collider(Position()->First(), Position()->Second(), Size().First() - 1, Size().Second());

    if (my_collider.IsColliding(player_collider)) {
        if (!lc->GameWon()) {
            GEC::AudioEngine::GetInstance().PlaySound("finish_line");
            lc->MarkWin();
        }
    }
};
void FinishLine::Render()
{
    GEC::Rect<float, float, float, float> r(position->First(), position->Second(), size.First(), size.Second());
    GEC::Render::SetColor(255);
    GEC::Render::Image("game.finish.base", 0, 0 - r.H(), 1.1, r.W(), r.H());
    GEC::Render::Image("game.finish.flag", 0 - r.W() / 2 + (r.W() / 4) / 2, 0 + r.H() * 4 / 2, 1.05, r.W() / 3, r.H() * 6);
};
FinishLine::~FinishLine() { };

void FinishLine::SetLevelContainer(LevelContainer* lc) { this->lc = lc; }

void FinishLine::WriteObject(std::ostream& out) const { }
void FinishLine::ReadObject(std::istream& in) { }